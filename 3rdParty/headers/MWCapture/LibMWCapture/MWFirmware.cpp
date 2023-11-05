#include "stdafx.h"
#include "MWFirmware.h"

DWORD CalcCheckSum(LPBYTE pbyData, DWORD cbData)
{
	DWORD dwCheckSum = 0;
	while (cbData--)
		dwCheckSum += *pbyData++;

	return dwCheckSum;
}

BOOL VerifyData(MW_FIRMWARE_HEADER * pHeader, LPBYTE apbySectionData[])
{
	DWORD dwCheckSum = CalcCheckSum((LPBYTE)(&pHeader->infoHeader.wVersion), pHeader->infoHeader.cbHeader - 8);
	BOOL bRet = (pHeader->infoHeader.dwCheckSum == dwCheckSum);

	for (int i = 0; bRet && i < pHeader->infoHeader.cSections; i++) {
		dwCheckSum = CalcCheckSum(apbySectionData[i], pHeader->aSectionHeaders[i].cbSection);
		bRet = (pHeader->aSectionHeaders[i].dwCheckSum == dwCheckSum);
	}

	return bRet;
}

VOID FreeSectionData(LPBYTE apbySectionData[], int cSections)
{
	for (int i = 0; i < cSections; i++) {
		if (apbySectionData[i])
			free(apbySectionData[i]);
	}
}

BOOL ReadMWF(LPCSTR pszPath, MW_FIRMWARE_HEADER * pHeader, LPBYTE apbySectionData[])
{
	FILE * fp = fopen(pszPath, "rb");
	BOOL bRet = (fp != NULL);

	if (bRet)
		bRet = (sizeof(pHeader->infoHeader) == fread(&pHeader->infoHeader, 1, sizeof(pHeader->infoHeader), fp));

	if (bRet)
		bRet = (pHeader->infoHeader.dwMagic == MW_FIRMWARE_HEADER_MAGIC && pHeader->infoHeader.wVersion == MW_FIRMWARE_HEADER_VERSION);
	
	int cbSectionHeaders;

	if (bRet) {
		cbSectionHeaders = pHeader->infoHeader.cSections * sizeof(MW_FIRMWARE_SECTION_HEADER);
		bRet = (pHeader->infoHeader.cSections <= MW_MAX_NUM_FIRMWARE_SECTIONS
			&& pHeader->infoHeader.cbHeader == (sizeof(pHeader->infoHeader) + cbSectionHeaders));
	}

	if (bRet)
		bRet = (cbSectionHeaders == fread(pHeader->aSectionHeaders, 1, cbSectionHeaders, fp));

	for (int i = 0; bRet && i < pHeader->infoHeader.cSections; i++) {
		apbySectionData[i] = (LPBYTE)malloc(pHeader->aSectionHeaders[i].cbSection);

		if (NULL == apbySectionData[i])
			bRet = FALSE;
		else
			bRet = (pHeader->aSectionHeaders[i].cbSection == fread(apbySectionData[i], 1, pHeader->aSectionHeaders[i].cbSection, fp));
	}

	if (!bRet)
		FreeSectionData(apbySectionData, pHeader->infoHeader.cSections);

	if (fp)
		fclose(fp);

	return bRet;
}

BOOL WriteMWF(LPCSTR pszPath, MW_FIRMWARE_HEADER * pHeader, LPBYTE apbySectionData[])
{
	FILE * fp = fopen(pszPath, "wb");
	BOOL bRet = (fp != NULL);

	if (bRet)
		bRet = (pHeader->infoHeader.cbHeader == fwrite(pHeader, 1, pHeader->infoHeader.cbHeader, fp));

	for (int i = 0; bRet && i < pHeader->infoHeader.cSections; i++)
		bRet = (pHeader->aSectionHeaders[i].cbSection == fwrite(apbySectionData[i], 1, pHeader->aSectionHeaders[i].cbSection, fp));

	if (fp)
		fclose(fp);
	return bRet;
}

BOOL ReadFirmware(CKsMWCAPFilter * pFilter, MW_FIRMWARE_HEADER * pHeader, LPBYTE apbySectionData[])
{
	DWORD cbRead;

	MWCAP_FIRMWARE_STORAGE storageInfo;
	BOOL bRet = SUCCEEDED(pFilter->GetFirmwareStorageInfo(&storageInfo));

	if (bRet) {
		bRet = SUCCEEDED(pFilter->ReadFirmwareData(storageInfo.cbHeaderOffset, (BYTE *)pHeader, sizeof(pHeader->infoHeader), &cbRead))
			&& (cbRead == sizeof(pHeader->infoHeader));
	}

	int cbSectionHeaders;

	if (bRet) {
		cbSectionHeaders = pHeader->infoHeader.cSections * sizeof(MW_FIRMWARE_SECTION_HEADER);
		bRet = (pHeader->infoHeader.cSections <= MW_MAX_NUM_FIRMWARE_SECTIONS
			&& pHeader->infoHeader.cbHeader == (sizeof(pHeader->infoHeader) + cbSectionHeaders));
	}

	if (bRet) {
		bRet = SUCCEEDED(pFilter->ReadFirmwareData(storageInfo.cbHeaderOffset + sizeof(pHeader->infoHeader), (BYTE *)pHeader->aSectionHeaders, cbSectionHeaders, &cbRead))
			&& (cbRead == cbSectionHeaders);
	}

	for (int i = 0; bRet && i < pHeader->infoHeader.cSections; i++) {
		apbySectionData[i] = (LPBYTE)malloc(pHeader->aSectionHeaders[i].cbSection);

		if (NULL == apbySectionData[i])
			bRet = FALSE;
		else
			bRet = SUCCEEDED(pFilter->ReadFirmwareData(pHeader->aSectionHeaders[i].cbOffset, apbySectionData[i], pHeader->aSectionHeaders[i].cbSection, &cbRead))
				&& (cbRead == pHeader->aSectionHeaders[i].cbSection);
	}

	if (!bRet)
		FreeSectionData(apbySectionData, pHeader->infoHeader.cSections);

	return bRet;
}

BOOL WriteFirmware(CKsMWCAPFilter * pFilter, MW_FIRMWARE_HEADER * pHeader, LPBYTE apbySectionData[])
{
	// Verify firmware compatible ID
	MWCAP_CHANNEL_INFO channelInfo;
	BOOL bRet = SUCCEEDED(pFilter->GetChannelInfo(&channelInfo));

	if (bRet) {
		bRet = (channelInfo.byFirmwareID == pHeader->infoHeader.byFirmwareID
			&& channelInfo.wProductID == pHeader->infoHeader.wProductID
			&& channelInfo.chHardwareVersion == pHeader->infoHeader.chHardwareVersion);
	}

	// Get storage information
	MWCAP_FIRMWARE_STORAGE storageInfo;
	if (bRet)
		bRet = SUCCEEDED(pFilter->GetFirmwareStorageInfo(&storageInfo));

	// Erase old firmware data
	if (bRet)
		bRet = SUCCEEDED(pFilter->EraseFirmwareData(storageInfo.cbHeaderOffset, pHeader->infoHeader.cbHeader));

	for (int i = 0; bRet && i < pHeader->infoHeader.cSections; i++)
		bRet = SUCCEEDED(pFilter->EraseFirmwareData(pHeader->aSectionHeaders[i].cbOffset, pHeader->aSectionHeaders[i].cbSection));

	// Write new firmware data
	if (bRet)
		bRet = SUCCEEDED(pFilter->WriteFirmwareData(storageInfo.cbHeaderOffset, (BYTE *)pHeader, pHeader->infoHeader.cbHeader));

	for (int i = 0; bRet && i < pHeader->infoHeader.cSections; i++)
		bRet = SUCCEEDED(pFilter->WriteFirmwareData(pHeader->aSectionHeaders[i].cbOffset, apbySectionData[i], pHeader->aSectionHeaders[i].cbSection));

	return bRet;
}

BOOL BackupFirmware(
	CKsMWCAPFilter * pFilter,
	LPCSTR pszMWFPath
	)
{
	MW_FIRMWARE_HEADER header = { 0 };
	LPBYTE apbySectionData[MW_MAX_NUM_FIRMWARE_SECTIONS] = { 0 };

	BOOL bRet = ReadFirmware(pFilter, &header, apbySectionData);

	if (bRet)
		bRet = VerifyData(&header, apbySectionData);

	if (bRet)
		bRet = WriteMWF(pszMWFPath, &header, apbySectionData);

	FreeSectionData(apbySectionData, header.infoHeader.cSections);
	return bRet;
}

BOOL UpgradeFirmware(
	CKsMWCAPFilter * pFilter,
	LPCSTR pszMWFPath
	)
{
	MW_FIRMWARE_HEADER header = { 0 };
	LPBYTE apbySectionData[MW_MAX_NUM_FIRMWARE_SECTIONS] = { 0 };

	BOOL bRet = ReadMWF(pszMWFPath, &header, apbySectionData);

	if (bRet)
		bRet = VerifyData(&header, apbySectionData);
	
	if (bRet)
		bRet = WriteFirmware(pFilter, &header, apbySectionData);

	FreeSectionData(apbySectionData, header.infoHeader.cSections);
	return bRet;
}
