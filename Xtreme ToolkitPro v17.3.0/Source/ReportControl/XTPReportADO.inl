// Created by Microsoft (R) C/C++ Compiler Version 11.00.0000 (c8fff05a).
//
// Debug/msado10.tlh
//
// C++ source equivalent of Win32 type library c:\program files\common files\system\ado\msado10.dll
// compiler-generated file created 03/29/05 at 17:13:40 - DO NOT EDIT!
//
// (c)1998-2016 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#pragma pack(push, 8)

#include <comdef.h>

#pragma warning(disable:4510 4513 4610)

namespace XTPREPORTADODB {

//
// interface _Collection wrapper method implementations
//

inline long _Collection::GetCount ( ) {
    long _result;
    HRESULT _hr = get_Count(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline IUnknownPtr _Collection::_NewEnum ( ) {
    IUnknown * _result;
    HRESULT _hr = raw__NewEnum(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IUnknownPtr(_result, false);
}

inline HRESULT _Collection::Refresh ( ) {
    HRESULT _hr = raw_Refresh();
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

//
// interface _DynaCollection wrapper method implementations
//

inline HRESULT _DynaCollection::Append ( IDispatch * Object ) {
    HRESULT _hr = raw_Append(Object);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT _DynaCollection::Delete ( const _variant_t & Index ) {
    HRESULT _hr = raw_Delete(Index);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

//
// interface _ADO wrapper method implementations
//

inline PropertiesPtr _ADO::GetProperties ( ) {
    struct Properties * _result;
    HRESULT _hr = get_Properties(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return PropertiesPtr(_result, false);
}

//
// interface Properties wrapper method implementations
//

inline PropertyPtr Properties::GetItem ( const _variant_t & Index ) {
    struct Property * _result;
    HRESULT _hr = get_Item(Index, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return PropertyPtr(_result, false);
}

//
// interface Property wrapper method implementations
//

inline _variant_t Property::GetValue ( ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = get_Value(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _variant_t(_result, false);
}

inline void Property::PutValue ( const _variant_t & pval ) {
    HRESULT _hr = put_Value(pval);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline _bstr_t Property::GetName ( ) {
    BSTR _result;
    HRESULT _hr = get_Name(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline enum DataTypeEnum Property::GetType ( ) {
    enum DataTypeEnum _result;
    HRESULT _hr = get_Type(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline long Property::GetAttributes ( ) {
    long _result;
    HRESULT _hr = get_Attributes(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void Property::PutAttributes ( long plAttributes ) {
    HRESULT _hr = put_Attributes(plAttributes);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

//
// interface Error wrapper method implementations
//

inline long Error::GetNumber ( ) {
    long _result;
    HRESULT _hr = get_Number(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline _bstr_t Error::GetSource ( ) {
    BSTR _result;
    HRESULT _hr = get_Source(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline _bstr_t Error::GetDescription ( ) {
    BSTR _result;
    HRESULT _hr = get_Description(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline _bstr_t Error::GetHelpFile ( ) {
    BSTR _result;
    HRESULT _hr = get_HelpFile(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline long Error::GetHelpContext ( ) {
    long _result;
    HRESULT _hr = get_HelpContext(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline _bstr_t Error::GetSQLState ( ) {
    BSTR _result;
    HRESULT _hr = get_SQLState(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline long Error::GetNativeError ( ) {
    long _result;
    HRESULT _hr = get_NativeError(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

//
// interface Errors wrapper method implementations
//

inline ErrorPtr Errors::GetItem ( const _variant_t & Index ) {
    struct Error * _result;
    HRESULT _hr = get_Item(Index, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return ErrorPtr(_result, false);
}

inline HRESULT Errors::Clear ( ) {
    HRESULT _hr = raw_Clear();
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

//
// interface Command15 wrapper method implementations
//

inline _ConnectionPtr Command15::GetActiveConnection ( ) {
    struct _Connection * _result;
    HRESULT _hr = get_ActiveConnection(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _ConnectionPtr(_result, false);
}

inline void Command15::PutRefActiveConnection ( struct _Connection * ppvObject ) {
    HRESULT _hr = putref_ActiveConnection(ppvObject);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline void Command15::PutActiveConnection ( const _variant_t & ppvObject ) {
    HRESULT _hr = put_ActiveConnection(ppvObject);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline _bstr_t Command15::GetCommandText ( ) {
    BSTR _result;
    HRESULT _hr = get_CommandText(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline void Command15::PutCommandText ( _bstr_t pbstr ) {
    HRESULT _hr = put_CommandText(pbstr);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline long Command15::GetCommandTimeout ( ) {
    long _result;
    HRESULT _hr = get_CommandTimeout(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void Command15::PutCommandTimeout ( long pl ) {
    HRESULT _hr = put_CommandTimeout(pl);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline VARIANT_BOOL Command15::GetPrepared ( ) {
    VARIANT_BOOL _result;
    HRESULT _hr = get_Prepared(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void Command15::PutPrepared ( VARIANT_BOOL pfPrepared ) {
    HRESULT _hr = put_Prepared(pfPrepared);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline _RecordsetPtr Command15::Execute ( VARIANT * RecordsAffected, VARIANT * Parameters, long Options ) {
    struct _Recordset * _result;
    HRESULT _hr = raw_Execute(RecordsAffected, Parameters, Options, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _RecordsetPtr(_result, false);
}

inline _ParameterPtr Command15::CreateParameter ( _bstr_t Name, enum DataTypeEnum Type, enum ParameterDirectionEnum Direction, ADO_LONGPTR Size, const _variant_t & Value ) {
    struct _Parameter * _result;
    HRESULT _hr = raw_CreateParameter(Name, Type, Direction, Size, Value, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _ParameterPtr(_result, false);
}

inline ParametersPtr Command15::GetParameters ( ) {
    struct Parameters * _result;
    HRESULT _hr = get_Parameters(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return ParametersPtr(_result, false);
}

inline void Command15::PutCommandType ( enum CommandTypeEnum plCmdType ) {
    HRESULT _hr = put_CommandType(plCmdType);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline enum CommandTypeEnum Command15::GetCommandType ( ) {
    enum CommandTypeEnum _result;
    HRESULT _hr = get_CommandType(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline _bstr_t Command15::GetName ( ) {
    BSTR _result;
    HRESULT _hr = get_Name(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline void Command15::PutName ( _bstr_t pbstrName ) {
    HRESULT _hr = put_Name(pbstrName);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

//
// interface Connection15 wrapper method implementations
//

inline _bstr_t Connection15::GetConnectionString ( ) {
    BSTR _result;
    HRESULT _hr = get_ConnectionString(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline void Connection15::PutConnectionString ( _bstr_t pbstr ) {
    HRESULT _hr = put_ConnectionString(pbstr);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline long Connection15::GetCommandTimeout ( ) {
    long _result;
    HRESULT _hr = get_CommandTimeout(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void Connection15::PutCommandTimeout ( long plTimeout ) {
    HRESULT _hr = put_CommandTimeout(plTimeout);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline long Connection15::GetConnectionTimeout ( ) {
    long _result;
    HRESULT _hr = get_ConnectionTimeout(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void Connection15::PutConnectionTimeout ( long plTimeout ) {
    HRESULT _hr = put_ConnectionTimeout(plTimeout);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline _bstr_t Connection15::GetVersion ( ) {
    BSTR _result;
    HRESULT _hr = get_Version(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline HRESULT Connection15::Close ( ) {
    HRESULT _hr = raw_Close();
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline _RecordsetPtr Connection15::Execute ( _bstr_t CommandText, VARIANT * RecordsAffected, long Options ) {
    struct _Recordset * _result;
    HRESULT _hr = raw_Execute(CommandText, RecordsAffected, Options, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _RecordsetPtr(_result, false);
}

inline long Connection15::BeginTrans ( ) {
    long _result;
    HRESULT _hr = raw_BeginTrans(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline HRESULT Connection15::CommitTrans ( ) {
    HRESULT _hr = raw_CommitTrans();
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT Connection15::RollbackTrans ( ) {
    HRESULT _hr = raw_RollbackTrans();
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT Connection15::Open ( _bstr_t ConnectionString, _bstr_t UserID, _bstr_t Password, long Options ) {
    HRESULT _hr = raw_Open(ConnectionString, UserID, Password, Options);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline ErrorsPtr Connection15::GetErrors ( ) {
    struct Errors * _result;
    HRESULT _hr = get_Errors(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return ErrorsPtr(_result, false);
}

inline _bstr_t Connection15::GetDefaultDatabase ( ) {
    BSTR _result;
    HRESULT _hr = get_DefaultDatabase(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline void Connection15::PutDefaultDatabase ( _bstr_t pbstr ) {
    HRESULT _hr = put_DefaultDatabase(pbstr);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline enum IsolationLevelEnum Connection15::GetIsolationLevel ( ) {
    enum IsolationLevelEnum _result;
    HRESULT _hr = get_IsolationLevel(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void Connection15::PutIsolationLevel ( enum IsolationLevelEnum Level ) {
    HRESULT _hr = put_IsolationLevel(Level);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline long Connection15::GetAttributes ( ) {
    long _result;
    HRESULT _hr = get_Attributes(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void Connection15::PutAttributes ( long plAttr ) {
    HRESULT _hr = put_Attributes(plAttr);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline enum CursorLocationEnum Connection15::GetCursorLocation ( ) {
    enum CursorLocationEnum _result;
    HRESULT _hr = get_CursorLocation(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void Connection15::PutCursorLocation ( enum CursorLocationEnum plCursorLoc ) {
    HRESULT _hr = put_CursorLocation(plCursorLoc);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline enum ConnectModeEnum Connection15::GetMode ( ) {
    enum ConnectModeEnum _result;
    HRESULT _hr = get_Mode(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void Connection15::PutMode ( enum ConnectModeEnum plMode ) {
    HRESULT _hr = put_Mode(plMode);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline _bstr_t Connection15::GetProvider ( ) {
    BSTR _result;
    HRESULT _hr = get_Provider(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline void Connection15::PutProvider ( _bstr_t pbstr ) {
    HRESULT _hr = put_Provider(pbstr);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline long Connection15::GetState ( ) {
    long _result;
    HRESULT _hr = get_State(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline _RecordsetPtr Connection15::OpenSchema ( enum SchemaEnum Schema, const _variant_t & Restrictions, const _variant_t & SchemaID ) {
    struct _Recordset * _result;
    HRESULT _hr = raw_OpenSchema(Schema, Restrictions, SchemaID, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _RecordsetPtr(_result, false);
}

//
// interface _Connection wrapper method implementations
//

inline HRESULT _Connection::Cancel ( ) {
    HRESULT _hr = raw_Cancel();
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

//
// interface Recordset15 wrapper method implementations
//

inline PositionEnum_Param Recordset15::GetAbsolutePosition ( ) {
    PositionEnum_Param _result;
    HRESULT _hr = get_AbsolutePosition(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void Recordset15::PutAbsolutePosition ( PositionEnum_Param pl ) {
    HRESULT _hr = put_AbsolutePosition(pl);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline void Recordset15::PutRefActiveConnection ( IDispatch * pvar ) {
    HRESULT _hr = putref_ActiveConnection(pvar);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline void Recordset15::PutActiveConnection ( const _variant_t & pvar ) {
    HRESULT _hr = put_ActiveConnection(pvar);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline _variant_t Recordset15::GetActiveConnection ( ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = get_ActiveConnection(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _variant_t(_result, false);
}

inline VARIANT_BOOL Recordset15::GetbBOF ( ) {
    VARIANT_BOOL _result;
    HRESULT _hr = get_bBOF(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline _variant_t Recordset15::GetBookmark ( ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = get_Bookmark(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _variant_t(_result, false);
}

inline void Recordset15::PutBookmark ( const _variant_t & pvBookmark ) {
    HRESULT _hr = put_Bookmark(pvBookmark);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline long Recordset15::GetCacheSize ( ) {
    long _result;
    HRESULT _hr = get_CacheSize(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void Recordset15::PutCacheSize ( long pl ) {
    HRESULT _hr = put_CacheSize(pl);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline enum CursorTypeEnum Recordset15::GetCursorType ( ) {
    enum CursorTypeEnum _result;
    HRESULT _hr = get_CursorType(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void Recordset15::PutCursorType ( enum CursorTypeEnum plCursorType ) {
    HRESULT _hr = put_CursorType(plCursorType);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline VARIANT_BOOL Recordset15::GetbEOF ( ) {
    VARIANT_BOOL _result;
    HRESULT _hr = get_bEOF(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline FieldsPtr Recordset15::GetFields ( ) {
    struct Fields * _result;
    HRESULT _hr = get_Fields(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return FieldsPtr(_result, false);
}

inline enum LockTypeEnum Recordset15::GetLockType ( ) {
    enum LockTypeEnum _result;
    HRESULT _hr = get_LockType(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void Recordset15::PutLockType ( enum LockTypeEnum plLockType ) {
    HRESULT _hr = put_LockType(plLockType);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline ADO_LONGPTR Recordset15::GetMaxRecords ( ) {
    ADO_LONGPTR _result;
    HRESULT _hr = get_MaxRecords(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void Recordset15::PutMaxRecords ( ADO_LONGPTR plMaxRecords ) {
    HRESULT _hr = put_MaxRecords(plMaxRecords);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline ADO_LONGPTR Recordset15::GetRecordCount ( ) {
    ADO_LONGPTR _result;
    HRESULT _hr = get_RecordCount(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void Recordset15::PutRefSource ( IDispatch * pvSource ) {
    HRESULT _hr = putref_Source(pvSource);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline void Recordset15::PutSource ( _bstr_t pvSource ) {
    HRESULT _hr = put_Source(pvSource);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline _variant_t Recordset15::GetSource ( ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = get_Source(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _variant_t(_result, false);
}

inline HRESULT Recordset15::AddNew ( const _variant_t & FieldList, const _variant_t & Values ) {
    HRESULT _hr = raw_AddNew(FieldList, Values);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT Recordset15::CancelUpdate ( ) {
    HRESULT _hr = raw_CancelUpdate();
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT Recordset15::Close ( ) {
    HRESULT _hr = raw_Close();
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT Recordset15::Delete ( enum AffectEnum AffectRecords ) {
    HRESULT _hr = raw_Delete(AffectRecords);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline _variant_t Recordset15::GetRows ( long Rows, const _variant_t & Start, const _variant_t & Fields ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = raw_GetRows(Rows, Start, Fields, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _variant_t(_result, false);
}

inline HRESULT Recordset15::Move ( ADO_LONGPTR NumRecords, const _variant_t & Start ) {
    HRESULT _hr = raw_Move(NumRecords, Start);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT Recordset15::MoveNext ( ) {
    HRESULT _hr = raw_MoveNext();
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT Recordset15::MovePrevious ( ) {
    HRESULT _hr = raw_MovePrevious();
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT Recordset15::MoveFirst ( ) {
    HRESULT _hr = raw_MoveFirst();
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT Recordset15::MoveLast ( ) {
    HRESULT _hr = raw_MoveLast();
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT Recordset15::Open ( const _variant_t & Source, const _variant_t & ActiveConnection, enum CursorTypeEnum CursorType, enum LockTypeEnum LockType, long Options ) {
    HRESULT _hr = raw_Open(Source, ActiveConnection, CursorType, LockType, Options);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT Recordset15::Requery ( long Options ) {
    HRESULT _hr = raw_Requery(Options);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT Recordset15::_xResync ( enum AffectEnum AffectRecords ) {
    HRESULT _hr = raw__xResync(AffectRecords);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT Recordset15::Update ( const _variant_t & Fields, const _variant_t & Values ) {
    HRESULT _hr = raw_Update(Fields, Values);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline PositionEnum_Param Recordset15::GetAbsolutePage ( ) {
    PositionEnum_Param _result;
    HRESULT _hr = get_AbsolutePage(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void Recordset15::PutAbsolutePage ( PositionEnum_Param pl ) {
    HRESULT _hr = put_AbsolutePage(pl);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline enum EditModeEnum Recordset15::GetEditMode ( ) {
    enum EditModeEnum _result;
    HRESULT _hr = get_EditMode(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline _variant_t Recordset15::GetFilter ( ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = get_Filter(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _variant_t(_result, false);
}

inline void Recordset15::PutFilter ( const _variant_t & Criteria ) {
    HRESULT _hr = put_Filter(Criteria);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline ADO_LONGPTR Recordset15::GetPageCount ( ) {
    ADO_LONGPTR _result;
    HRESULT _hr = get_PageCount(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline long Recordset15::GetPageSize ( ) {
    long _result;
    HRESULT _hr = get_PageSize(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void Recordset15::PutPageSize ( long pl ) {
    HRESULT _hr = put_PageSize(pl);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline _bstr_t Recordset15::GetSort ( ) {
    BSTR _result;
    HRESULT _hr = get_Sort(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline void Recordset15::PutSort ( _bstr_t Criteria ) {
    HRESULT _hr = put_Sort(Criteria);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline long Recordset15::GetStatus ( ) {
    long _result;
    HRESULT _hr = get_Status(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline long Recordset15::GetState ( ) {
    long _result;
    HRESULT _hr = get_State(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline _RecordsetPtr Recordset15::_xClone ( ) {
    struct _Recordset * _result;
    HRESULT _hr = raw__xClone(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _RecordsetPtr(_result, false);
}

inline HRESULT Recordset15::UpdateBatch ( enum AffectEnum AffectRecords ) {
    HRESULT _hr = raw_UpdateBatch(AffectRecords);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT Recordset15::CancelBatch ( enum AffectEnum AffectRecords ) {
    HRESULT _hr = raw_CancelBatch(AffectRecords);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline enum CursorLocationEnum Recordset15::GetCursorLocation ( ) {
    enum CursorLocationEnum _result;
    HRESULT _hr = get_CursorLocation(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void Recordset15::PutCursorLocation ( enum CursorLocationEnum plCursorLoc ) {
    HRESULT _hr = put_CursorLocation(plCursorLoc);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline _RecordsetPtr Recordset15::NextRecordset ( VARIANT * RecordsAffected ) {
    struct _Recordset * _result;
    HRESULT _hr = raw_NextRecordset(RecordsAffected, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _RecordsetPtr(_result, false);
}

inline VARIANT_BOOL Recordset15::Supports ( enum CursorOptionEnum CursorOptions ) {
    VARIANT_BOOL _result;
    HRESULT _hr = raw_Supports(CursorOptions, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline _variant_t Recordset15::GetCollect ( const _variant_t & Index ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = get_Collect(Index, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _variant_t(_result, false);
}

inline void Recordset15::PutCollect ( const _variant_t & Index, const _variant_t & pvar ) {
    HRESULT _hr = put_Collect(Index, pvar);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline enum MarshalOptionsEnum Recordset15::GetMarshalOptions ( ) {
    enum MarshalOptionsEnum _result;
    HRESULT _hr = get_MarshalOptions(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void Recordset15::PutMarshalOptions ( enum MarshalOptionsEnum peMarshal ) {
    HRESULT _hr = put_MarshalOptions(peMarshal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline HRESULT Recordset15::Find ( _bstr_t Criteria, ADO_LONGPTR SkipRecords, enum SearchDirectionEnum SearchDirection, const _variant_t & Start ) {
    HRESULT _hr = raw_Find(Criteria, SkipRecords, SearchDirection, Start);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

//
// interface Recordset20 wrapper method implementations
//

inline HRESULT Recordset20::Cancel ( ) {
    HRESULT _hr = raw_Cancel();
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline IUnknownPtr Recordset20::GetDataSource ( ) {
    IUnknown * _result;
    HRESULT _hr = get_DataSource(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IUnknownPtr(_result, false);
}

inline void Recordset20::PutRefDataSource ( IUnknown * ppunkDataSource ) {
    HRESULT _hr = putref_DataSource(ppunkDataSource);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline HRESULT Recordset20::_xSave ( _bstr_t FileName, enum PersistFormatEnum PersistFormat ) {
    HRESULT _hr = raw__xSave(FileName, PersistFormat);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline IDispatchPtr Recordset20::GetActiveCommand ( ) {
    IDispatch * _result;
    HRESULT _hr = get_ActiveCommand(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IDispatchPtr(_result, false);
}

inline void Recordset20::PutStayInSync ( VARIANT_BOOL pbStayInSync ) {
    HRESULT _hr = put_StayInSync(pbStayInSync);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline VARIANT_BOOL Recordset20::GetStayInSync ( ) {
    VARIANT_BOOL _result;
    HRESULT _hr = get_StayInSync(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline _bstr_t Recordset20::GetString ( enum StringFormatEnum StringFormat, long NumRows, _bstr_t ColumnDelimeter, _bstr_t RowDelimeter, _bstr_t NullExpr ) {
    BSTR _result;
    HRESULT _hr = raw_GetString(StringFormat, NumRows, ColumnDelimeter, RowDelimeter, NullExpr, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline _bstr_t Recordset20::GetDataMember ( ) {
    BSTR _result;
    HRESULT _hr = get_DataMember(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline void Recordset20::PutDataMember ( _bstr_t pbstrDataMember ) {
    HRESULT _hr = put_DataMember(pbstrDataMember);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline enum CompareEnum Recordset20::CompareBookmarks ( const _variant_t & Bookmark1, const _variant_t & Bookmark2 ) {
    enum CompareEnum _result;
    HRESULT _hr = raw_CompareBookmarks(Bookmark1, Bookmark2, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline _RecordsetPtr Recordset20::Clone ( enum LockTypeEnum LockType ) {
    struct _Recordset * _result;
    HRESULT _hr = raw_Clone(LockType, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _RecordsetPtr(_result, false);
}

inline HRESULT Recordset20::Resync ( enum AffectEnum AffectRecords, enum ResyncEnum ResyncValues ) {
    HRESULT _hr = raw_Resync(AffectRecords, ResyncValues);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

//
// interface Recordset21 wrapper method implementations
//

inline HRESULT Recordset21::Seek ( const _variant_t & KeyValues, enum SeekEnum SeekOption ) {
    HRESULT _hr = raw_Seek(KeyValues, SeekOption);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline void Recordset21::PutIndex ( _bstr_t pbstrIndex ) {
    HRESULT _hr = put_Index(pbstrIndex);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline _bstr_t Recordset21::GetIndex ( ) {
    BSTR _result;
    HRESULT _hr = get_Index(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

//
// interface _Recordset wrapper method implementations
//

inline HRESULT _Recordset::Save ( const _variant_t & Destination, enum PersistFormatEnum PersistFormat ) {
    HRESULT _hr = raw_Save(Destination, PersistFormat);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

//
// interface Fields15 wrapper method implementations
//

inline FieldPtr Fields15::GetItem ( const _variant_t & Index ) {
    struct Field * _result;
    HRESULT _hr = get_Item(Index, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return FieldPtr(_result, false);
}

//
// interface Fields20 wrapper method implementations
//

inline HRESULT Fields20::_Append ( _bstr_t Name, enum DataTypeEnum Type, ADO_LONGPTR DefinedSize, enum FieldAttributeEnum Attrib ) {
    HRESULT _hr = raw__Append(Name, Type, DefinedSize, Attrib);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT Fields20::Delete ( const _variant_t & Index ) {
    HRESULT _hr = raw_Delete(Index);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

//
// interface Fields wrapper method implementations
//

inline HRESULT Fields::Append ( _bstr_t Name, enum DataTypeEnum Type, ADO_LONGPTR DefinedSize, enum FieldAttributeEnum Attrib, const _variant_t & FieldValue ) {
    HRESULT _hr = raw_Append(Name, Type, DefinedSize, Attrib, FieldValue);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT Fields::Update ( ) {
    HRESULT _hr = raw_Update();
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT Fields::Resync ( enum ResyncEnum ResyncValues ) {
    HRESULT _hr = raw_Resync(ResyncValues);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT Fields::CancelUpdate ( ) {
    HRESULT _hr = raw_CancelUpdate();
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

//
// interface Field20 wrapper method implementations
//

inline ADO_LONGPTR Field20::GetActualSize ( ) {
    ADO_LONGPTR _result;
    HRESULT _hr = get_ActualSize(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline long Field20::GetAttributes ( ) {
    long _result;
    HRESULT _hr = get_Attributes(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline ADO_LONGPTR Field20::GetDefinedSize ( ) {
    ADO_LONGPTR _result;
    HRESULT _hr = get_DefinedSize(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline _bstr_t Field20::GetName ( ) {
    BSTR _result;
    HRESULT _hr = get_Name(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline enum DataTypeEnum Field20::GetType ( ) {
    enum DataTypeEnum _result;
    HRESULT _hr = get_Type(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline _variant_t Field20::GetValue ( ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = get_Value(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _variant_t(_result, false);
}

inline void Field20::PutValue ( const _variant_t & pvar ) {
    HRESULT _hr = put_Value(pvar);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline unsigned char Field20::GetPrecision ( ) {
    unsigned char _result;
    HRESULT _hr = get_Precision(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline unsigned char Field20::GetNumericScale ( ) {
    unsigned char _result;
    HRESULT _hr = get_NumericScale(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline HRESULT Field20::AppendChunk ( const _variant_t & Data ) {
    HRESULT _hr = raw_AppendChunk(Data);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline _variant_t Field20::GetChunk ( long Length ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = raw_GetChunk(Length, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _variant_t(_result, false);
}

inline _variant_t Field20::GetOriginalValue ( ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = get_OriginalValue(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _variant_t(_result, false);
}

inline _variant_t Field20::GetUnderlyingValue ( ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = get_UnderlyingValue(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _variant_t(_result, false);
}

inline IUnknownPtr Field20::GetDataFormat ( ) {
    IUnknown * _result;
    HRESULT _hr = get_DataFormat(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IUnknownPtr(_result, false);
}

inline void Field20::PutRefDataFormat ( IUnknown * ppiDF ) {
    HRESULT _hr = putref_DataFormat(ppiDF);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline void Field20::PutPrecision ( unsigned char pbPrecision ) {
    HRESULT _hr = put_Precision(pbPrecision);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline void Field20::PutNumericScale ( unsigned char pbNumericScale ) {
    HRESULT _hr = put_NumericScale(pbNumericScale);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline void Field20::PutType ( enum DataTypeEnum pDataType ) {
    HRESULT _hr = put_Type(pDataType);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline void Field20::PutDefinedSize ( ADO_LONGPTR pl ) {
    HRESULT _hr = put_DefinedSize(pl);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline void Field20::PutAttributes ( long pl ) {
    HRESULT _hr = put_Attributes(pl);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

//
// interface Field wrapper method implementations
//

inline long Field::GetStatus ( ) {
    long _result;
    HRESULT _hr = get_Status(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

//
// interface _Parameter wrapper method implementations
//

inline _bstr_t _Parameter::GetName ( ) {
    BSTR _result;
    HRESULT _hr = get_Name(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline void _Parameter::PutName ( _bstr_t pbstr ) {
    HRESULT _hr = put_Name(pbstr);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline _variant_t _Parameter::GetValue ( ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = get_Value(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _variant_t(_result, false);
}

inline void _Parameter::PutValue ( const _variant_t & pvar ) {
    HRESULT _hr = put_Value(pvar);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline enum DataTypeEnum _Parameter::GetType ( ) {
    enum DataTypeEnum _result;
    HRESULT _hr = get_Type(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void _Parameter::PutType ( enum DataTypeEnum psDataType ) {
    HRESULT _hr = put_Type(psDataType);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline void _Parameter::PutDirection ( enum ParameterDirectionEnum plParmDirection ) {
    HRESULT _hr = put_Direction(plParmDirection);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline enum ParameterDirectionEnum _Parameter::GetDirection ( ) {
    enum ParameterDirectionEnum _result;
    HRESULT _hr = get_Direction(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void _Parameter::PutPrecision ( unsigned char pbPrecision ) {
    HRESULT _hr = put_Precision(pbPrecision);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline unsigned char _Parameter::GetPrecision ( ) {
    unsigned char _result;
    HRESULT _hr = get_Precision(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void _Parameter::PutNumericScale ( unsigned char pbScale ) {
    HRESULT _hr = put_NumericScale(pbScale);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline unsigned char _Parameter::GetNumericScale ( ) {
    unsigned char _result;
    HRESULT _hr = get_NumericScale(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void _Parameter::PutSize ( ADO_LONGPTR pl ) {
    HRESULT _hr = put_Size(pl);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline ADO_LONGPTR _Parameter::GetSize ( ) {
    ADO_LONGPTR _result;
    HRESULT _hr = get_Size(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline HRESULT _Parameter::AppendChunk ( const _variant_t & Val ) {
    HRESULT _hr = raw_AppendChunk(Val);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline long _Parameter::GetAttributes ( ) {
    long _result;
    HRESULT _hr = get_Attributes(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void _Parameter::PutAttributes ( long plParmAttribs ) {
    HRESULT _hr = put_Attributes(plParmAttribs);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

//
// interface Parameters wrapper method implementations
//

inline _ParameterPtr Parameters::GetItem ( const _variant_t & Index ) {
    struct _Parameter * _result;
    HRESULT _hr = get_Item(Index, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _ParameterPtr(_result, false);
}

//
// interface Command25 wrapper method implementations
//

inline long Command25::GetState ( ) {
    long _result;
    HRESULT _hr = get_State(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline HRESULT Command25::Cancel ( ) {
    HRESULT _hr = raw_Cancel();
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

//
// interface _Command wrapper method implementations
//

inline void _Command::PutRefCommandStream ( IUnknown * pvStream ) {
    HRESULT _hr = putref_CommandStream(pvStream);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline _variant_t _Command::GetCommandStream ( ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = get_CommandStream(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _variant_t(_result, false);
}

inline void _Command::PutDialect ( _bstr_t pbstrDialect ) {
    HRESULT _hr = put_Dialect(pbstrDialect);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline _bstr_t _Command::GetDialect ( ) {
    BSTR _result;
    HRESULT _hr = get_Dialect(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline void _Command::PutNamedParameters ( VARIANT_BOOL pfNamedParameters ) {
    HRESULT _hr = put_NamedParameters(pfNamedParameters);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline VARIANT_BOOL _Command::GetNamedParameters ( ) {
    VARIANT_BOOL _result;
    HRESULT _hr = get_NamedParameters(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

//
// interface ConnectionEventsVt wrapper method implementations
//

inline HRESULT ConnectionEventsVt::InfoMessage ( struct Error * pError, enum EventStatusEnum * adStatus, struct _Connection * pConnection ) {
    HRESULT _hr = raw_InfoMessage(pError, adStatus, pConnection);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT ConnectionEventsVt::BeginTransComplete ( long TransactionLevel, struct Error * pError, enum EventStatusEnum * adStatus, struct _Connection * pConnection ) {
    HRESULT _hr = raw_BeginTransComplete(TransactionLevel, pError, adStatus, pConnection);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT ConnectionEventsVt::CommitTransComplete ( struct Error * pError, enum EventStatusEnum * adStatus, struct _Connection * pConnection ) {
    HRESULT _hr = raw_CommitTransComplete(pError, adStatus, pConnection);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT ConnectionEventsVt::RollbackTransComplete ( struct Error * pError, enum EventStatusEnum * adStatus, struct _Connection * pConnection ) {
    HRESULT _hr = raw_RollbackTransComplete(pError, adStatus, pConnection);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT ConnectionEventsVt::WillExecute ( BSTR * Source, enum CursorTypeEnum * CursorType, enum LockTypeEnum * LockType, long * Options, enum EventStatusEnum * adStatus, struct _Command * pCommand, struct _Recordset * pRecordset, struct _Connection * pConnection ) {
    HRESULT _hr = raw_WillExecute(Source, CursorType, LockType, Options, adStatus, pCommand, pRecordset, pConnection);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT ConnectionEventsVt::ExecuteComplete ( long RecordsAffected, struct Error * pError, enum EventStatusEnum * adStatus, struct _Command * pCommand, struct _Recordset * pRecordset, struct _Connection * pConnection ) {
    HRESULT _hr = raw_ExecuteComplete(RecordsAffected, pError, adStatus, pCommand, pRecordset, pConnection);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT ConnectionEventsVt::WillConnect ( BSTR * ConnectionString, BSTR * UserID, BSTR * Password, long * Options, enum EventStatusEnum * adStatus, struct _Connection * pConnection ) {
    HRESULT _hr = raw_WillConnect(ConnectionString, UserID, Password, Options, adStatus, pConnection);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT ConnectionEventsVt::ConnectComplete ( struct Error * pError, enum EventStatusEnum * adStatus, struct _Connection * pConnection ) {
    HRESULT _hr = raw_ConnectComplete(pError, adStatus, pConnection);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT ConnectionEventsVt::Disconnect ( enum EventStatusEnum * adStatus, struct _Connection * pConnection ) {
    HRESULT _hr = raw_Disconnect(adStatus, pConnection);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

//
// interface RecordsetEventsVt wrapper method implementations
//

inline HRESULT RecordsetEventsVt::WillChangeField ( long cFields, const _variant_t & Fields, enum EventStatusEnum * adStatus, struct _Recordset * pRecordset ) {
    HRESULT _hr = raw_WillChangeField(cFields, Fields, adStatus, pRecordset);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT RecordsetEventsVt::FieldChangeComplete ( long cFields, const _variant_t & Fields, struct Error * pError, enum EventStatusEnum * adStatus, struct _Recordset * pRecordset ) {
    HRESULT _hr = raw_FieldChangeComplete(cFields, Fields, pError, adStatus, pRecordset);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT RecordsetEventsVt::WillChangeRecord ( enum EventReasonEnum adReason, long cRecords, enum EventStatusEnum * adStatus, struct _Recordset * pRecordset ) {
    HRESULT _hr = raw_WillChangeRecord(adReason, cRecords, adStatus, pRecordset);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT RecordsetEventsVt::RecordChangeComplete ( enum EventReasonEnum adReason, long cRecords, struct Error * pError, enum EventStatusEnum * adStatus, struct _Recordset * pRecordset ) {
    HRESULT _hr = raw_RecordChangeComplete(adReason, cRecords, pError, adStatus, pRecordset);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT RecordsetEventsVt::WillChangeRecordset ( enum EventReasonEnum adReason, enum EventStatusEnum * adStatus, struct _Recordset * pRecordset ) {
    HRESULT _hr = raw_WillChangeRecordset(adReason, adStatus, pRecordset);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT RecordsetEventsVt::RecordsetChangeComplete ( enum EventReasonEnum adReason, struct Error * pError, enum EventStatusEnum * adStatus, struct _Recordset * pRecordset ) {
    HRESULT _hr = raw_RecordsetChangeComplete(adReason, pError, adStatus, pRecordset);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT RecordsetEventsVt::WillMove ( enum EventReasonEnum adReason, enum EventStatusEnum * adStatus, struct _Recordset * pRecordset ) {
    HRESULT _hr = raw_WillMove(adReason, adStatus, pRecordset);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT RecordsetEventsVt::MoveComplete ( enum EventReasonEnum adReason, struct Error * pError, enum EventStatusEnum * adStatus, struct _Recordset * pRecordset ) {
    HRESULT _hr = raw_MoveComplete(adReason, pError, adStatus, pRecordset);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT RecordsetEventsVt::EndOfRecordset ( VARIANT_BOOL * fMoreData, enum EventStatusEnum * adStatus, struct _Recordset * pRecordset ) {
    HRESULT _hr = raw_EndOfRecordset(fMoreData, adStatus, pRecordset);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT RecordsetEventsVt::FetchProgress ( long Progress, long MaxProgress, enum EventStatusEnum * adStatus, struct _Recordset * pRecordset ) {
    HRESULT _hr = raw_FetchProgress(Progress, MaxProgress, adStatus, pRecordset);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT RecordsetEventsVt::FetchComplete ( struct Error * pError, enum EventStatusEnum * adStatus, struct _Recordset * pRecordset ) {
    HRESULT _hr = raw_FetchComplete(pError, adStatus, pRecordset);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

//
// dispinterface ConnectionEvents wrapper method implementations
//

inline HRESULT ConnectionEvents::InfoMessage ( struct Error * pError, enum EventStatusEnum * adStatus, struct _Connection * pConnection ) {
    HRESULT _result;
    _com_dispatch_method(this, 0x0, DISPATCH_METHOD, VT_ERROR, (void*)&_result, 
        L"\x0009\x4003\x0009", pError, adStatus, pConnection);
    return _result;
}

inline HRESULT ConnectionEvents::BeginTransComplete ( long TransactionLevel, struct Error * pError, enum EventStatusEnum * adStatus, struct _Connection * pConnection ) {
    HRESULT _result;
    _com_dispatch_method(this, 0x1, DISPATCH_METHOD, VT_ERROR, (void*)&_result, 
        L"\x0003\x0009\x4003\x0009", TransactionLevel, pError, adStatus, pConnection);
    return _result;
}

inline HRESULT ConnectionEvents::CommitTransComplete ( struct Error * pError, enum EventStatusEnum * adStatus, struct _Connection * pConnection ) {
    HRESULT _result;
    _com_dispatch_method(this, 0x3, DISPATCH_METHOD, VT_ERROR, (void*)&_result, 
        L"\x0009\x4003\x0009", pError, adStatus, pConnection);
    return _result;
}

inline HRESULT ConnectionEvents::RollbackTransComplete ( struct Error * pError, enum EventStatusEnum * adStatus, struct _Connection * pConnection ) {
    HRESULT _result;
    _com_dispatch_method(this, 0x2, DISPATCH_METHOD, VT_ERROR, (void*)&_result, 
        L"\x0009\x4003\x0009", pError, adStatus, pConnection);
    return _result;
}

inline HRESULT ConnectionEvents::WillExecute ( BSTR * Source, enum CursorTypeEnum * CursorType, enum LockTypeEnum * LockType, long * Options, enum EventStatusEnum * adStatus, struct _Command * pCommand, struct _Recordset * pRecordset, struct _Connection * pConnection ) {
    HRESULT _result;
    _com_dispatch_method(this, 0x4, DISPATCH_METHOD, VT_ERROR, (void*)&_result, 
        L"\x4008\x4003\x4003\x4003\x4003\x0009\x0009\x0009", Source, CursorType, LockType, Options, adStatus, pCommand, pRecordset, pConnection);
    return _result;
}

inline HRESULT ConnectionEvents::ExecuteComplete ( long RecordsAffected, struct Error * pError, enum EventStatusEnum * adStatus, struct _Command * pCommand, struct _Recordset * pRecordset, struct _Connection * pConnection ) {
    HRESULT _result;
    _com_dispatch_method(this, 0x5, DISPATCH_METHOD, VT_ERROR, (void*)&_result, 
        L"\x0003\x0009\x4003\x0009\x0009\x0009", RecordsAffected, pError, adStatus, pCommand, pRecordset, pConnection);
    return _result;
}

inline HRESULT ConnectionEvents::WillConnect ( BSTR * ConnectionString, BSTR * UserID, BSTR * Password, long * Options, enum EventStatusEnum * adStatus, struct _Connection * pConnection ) {
    HRESULT _result;
    _com_dispatch_method(this, 0x6, DISPATCH_METHOD, VT_ERROR, (void*)&_result, 
        L"\x4008\x4008\x4008\x4003\x4003\x0009", ConnectionString, UserID, Password, Options, adStatus, pConnection);
    return _result;
}

inline HRESULT ConnectionEvents::ConnectComplete ( struct Error * pError, enum EventStatusEnum * adStatus, struct _Connection * pConnection ) {
    HRESULT _result;
    _com_dispatch_method(this, 0x7, DISPATCH_METHOD, VT_ERROR, (void*)&_result, 
        L"\x0009\x4003\x0009", pError, adStatus, pConnection);
    return _result;
}

inline HRESULT ConnectionEvents::Disconnect ( enum EventStatusEnum * adStatus, struct _Connection * pConnection ) {
    HRESULT _result;
    _com_dispatch_method(this, 0x8, DISPATCH_METHOD, VT_ERROR, (void*)&_result, 
        L"\x4003\x0009", adStatus, pConnection);
    return _result;
}

//
// dispinterface RecordsetEvents wrapper method implementations
//

inline HRESULT RecordsetEvents::WillChangeField ( long cFields, const _variant_t & Fields, enum EventStatusEnum * adStatus, struct _Recordset * pRecordset ) {
    HRESULT _result;
    _com_dispatch_method(this, 0x9, DISPATCH_METHOD, VT_ERROR, (void*)&_result, 
        L"\x0003\x000c\x4003\x0009", cFields, &Fields, adStatus, pRecordset);
    return _result;
}

inline HRESULT RecordsetEvents::FieldChangeComplete ( long cFields, const _variant_t & Fields, struct Error * pError, enum EventStatusEnum * adStatus, struct _Recordset * pRecordset ) {
    HRESULT _result;
    _com_dispatch_method(this, 0xa, DISPATCH_METHOD, VT_ERROR, (void*)&_result, 
        L"\x0003\x000c\x0009\x4003\x0009", cFields, &Fields, pError, adStatus, pRecordset);
    return _result;
}

inline HRESULT RecordsetEvents::WillChangeRecord ( enum EventReasonEnum adReason, long cRecords, enum EventStatusEnum * adStatus, struct _Recordset * pRecordset ) {
    HRESULT _result;
    _com_dispatch_method(this, 0xb, DISPATCH_METHOD, VT_ERROR, (void*)&_result, 
        L"\x0003\x0003\x4003\x0009", adReason, cRecords, adStatus, pRecordset);
    return _result;
}

inline HRESULT RecordsetEvents::RecordChangeComplete ( enum EventReasonEnum adReason, long cRecords, struct Error * pError, enum EventStatusEnum * adStatus, struct _Recordset * pRecordset ) {
    HRESULT _result;
    _com_dispatch_method(this, 0xc, DISPATCH_METHOD, VT_ERROR, (void*)&_result, 
        L"\x0003\x0003\x0009\x4003\x0009", adReason, cRecords, pError, adStatus, pRecordset);
    return _result;
}

inline HRESULT RecordsetEvents::WillChangeRecordset ( enum EventReasonEnum adReason, enum EventStatusEnum * adStatus, struct _Recordset * pRecordset ) {
    HRESULT _result;
    _com_dispatch_method(this, 0xd, DISPATCH_METHOD, VT_ERROR, (void*)&_result, 
        L"\x0003\x4003\x0009", adReason, adStatus, pRecordset);
    return _result;
}

inline HRESULT RecordsetEvents::RecordsetChangeComplete ( enum EventReasonEnum adReason, struct Error * pError, enum EventStatusEnum * adStatus, struct _Recordset * pRecordset ) {
    HRESULT _result;
    _com_dispatch_method(this, 0xe, DISPATCH_METHOD, VT_ERROR, (void*)&_result, 
        L"\x0003\x0009\x4003\x0009", adReason, pError, adStatus, pRecordset);
    return _result;
}

inline HRESULT RecordsetEvents::WillMove ( enum EventReasonEnum adReason, enum EventStatusEnum * adStatus, struct _Recordset * pRecordset ) {
    HRESULT _result;
    _com_dispatch_method(this, 0xf, DISPATCH_METHOD, VT_ERROR, (void*)&_result, 
        L"\x0003\x4003\x0009", adReason, adStatus, pRecordset);
    return _result;
}

inline HRESULT RecordsetEvents::MoveComplete ( enum EventReasonEnum adReason, struct Error * pError, enum EventStatusEnum * adStatus, struct _Recordset * pRecordset ) {
    HRESULT _result;
    _com_dispatch_method(this, 0x10, DISPATCH_METHOD, VT_ERROR, (void*)&_result, 
        L"\x0003\x0009\x4003\x0009", adReason, pError, adStatus, pRecordset);
    return _result;
}

inline HRESULT RecordsetEvents::EndOfRecordset ( VARIANT_BOOL * fMoreData, enum EventStatusEnum * adStatus, struct _Recordset * pRecordset ) {
    HRESULT _result;
    _com_dispatch_method(this, 0x11, DISPATCH_METHOD, VT_ERROR, (void*)&_result, 
        L"\x400b\x4003\x0009", fMoreData, adStatus, pRecordset);
    return _result;
}

inline HRESULT RecordsetEvents::FetchProgress ( long Progress, long MaxProgress, enum EventStatusEnum * adStatus, struct _Recordset * pRecordset ) {
    HRESULT _result;
    _com_dispatch_method(this, 0x12, DISPATCH_METHOD, VT_ERROR, (void*)&_result, 
        L"\x0003\x0003\x4003\x0009", Progress, MaxProgress, adStatus, pRecordset);
    return _result;
}

inline HRESULT RecordsetEvents::FetchComplete ( struct Error * pError, enum EventStatusEnum * adStatus, struct _Recordset * pRecordset ) {
    HRESULT _result;
    _com_dispatch_method(this, 0x13, DISPATCH_METHOD, VT_ERROR, (void*)&_result, 
        L"\x0009\x4003\x0009", pError, adStatus, pRecordset);
    return _result;
}

//
// interface ADOConnectionConstruction15 wrapper method implementations
//

inline IUnknownPtr ADOConnectionConstruction15::GetDSO ( ) {
    IUnknown * _result;
    HRESULT _hr = get_DSO(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IUnknownPtr(_result, false);
}

inline IUnknownPtr ADOConnectionConstruction15::GetSession ( ) {
    IUnknown * _result;
    HRESULT _hr = get_Session(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IUnknownPtr(_result, false);
}

inline HRESULT ADOConnectionConstruction15::WrapDSOandSession ( IUnknown * pDSO, IUnknown * pSession ) {
    HRESULT _hr = raw_WrapDSOandSession(pDSO, pSession);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

//
// interface _Record wrapper method implementations
//

inline _variant_t _Record::GetActiveConnection ( ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = get_ActiveConnection(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _variant_t(_result, false);
}

inline void _Record::PutActiveConnection ( _bstr_t pvar ) {
    HRESULT _hr = put_ActiveConnection(pvar);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline void _Record::PutRefActiveConnection ( struct _Connection * pvar ) {
    HRESULT _hr = putref_ActiveConnection(pvar);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline enum ObjectStateEnum _Record::GetState ( ) {
    enum ObjectStateEnum _result;
    HRESULT _hr = get_State(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline _variant_t _Record::GetSource ( ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = get_Source(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _variant_t(_result, false);
}

inline void _Record::PutSource ( _bstr_t pvar ) {
    HRESULT _hr = put_Source(pvar);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline void _Record::PutRefSource ( IDispatch * pvar ) {
    HRESULT _hr = putref_Source(pvar);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline enum ConnectModeEnum _Record::GetMode ( ) {
    enum ConnectModeEnum _result;
    HRESULT _hr = get_Mode(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void _Record::PutMode ( enum ConnectModeEnum pMode ) {
    HRESULT _hr = put_Mode(pMode);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline _bstr_t _Record::GetParentURL ( ) {
    BSTR _result;
    HRESULT _hr = get_ParentURL(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline _bstr_t _Record::MoveRecord ( _bstr_t Source, _bstr_t Destination, _bstr_t UserName, _bstr_t Password, enum MoveRecordOptionsEnum Options, VARIANT_BOOL Async ) {
    BSTR _result;
    HRESULT _hr = raw_MoveRecord(Source, Destination, UserName, Password, Options, Async, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline _bstr_t _Record::CopyRecord ( _bstr_t Source, _bstr_t Destination, _bstr_t UserName, _bstr_t Password, enum CopyRecordOptionsEnum Options, VARIANT_BOOL Async ) {
    BSTR _result;
    HRESULT _hr = raw_CopyRecord(Source, Destination, UserName, Password, Options, Async, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline HRESULT _Record::DeleteRecord ( _bstr_t Source, VARIANT_BOOL Async ) {
    HRESULT _hr = raw_DeleteRecord(Source, Async);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT _Record::Open ( const _variant_t & Source, const _variant_t & ActiveConnection, enum ConnectModeEnum Mode, enum RecordCreateOptionsEnum CreateOptions, enum RecordOpenOptionsEnum Options, _bstr_t UserName, _bstr_t Password ) {
    HRESULT _hr = raw_Open(Source, ActiveConnection, Mode, CreateOptions, Options, UserName, Password);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT _Record::Close ( ) {
    HRESULT _hr = raw_Close();
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline FieldsPtr _Record::GetFields ( ) {
    struct Fields * _result;
    HRESULT _hr = get_Fields(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return FieldsPtr(_result, false);
}

inline enum RecordTypeEnum _Record::GetRecordType ( ) {
    enum RecordTypeEnum _result;
    HRESULT _hr = get_RecordType(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline _RecordsetPtr _Record::GetChildren ( ) {
    struct _Recordset * _result;
    HRESULT _hr = raw_GetChildren(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _RecordsetPtr(_result, false);
}

inline HRESULT _Record::Cancel ( ) {
    HRESULT _hr = raw_Cancel();
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

//
// interface _Stream wrapper method implementations
//

inline ADO_LONGPTR _Stream::GetSize ( ) {
    ADO_LONGPTR _result;
    HRESULT _hr = get_Size(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline VARIANT_BOOL _Stream::GetEOS ( ) {
    VARIANT_BOOL _result;
    HRESULT _hr = get_EOS(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline ADO_LONGPTR _Stream::GetPosition ( ) {
    ADO_LONGPTR _result;
    HRESULT _hr = get_Position(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void _Stream::PutPosition ( ADO_LONGPTR pPos ) {
    HRESULT _hr = put_Position(pPos);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline enum StreamTypeEnum _Stream::GetType ( ) {
    enum StreamTypeEnum _result;
    HRESULT _hr = get_Type(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void _Stream::PutType ( enum StreamTypeEnum ptype ) {
    HRESULT _hr = put_Type(ptype);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline enum LineSeparatorEnum _Stream::GetLineSeparator ( ) {
    enum LineSeparatorEnum _result;
    HRESULT _hr = get_LineSeparator(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void _Stream::PutLineSeparator ( enum LineSeparatorEnum pLS ) {
    HRESULT _hr = put_LineSeparator(pLS);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline enum ObjectStateEnum _Stream::GetState ( ) {
    enum ObjectStateEnum _result;
    HRESULT _hr = get_State(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline enum ConnectModeEnum _Stream::GetMode ( ) {
    enum ConnectModeEnum _result;
    HRESULT _hr = get_Mode(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void _Stream::PutMode ( enum ConnectModeEnum pMode ) {
    HRESULT _hr = put_Mode(pMode);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline _bstr_t _Stream::GetCharset ( ) {
    BSTR _result;
    HRESULT _hr = get_Charset(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline void _Stream::PutCharset ( _bstr_t pbstrCharset ) {
    HRESULT _hr = put_Charset(pbstrCharset);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline _variant_t _Stream::Read ( long NumBytes ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = raw_Read(NumBytes, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _variant_t(_result, false);
}

inline HRESULT _Stream::Open ( const _variant_t & Source, enum ConnectModeEnum Mode, enum StreamOpenOptionsEnum Options, _bstr_t UserName, _bstr_t Password ) {
    HRESULT _hr = raw_Open(Source, Mode, Options, UserName, Password);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT _Stream::Close ( ) {
    HRESULT _hr = raw_Close();
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT _Stream::SkipLine ( ) {
    HRESULT _hr = raw_SkipLine();
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT _Stream::Write ( const _variant_t & Buffer ) {
    HRESULT _hr = raw_Write(Buffer);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT _Stream::SetEOS ( ) {
    HRESULT _hr = raw_SetEOS();
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT _Stream::CopyTo ( struct _Stream * DestStream, ADO_LONGPTR CharNumber ) {
    HRESULT _hr = raw_CopyTo(DestStream, CharNumber);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT _Stream::Flush ( ) {
    HRESULT _hr = raw_Flush();
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT _Stream::SaveToFile ( _bstr_t FileName, enum SaveOptionsEnum Options ) {
    HRESULT _hr = raw_SaveToFile(FileName, Options);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT _Stream::LoadFromFile ( _bstr_t FileName ) {
    HRESULT _hr = raw_LoadFromFile(FileName);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline _bstr_t _Stream::ReadText ( long NumChars ) {
    BSTR _result;
    HRESULT _hr = raw_ReadText(NumChars, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline HRESULT _Stream::WriteText ( _bstr_t Data, enum StreamWriteEnum Options ) {
    HRESULT _hr = raw_WriteText(Data, Options);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT _Stream::Cancel ( ) {
    HRESULT _hr = raw_Cancel();
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

//
// interface ADORecordConstruction wrapper method implementations
//

inline IUnknownPtr ADORecordConstruction::GetRow ( ) {
    IUnknown * _result;
    HRESULT _hr = get_Row(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IUnknownPtr(_result, false);
}

inline void ADORecordConstruction::PutRow ( IUnknown * ppRow ) {
    HRESULT _hr = put_Row(ppRow);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline void ADORecordConstruction::PutParentRow ( IUnknown * _arg1 ) {
    HRESULT _hr = put_ParentRow(_arg1);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

//
// interface ADOStreamConstruction wrapper method implementations
//

inline IUnknownPtr ADOStreamConstruction::GetStream ( ) {
    IUnknown * _result;
    HRESULT _hr = get_Stream(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IUnknownPtr(_result, false);
}

inline void ADOStreamConstruction::PutStream ( IUnknown * ppStm ) {
    HRESULT _hr = put_Stream(ppStm);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

//
// interface ADOCommandConstruction wrapper method implementations
//

inline IUnknownPtr ADOCommandConstruction::GetOLEDBCommand ( ) {
    IUnknown * _result;
    HRESULT _hr = get_OLEDBCommand(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IUnknownPtr(_result, false);
}

inline void ADOCommandConstruction::PutOLEDBCommand ( IUnknown * ppOLEDBCommand ) {
    HRESULT _hr = put_OLEDBCommand(ppOLEDBCommand);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

//
// interface ADORecordsetConstruction wrapper method implementations
//

inline IUnknownPtr ADORecordsetConstruction::GetRowset ( ) {
    IUnknown * _result;
    HRESULT _hr = get_Rowset(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IUnknownPtr(_result, false);
}

inline void ADORecordsetConstruction::PutRowset ( IUnknown * ppRowset ) {
    HRESULT _hr = put_Rowset(ppRowset);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline ADO_LONGPTR ADORecordsetConstruction::GetChapter ( ) {
    ADO_LONGPTR _result;
    HRESULT _hr = get_Chapter(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void ADORecordsetConstruction::PutChapter ( ADO_LONGPTR plChapter ) {
    HRESULT _hr = put_Chapter(plChapter);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline IUnknownPtr ADORecordsetConstruction::GetRowPosition ( ) {
    IUnknown * _result;
    HRESULT _hr = get_RowPosition(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IUnknownPtr(_result, false);
}

inline void ADORecordsetConstruction::PutRowPosition ( IUnknown * ppRowPos ) {
    HRESULT _hr = put_RowPosition(ppRowPos);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

//
// interface Field15 wrapper method implementations
//

inline ADO_LONGPTR Field15::GetActualSize ( ) {
    ADO_LONGPTR _result;
    HRESULT _hr = get_ActualSize(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline long Field15::GetAttributes ( ) {
    long _result;
    HRESULT _hr = get_Attributes(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline ADO_LONGPTR Field15::GetDefinedSize ( ) {
    ADO_LONGPTR _result;
    HRESULT _hr = get_DefinedSize(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline _bstr_t Field15::GetName ( ) {
    BSTR _result;
    HRESULT _hr = get_Name(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline enum DataTypeEnum Field15::GetType ( ) {
    enum DataTypeEnum _result;
    HRESULT _hr = get_Type(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline _variant_t Field15::GetValue ( ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = get_Value(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _variant_t(_result, false);
}

inline void Field15::PutValue ( const _variant_t & pvar ) {
    HRESULT _hr = put_Value(pvar);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline unsigned char Field15::GetPrecision ( ) {
    unsigned char _result;
    HRESULT _hr = get_Precision(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline unsigned char Field15::GetNumericScale ( ) {
    unsigned char _result;
    HRESULT _hr = get_NumericScale(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline HRESULT Field15::AppendChunk ( const _variant_t & Data ) {
    HRESULT _hr = raw_AppendChunk(Data);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline _variant_t Field15::GetChunk ( long Length ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = raw_GetChunk(Length, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _variant_t(_result, false);
}

inline _variant_t Field15::GetOriginalValue ( ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = get_OriginalValue(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _variant_t(_result, false);
}

inline _variant_t Field15::GetUnderlyingValue ( ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = get_UnderlyingValue(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _variant_t(_result, false);
}

} // namespace XTPREPORTADODB


namespace XTPREPORTADOX {

//
// interface _Collection wrapper method implementations
//

inline long _Collection::GetCount ( ) {
    long _result;
    HRESULT _hr = get_Count(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline IUnknown * _Collection::_NewEnum ( ) {
    IUnknown * _result;
    HRESULT _hr = raw__NewEnum(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline HRESULT _Collection::Refresh ( ) {
    HRESULT _hr = raw_Refresh();
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

//
// interface _DynaCollection wrapper method implementations
//

inline HRESULT _DynaCollection::Append ( IDispatch * Object ) {
    HRESULT _hr = raw_Append(Object);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT _DynaCollection::Delete ( const _variant_t & Item ) {
    HRESULT _hr = raw_Delete(Item);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

//
// interface _Catalog wrapper method implementations
//

inline TablesPtr _Catalog::GetTables ( ) {
    struct Tables * _result;
    HRESULT _hr = get_Tables(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return TablesPtr(_result, false);
}

inline _variant_t _Catalog::GetActiveConnection ( ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = get_ActiveConnection(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _variant_t(_result, false);
}

inline void _Catalog::PutActiveConnection ( const _variant_t & pVal ) {
    HRESULT _hr = put_ActiveConnection(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline void _Catalog::PutRefActiveConnection ( IDispatch * pVal ) {
    HRESULT _hr = putref_ActiveConnection(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline ProceduresPtr _Catalog::GetProcedures ( ) {
    struct Procedures * _result;
    HRESULT _hr = get_Procedures(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return ProceduresPtr(_result, false);
}

inline ViewsPtr _Catalog::GetViews ( ) {
    struct Views * _result;
    HRESULT _hr = get_Views(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return ViewsPtr(_result, false);
}

inline GroupsPtr _Catalog::GetGroups ( ) {
    struct Groups * _result;
    HRESULT _hr = get_Groups(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return GroupsPtr(_result, false);
}

inline UsersPtr _Catalog::GetUsers ( ) {
    struct Users * _result;
    HRESULT _hr = get_Users(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return UsersPtr(_result, false);
}

inline _variant_t _Catalog::Create ( _bstr_t ConnectString ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = raw_Create(ConnectString, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _variant_t(_result, false);
}

inline _bstr_t _Catalog::GetObjectOwner ( _bstr_t ObjectName, enum ObjectTypeEnum ObjectType, const _variant_t & ObjectTypeId ) {
    BSTR _result;
    HRESULT _hr = raw_GetObjectOwner(ObjectName, ObjectType, ObjectTypeId, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline HRESULT _Catalog::SetObjectOwner ( _bstr_t ObjectName, enum ObjectTypeEnum ObjectType, _bstr_t UserName, const _variant_t & ObjectTypeId ) {
    HRESULT _hr = raw_SetObjectOwner(ObjectName, ObjectType, UserName, ObjectTypeId);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

//
// interface Tables wrapper method implementations
//

inline _TablePtr Tables::GetItem ( const _variant_t & Item ) {
    struct _Table * _result;
    HRESULT _hr = get_Item(Item, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _TablePtr(_result, false);
}

inline HRESULT Tables::Append ( const _variant_t & Item ) {
    HRESULT _hr = raw_Append(Item);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT Tables::Delete ( const _variant_t & Item ) {
    HRESULT _hr = raw_Delete(Item);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

//
// interface _Table wrapper method implementations
//

inline ColumnsPtr _Table::GetColumns ( ) {
    struct Columns * _result;
    HRESULT _hr = get_Columns(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return ColumnsPtr(_result, false);
}

inline _bstr_t _Table::GetName ( ) {
    BSTR _result;
    HRESULT _hr = get_Name(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline void _Table::PutName ( _bstr_t pVal ) {
    HRESULT _hr = put_Name(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline _bstr_t _Table::GetType ( ) {
    BSTR _result;
    HRESULT _hr = get_Type(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline IndexesPtr _Table::GetIndexes ( ) {
    struct Indexes * _result;
    HRESULT _hr = get_Indexes(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IndexesPtr(_result, false);
}

inline KeysPtr _Table::GetKeys ( ) {
    struct Keys * _result;
    HRESULT _hr = get_Keys(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return KeysPtr(_result, false);
}

inline PropertiesPtr _Table::GetProperties ( ) {
    struct Properties * _result;
    HRESULT _hr = get_Properties(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return PropertiesPtr(_result, false);
}

inline _variant_t _Table::GetDateCreated ( ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = get_DateCreated(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _variant_t(_result, false);
}

inline _variant_t _Table::GetDateModified ( ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = get_DateModified(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _variant_t(_result, false);
}

inline _CatalogPtr _Table::GetParentCatalog ( ) {
    struct _Catalog * _result;
    HRESULT _hr = get_ParentCatalog(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _CatalogPtr(_result, false);
}

inline void _Table::PutParentCatalog ( struct _Catalog * ppvObject ) {
    HRESULT _hr = put_ParentCatalog(ppvObject);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline void _Table::PutRefParentCatalog ( struct _Catalog * ppvObject ) {
    HRESULT _hr = putref_ParentCatalog(ppvObject);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

//
// interface Columns wrapper method implementations
//

inline _ColumnPtr Columns::GetItem ( const _variant_t & Item ) {
    struct _Column * _result;
    HRESULT _hr = get_Item(Item, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _ColumnPtr(_result, false);
}

inline HRESULT Columns::Append ( const _variant_t & Item, enum DataTypeEnumAdoX Type, long DefinedSize ) {
    HRESULT _hr = raw_Append(Item, Type, DefinedSize);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT Columns::Delete ( const _variant_t & Item ) {
    HRESULT _hr = raw_Delete(Item);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

//
// interface _Column wrapper method implementations
//

inline _bstr_t _Column::GetName ( ) {
    BSTR _result;
    HRESULT _hr = get_Name(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline void _Column::PutName ( _bstr_t pVal ) {
    HRESULT _hr = put_Name(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline enum ColumnAttributesEnum _Column::GetAttributes ( ) {
    enum ColumnAttributesEnum _result;
    HRESULT _hr = get_Attributes(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void _Column::PutAttributes ( enum ColumnAttributesEnum pVal ) {
    HRESULT _hr = put_Attributes(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline long _Column::GetDefinedSize ( ) {
    long _result;
    HRESULT _hr = get_DefinedSize(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void _Column::PutDefinedSize ( long pVal ) {
    HRESULT _hr = put_DefinedSize(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline unsigned char _Column::GetNumericScale ( ) {
    unsigned char _result;
    HRESULT _hr = get_NumericScale(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void _Column::PutNumericScale ( unsigned char pVal ) {
    HRESULT _hr = put_NumericScale(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline long _Column::GetPrecision ( ) {
    long _result;
    HRESULT _hr = get_Precision(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void _Column::PutPrecision ( long pVal ) {
    HRESULT _hr = put_Precision(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline _bstr_t _Column::GetRelatedColumn ( ) {
    BSTR _result;
    HRESULT _hr = get_RelatedColumn(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline void _Column::PutRelatedColumn ( _bstr_t pVal ) {
    HRESULT _hr = put_RelatedColumn(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline enum SortOrderEnum _Column::GetSortOrder ( ) {
    enum SortOrderEnum _result;
    HRESULT _hr = get_SortOrder(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void _Column::PutSortOrder ( enum SortOrderEnum pVal ) {
    HRESULT _hr = put_SortOrder(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline enum DataTypeEnumAdoX _Column::GetType ( ) {
    enum DataTypeEnumAdoX _result;
    HRESULT _hr = get_Type(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void _Column::PutType ( enum DataTypeEnumAdoX pVal ) {
    HRESULT _hr = put_Type(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline PropertiesPtr _Column::GetProperties ( ) {
    struct Properties * _result;
    HRESULT _hr = get_Properties(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return PropertiesPtr(_result, false);
}

inline _CatalogPtr _Column::GetParentCatalog ( ) {
    struct _Catalog * _result;
    HRESULT _hr = get_ParentCatalog(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _CatalogPtr(_result, false);
}

inline void _Column::PutParentCatalog ( struct _Catalog * ppvObject ) {
    HRESULT _hr = put_ParentCatalog(ppvObject);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline void _Column::PutRefParentCatalog ( struct _Catalog * ppvObject ) {
    HRESULT _hr = putref_ParentCatalog(ppvObject);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

//
// interface Properties wrapper method implementations
//

inline PropertyPtr Properties::GetItem ( const _variant_t & Item ) {
    struct Property * _result;
    HRESULT _hr = get_Item(Item, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return PropertyPtr(_result, false);
}

//
// interface Property wrapper method implementations
//

inline _variant_t Property::GetValue ( ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = get_Value(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _variant_t(_result, false);
}

inline void Property::PutValue ( const _variant_t & pVal ) {
    HRESULT _hr = put_Value(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline _bstr_t Property::GetName ( ) {
    BSTR _result;
    HRESULT _hr = get_Name(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline enum DataTypeEnumAdoX Property::GetType ( ) {
    enum DataTypeEnumAdoX _result;
    HRESULT _hr = get_Type(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline long Property::GetAttributes ( ) {
    long _result;
    HRESULT _hr = get_Attributes(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void Property::PutAttributes ( long plAttributes ) {
    HRESULT _hr = put_Attributes(plAttributes);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

//
// interface Indexes wrapper method implementations
//

inline _IndexPtr Indexes::GetItem ( const _variant_t & Item ) {
    struct _Index * _result;
    HRESULT _hr = get_Item(Item, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _IndexPtr(_result, false);
}

inline HRESULT Indexes::Append ( const _variant_t & Item, const _variant_t & Columns ) {
    HRESULT _hr = raw_Append(Item, Columns);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT Indexes::Delete ( const _variant_t & Item ) {
    HRESULT _hr = raw_Delete(Item);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

//
// interface _Index wrapper method implementations
//

inline _bstr_t _Index::GetName ( ) {
    BSTR _result;
    HRESULT _hr = get_Name(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline void _Index::PutName ( _bstr_t pVal ) {
    HRESULT _hr = put_Name(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline VARIANT_BOOL _Index::GetClustered ( ) {
    VARIANT_BOOL _result;
    HRESULT _hr = get_Clustered(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void _Index::PutClustered ( VARIANT_BOOL pVal ) {
    HRESULT _hr = put_Clustered(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline enum AllowNullsEnum _Index::GetIndexNulls ( ) {
    enum AllowNullsEnum _result;
    HRESULT _hr = get_IndexNulls(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void _Index::PutIndexNulls ( enum AllowNullsEnum pVal ) {
    HRESULT _hr = put_IndexNulls(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline VARIANT_BOOL _Index::GetPrimaryKey ( ) {
    VARIANT_BOOL _result;
    HRESULT _hr = get_PrimaryKey(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void _Index::PutPrimaryKey ( VARIANT_BOOL pVal ) {
    HRESULT _hr = put_PrimaryKey(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline VARIANT_BOOL _Index::GetUnique ( ) {
    VARIANT_BOOL _result;
    HRESULT _hr = get_Unique(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void _Index::PutUnique ( VARIANT_BOOL pVal ) {
    HRESULT _hr = put_Unique(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline ColumnsPtr _Index::GetColumns ( ) {
    struct Columns * _result;
    HRESULT _hr = get_Columns(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return ColumnsPtr(_result, false);
}

inline PropertiesPtr _Index::GetProperties ( ) {
    struct Properties * _result;
    HRESULT _hr = get_Properties(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return PropertiesPtr(_result, false);
}

//
// interface Keys wrapper method implementations
//

inline _KeyPtr Keys::GetItem ( const _variant_t & Item ) {
    struct _Key * _result;
    HRESULT _hr = get_Item(Item, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _KeyPtr(_result, false);
}

inline HRESULT Keys::Append ( const _variant_t & Item, enum KeyTypeEnum Type, const _variant_t & Column, _bstr_t RelatedTable, _bstr_t RelatedColumn ) {
    HRESULT _hr = raw_Append(Item, Type, Column, RelatedTable, RelatedColumn);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT Keys::Delete ( const _variant_t & Item ) {
    HRESULT _hr = raw_Delete(Item);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

//
// interface _Key wrapper method implementations
//

inline _bstr_t _Key::GetName ( ) {
    BSTR _result;
    HRESULT _hr = get_Name(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline void _Key::PutName ( _bstr_t pVal ) {
    HRESULT _hr = put_Name(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline enum RuleEnum _Key::GetDeleteRule ( ) {
    enum RuleEnum _result;
    HRESULT _hr = get_DeleteRule(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void _Key::PutDeleteRule ( enum RuleEnum pVal ) {
    HRESULT _hr = put_DeleteRule(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline enum KeyTypeEnum _Key::GetType ( ) {
    enum KeyTypeEnum _result;
    HRESULT _hr = get_Type(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void _Key::PutType ( enum KeyTypeEnum pVal ) {
    HRESULT _hr = put_Type(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline _bstr_t _Key::GetRelatedTable ( ) {
    BSTR _result;
    HRESULT _hr = get_RelatedTable(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline void _Key::PutRelatedTable ( _bstr_t pVal ) {
    HRESULT _hr = put_RelatedTable(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline enum RuleEnum _Key::GetUpdateRule ( ) {
    enum RuleEnum _result;
    HRESULT _hr = get_UpdateRule(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void _Key::PutUpdateRule ( enum RuleEnum pVal ) {
    HRESULT _hr = put_UpdateRule(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline ColumnsPtr _Key::GetColumns ( ) {
    struct Columns * _result;
    HRESULT _hr = get_Columns(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return ColumnsPtr(_result, false);
}

//
// interface Procedures wrapper method implementations
//

inline ProcedurePtr Procedures::GetItem ( const _variant_t & Item ) {
    struct Procedure * _result;
    HRESULT _hr = get_Item(Item, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return ProcedurePtr(_result, false);
}

inline HRESULT Procedures::Append ( _bstr_t Name, IDispatch * Command ) {
    HRESULT _hr = raw_Append(Name, Command);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT Procedures::Delete ( const _variant_t & Item ) {
    HRESULT _hr = raw_Delete(Item);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

//
// interface Procedure wrapper method implementations
//

inline _variant_t Procedure::GetCommand ( ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = get_Command(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _variant_t(_result, false);
}

inline void Procedure::PutCommand ( const _variant_t & pVar ) {
    HRESULT _hr = put_Command(pVar);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline void Procedure::PutRefCommand ( IDispatch * pVar ) {
    HRESULT _hr = putref_Command(pVar);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline _bstr_t Procedure::GetName ( ) {
    BSTR _result;
    HRESULT _hr = get_Name(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline _variant_t Procedure::GetDateCreated ( ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = get_DateCreated(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _variant_t(_result, false);
}

inline _variant_t Procedure::GetDateModified ( ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = get_DateModified(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _variant_t(_result, false);
}

//
// interface Views wrapper method implementations
//

inline ViewPtr Views::GetItem ( const _variant_t & Item ) {
    struct View * _result;
    HRESULT _hr = get_Item(Item, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return ViewPtr(_result, false);
}

inline HRESULT Views::Append ( _bstr_t Name, IDispatch * Command ) {
    HRESULT _hr = raw_Append(Name, Command);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT Views::Delete ( const _variant_t & Item ) {
    HRESULT _hr = raw_Delete(Item);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

//
// interface View wrapper method implementations
//

inline _variant_t View::GetCommand ( ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = get_Command(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _variant_t(_result, false);
}

inline void View::PutCommand ( const _variant_t & pVal ) {
    HRESULT _hr = put_Command(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline void View::PutRefCommand ( IDispatch * pVal ) {
    HRESULT _hr = putref_Command(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline _bstr_t View::GetName ( ) {
    BSTR _result;
    HRESULT _hr = get_Name(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline _variant_t View::GetDateCreated ( ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = get_DateCreated(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _variant_t(_result, false);
}

inline _variant_t View::GetDateModified ( ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = get_DateModified(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _variant_t(_result, false);
}

//
// interface Groups wrapper method implementations
//

inline _GroupPtr Groups::GetItem ( const _variant_t & Item ) {
    struct _Group * _result;
    HRESULT _hr = get_Item(Item, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _GroupPtr(_result, false);
}

inline HRESULT Groups::Append ( const _variant_t & Item ) {
    HRESULT _hr = raw_Append(Item);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT Groups::Delete ( const _variant_t & Item ) {
    HRESULT _hr = raw_Delete(Item);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

//
// interface _Group wrapper method implementations
//

inline PropertiesPtr _Group::GetProperties ( ) {
    struct Properties * _result;
    HRESULT _hr = get_Properties(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return PropertiesPtr(_result, false);
}

inline _CatalogPtr _Group::GetParentCatalog ( ) {
    struct _Catalog * _result;
    HRESULT _hr = get_ParentCatalog(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _CatalogPtr(_result, false);
}

inline void _Group::PutParentCatalog ( struct _Catalog * ppvObject ) {
    HRESULT _hr = put_ParentCatalog(ppvObject);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline void _Group::PutRefParentCatalog ( struct _Catalog * ppvObject ) {
    HRESULT _hr = putref_ParentCatalog(ppvObject);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

//
// interface _Group25 wrapper method implementations
//

inline _bstr_t _Group25::GetName ( ) {
    BSTR _result;
    HRESULT _hr = get_Name(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline void _Group25::PutName ( _bstr_t pVal ) {
    HRESULT _hr = put_Name(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline enum RightsEnum _Group25::GetPermissions ( const _variant_t & Name, enum ObjectTypeEnum ObjectType, const _variant_t & ObjectTypeId ) {
    enum RightsEnum _result;
    HRESULT _hr = raw_GetPermissions(Name, ObjectType, ObjectTypeId, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline HRESULT _Group25::SetPermissions ( const _variant_t & Name, enum ObjectTypeEnum ObjectType, enum ActionEnum Action, enum RightsEnum Rights, enum InheritTypeEnum Inherit, const _variant_t & ObjectTypeId ) {
    HRESULT _hr = raw_SetPermissions(Name, ObjectType, Action, Rights, Inherit, ObjectTypeId);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline UsersPtr _Group25::GetUsers ( ) {
    struct Users * _result;
    HRESULT _hr = get_Users(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return UsersPtr(_result, false);
}

//
// interface Users wrapper method implementations
//

inline _UserPtr Users::GetItem ( const _variant_t & Item ) {
    struct _User * _result;
    HRESULT _hr = get_Item(Item, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _UserPtr(_result, false);
}

inline HRESULT Users::Append ( const _variant_t & Item, _bstr_t Password ) {
    HRESULT _hr = raw_Append(Item, Password);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT Users::Delete ( const _variant_t & Item ) {
    HRESULT _hr = raw_Delete(Item);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

//
// interface _User wrapper method implementations
//

inline PropertiesPtr _User::GetProperties ( ) {
    struct Properties * _result;
    HRESULT _hr = get_Properties(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return PropertiesPtr(_result, false);
}

inline _CatalogPtr _User::GetParentCatalog ( ) {
    struct _Catalog * _result;
    HRESULT _hr = get_ParentCatalog(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _CatalogPtr(_result, false);
}

inline void _User::PutParentCatalog ( struct _Catalog * ppvObject ) {
    HRESULT _hr = put_ParentCatalog(ppvObject);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline void _User::PutRefParentCatalog ( struct _Catalog * ppvObject ) {
    HRESULT _hr = putref_ParentCatalog(ppvObject);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

//
// interface _User25 wrapper method implementations
//

inline _bstr_t _User25::GetName ( ) {
    BSTR _result;
    HRESULT _hr = get_Name(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline void _User25::PutName ( _bstr_t pVal ) {
    HRESULT _hr = put_Name(pVal);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline enum RightsEnum _User25::GetPermissions ( const _variant_t & Name, enum ObjectTypeEnum ObjectType, const _variant_t & ObjectTypeId ) {
    enum RightsEnum _result;
    HRESULT _hr = raw_GetPermissions(Name, ObjectType, ObjectTypeId, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline HRESULT _User25::SetPermissions ( const _variant_t & Name, enum ObjectTypeEnum ObjectType, enum ActionEnum Action, enum RightsEnum Rights, enum InheritTypeEnum Inherit, const _variant_t & ObjectTypeId ) {
    HRESULT _hr = raw_SetPermissions(Name, ObjectType, Action, Rights, Inherit, ObjectTypeId);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT _User25::ChangePassword ( _bstr_t OldPassword, _bstr_t NewPassword ) {
    HRESULT _hr = raw_ChangePassword(OldPassword, NewPassword);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline GroupsPtr _User25::GetGroups ( ) {
    struct Groups * _result;
    HRESULT _hr = get_Groups(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return GroupsPtr(_result, false);
}

} // namespace XTPREPORTADOX

#pragma pack(pop)
