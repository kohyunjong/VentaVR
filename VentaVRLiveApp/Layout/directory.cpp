#include "directory.h"

int getDirectoryFiles(char* folderPath, char** file_list)
{
	struct _finddata_t c_file;
	int i = 0;
	char path[1024];
	char temp[1024];
	sprintf(path, "%s\\*.jpg", folderPath);
	printf(":::> %s\n", path);
	intptr_t hFile = _findfirst(path, &c_file);
	if (hFile == -1) return 0;      // no files
	do
	{
		if (c_file.attrib & _A_SUBDIR)
		{
			// directory 贸府
		}
		else if (c_file.attrib & _A_HIDDEN)
		{
			// hidden file 贸府
		}
		else
		{
			// 老馆 颇老 贸府			
			sprintf(temp, "%s\\%s", folderPath, c_file.name);
			printf("%s\n", temp);
			strcpy(file_list[i++], temp);
		}
	} while (_findnext(hFile, &c_file) == 0);
	_findclose(hFile);
	return 1;
}