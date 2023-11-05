// XTPXMLhelpers.inl : XML Helpers
// Created by Microsoft (R) C/C++ Compiler Version 11.00.50727.1 (fbac001d).
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
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

//{{AFX_CODEJOCK_PRIVATE

//
// interface IXMLDOMImplementation wrapper method implementations
//

inline VARIANT_BOOL IXMLDOMImplementation::hasFeature ( _bstr_t feature, _bstr_t version ) {
    VARIANT_BOOL _result = 0;
    HRESULT _hr = raw_hasFeature(feature, version, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

//
// interface IXMLDOMParseError wrapper method implementations
//

inline long IXMLDOMParseError::GeterrorCode ( ) {
    long _result = 0;
    HRESULT _hr = get_errorCode(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline _bstr_t IXMLDOMParseError::Geturl ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_url(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline _bstr_t IXMLDOMParseError::Getreason ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_reason(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline _bstr_t IXMLDOMParseError::GetsrcText ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_srcText(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline long IXMLDOMParseError::Getline ( ) {
    long _result = 0;
    HRESULT _hr = get_line(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline long IXMLDOMParseError::Getlinepos ( ) {
    long _result = 0;
    HRESULT _hr = get_linepos(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline long IXMLDOMParseError::Getfilepos ( ) {
    long _result = 0;
    HRESULT _hr = get_filepos(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

//
// dispinterface XMLDOMDocumentEvents wrapper method implementations
//

inline HRESULT XMLDOMDocumentEvents::ondataavailable ( ) {
    HRESULT _result = 0;
    _com_dispatch_method(this, 0xc6, DISPATCH_METHOD, VT_ERROR, (void*)&_result, NULL);
    return _result;
}

inline HRESULT XMLDOMDocumentEvents::onreadystatechange ( ) {
    HRESULT _result = 0;
    _com_dispatch_method(this, DISPID_READYSTATECHANGE, DISPATCH_METHOD, VT_ERROR, (void*)&_result, NULL);
    return _result;
}

//
// interface IXMLHttpRequest wrapper method implementations
//

inline HRESULT IXMLHttpRequest::open ( _bstr_t bstrMethod, _bstr_t bstrUrl, const _variant_t & varAsync, const _variant_t & bstrUser, const _variant_t & bstrPassword ) {
    HRESULT _hr = raw_open(bstrMethod, bstrUrl, varAsync, bstrUser, bstrPassword);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IXMLHttpRequest::setRequestHeader ( _bstr_t bstrHeader, _bstr_t bstrValue ) {
    HRESULT _hr = raw_setRequestHeader(bstrHeader, bstrValue);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline _bstr_t IXMLHttpRequest::getResponseHeader ( _bstr_t bstrHeader ) {
    BSTR _result = 0;
    HRESULT _hr = raw_getResponseHeader(bstrHeader, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline _bstr_t IXMLHttpRequest::getAllResponseHeaders ( ) {
    BSTR _result = 0;
    HRESULT _hr = raw_getAllResponseHeaders(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline HRESULT IXMLHttpRequest::send ( const _variant_t & varBody ) {
    HRESULT _hr = raw_send(varBody);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IXMLHttpRequest::abort ( ) {
    HRESULT _hr = raw_abort();
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline long IXMLHttpRequest::Getstatus ( ) {
    long _result = 0;
    HRESULT _hr = get_status(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline _bstr_t IXMLHttpRequest::GetstatusText ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_statusText(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline IDispatchPtr IXMLHttpRequest::GetresponseXML ( ) {
    IDispatch * _result = 0;
    HRESULT _hr = get_responseXML(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IDispatchPtr(_result, false);
}

inline _bstr_t IXMLHttpRequest::GetresponseText ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_responseText(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline _variant_t IXMLHttpRequest::GetresponseBody ( ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = get_responseBody(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _variant_t(_result, false);
}

inline _variant_t IXMLHttpRequest::GetresponseStream ( ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = get_responseStream(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _variant_t(_result, false);
}

inline long IXMLHttpRequest::GetreadyState ( ) {
    long _result = 0;
    HRESULT _hr = get_readyState(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IXMLHttpRequest::Putonreadystatechange ( IDispatch * _arg1 ) {
    HRESULT _hr = put_onreadystatechange(_arg1);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

//
// interface IXMLElementCollection wrapper method implementations
//

inline void IXMLElementCollection::Putlength ( long p ) {
    HRESULT _hr = put_length(p);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline long IXMLElementCollection::Getlength ( ) {
    long _result = 0;
    HRESULT _hr = get_length(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline IUnknownPtr IXMLElementCollection::Get_newEnum ( ) {
    IUnknown * _result = 0;
    HRESULT _hr = get__newEnum(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IUnknownPtr(_result, false);
}

inline IDispatchPtr IXMLElementCollection::item ( const _variant_t & var1, const _variant_t & var2 ) {
    IDispatch * _result = 0;
    HRESULT _hr = raw_item(var1, var2, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IDispatchPtr(_result, false);
}

//
// interface IXMLElement wrapper method implementations
//

inline _bstr_t IXMLElement::GettagName ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_tagName(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline void IXMLElement::PuttagName ( _bstr_t p ) {
    HRESULT _hr = put_tagName(p);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline IXMLElementPtr IXMLElement::Getparent ( ) {
    struct IXMLElement * _result = 0;
    HRESULT _hr = get_parent(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLElementPtr(_result, false);
}

inline HRESULT IXMLElement::setAttribute ( _bstr_t strPropertyName, const _variant_t & PropertyValue ) {
    HRESULT _hr = raw_setAttribute(strPropertyName, PropertyValue);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline _variant_t IXMLElement::getAttribute ( _bstr_t strPropertyName ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = raw_getAttribute(strPropertyName, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _variant_t(_result, false);
}

inline HRESULT IXMLElement::removeAttribute ( _bstr_t strPropertyName ) {
    HRESULT _hr = raw_removeAttribute(strPropertyName);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline IXMLElementCollectionPtr IXMLElement::Getchildren ( ) {
    struct IXMLElementCollection * _result = 0;
    HRESULT _hr = get_children(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLElementCollectionPtr(_result, false);
}

inline long IXMLElement::Gettype ( ) {
    long _result = 0;
    HRESULT _hr = get_type(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline _bstr_t IXMLElement::Gettext ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_text(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline void IXMLElement::Puttext ( _bstr_t p ) {
    HRESULT _hr = put_text(p);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline HRESULT IXMLElement::addChild ( struct IXMLElement * pChildElem, long lIndex, long lReserved ) {
    HRESULT _hr = raw_addChild(pChildElem, lIndex, lReserved);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IXMLElement::removeChild ( struct IXMLElement * pChildElem ) {
    HRESULT _hr = raw_removeChild(pChildElem);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

//
// interface IXMLDocument wrapper method implementations
//

inline IXMLElementPtr IXMLDocument::Getroot ( ) {
    struct IXMLElement * _result = 0;
    HRESULT _hr = get_root(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLElementPtr(_result, false);
}

inline _bstr_t IXMLDocument::GetfileSize ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_fileSize(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline _bstr_t IXMLDocument::GetfileModifiedDate ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_fileModifiedDate(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline _bstr_t IXMLDocument::GetfileUpdatedDate ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_fileUpdatedDate(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline _bstr_t IXMLDocument::Geturl ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_url(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline void IXMLDocument::Puturl ( _bstr_t p ) {
    HRESULT _hr = put_url(p);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline _bstr_t IXMLDocument::GetmimeType ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_mimeType(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline long IXMLDocument::GetreadyState ( ) {
    long _result = 0;
    HRESULT _hr = get_readyState(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline _bstr_t IXMLDocument::Getcharset ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_charset(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline void IXMLDocument::Putcharset ( _bstr_t p ) {
    HRESULT _hr = put_charset(p);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline _bstr_t IXMLDocument::Getversion ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_version(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline _bstr_t IXMLDocument::Getdoctype ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_doctype(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline _bstr_t IXMLDocument::GetdtdURL ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_dtdURL(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline IXMLElementPtr IXMLDocument::createElement ( const _variant_t & vType, const _variant_t & var1 ) {
    struct IXMLElement * _result = 0;
    HRESULT _hr = raw_createElement(vType, var1, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLElementPtr(_result, false);
}

//
// interface IXMLElement2 wrapper method implementations
//

inline _bstr_t IXMLElement2::GettagName ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_tagName(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline void IXMLElement2::PuttagName ( _bstr_t p ) {
    HRESULT _hr = put_tagName(p);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline IXMLElement2Ptr IXMLElement2::Getparent ( ) {
    struct IXMLElement2 * _result = 0;
    HRESULT _hr = get_parent(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLElement2Ptr(_result, false);
}

inline HRESULT IXMLElement2::setAttribute ( _bstr_t strPropertyName, const _variant_t & PropertyValue ) {
    HRESULT _hr = raw_setAttribute(strPropertyName, PropertyValue);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline _variant_t IXMLElement2::getAttribute ( _bstr_t strPropertyName ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = raw_getAttribute(strPropertyName, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _variant_t(_result, false);
}

inline HRESULT IXMLElement2::removeAttribute ( _bstr_t strPropertyName ) {
    HRESULT _hr = raw_removeAttribute(strPropertyName);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline IXMLElementCollectionPtr IXMLElement2::Getchildren ( ) {
    struct IXMLElementCollection * _result = 0;
    HRESULT _hr = get_children(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLElementCollectionPtr(_result, false);
}

inline long IXMLElement2::Gettype ( ) {
    long _result = 0;
    HRESULT _hr = get_type(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline _bstr_t IXMLElement2::Gettext ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_text(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline void IXMLElement2::Puttext ( _bstr_t p ) {
    HRESULT _hr = put_text(p);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline HRESULT IXMLElement2::addChild ( struct IXMLElement2 * pChildElem, long lIndex, long lReserved ) {
    HRESULT _hr = raw_addChild(pChildElem, lIndex, lReserved);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IXMLElement2::removeChild ( struct IXMLElement2 * pChildElem ) {
    HRESULT _hr = raw_removeChild(pChildElem);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline IXMLElementCollectionPtr IXMLElement2::Getattributes ( ) {
    struct IXMLElementCollection * _result = 0;
    HRESULT _hr = get_attributes(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLElementCollectionPtr(_result, false);
}

//
// interface IXMLDocument2 wrapper method implementations
//

inline IXMLElement2Ptr IXMLDocument2::Getroot ( ) {
    struct IXMLElement2 * _result = 0;
    HRESULT _hr = get_root(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLElement2Ptr(_result, false);
}

inline _bstr_t IXMLDocument2::GetfileSize ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_fileSize(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline _bstr_t IXMLDocument2::GetfileModifiedDate ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_fileModifiedDate(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline _bstr_t IXMLDocument2::GetfileUpdatedDate ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_fileUpdatedDate(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline _bstr_t IXMLDocument2::Geturl ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_url(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline void IXMLDocument2::Puturl ( _bstr_t p ) {
    HRESULT _hr = put_url(p);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline _bstr_t IXMLDocument2::GetmimeType ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_mimeType(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline long IXMLDocument2::GetreadyState ( ) {
    long _result = 0;
    HRESULT _hr = get_readyState(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline _bstr_t IXMLDocument2::Getcharset ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_charset(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline void IXMLDocument2::Putcharset ( _bstr_t p ) {
    HRESULT _hr = put_charset(p);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline _bstr_t IXMLDocument2::Getversion ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_version(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline _bstr_t IXMLDocument2::Getdoctype ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_doctype(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline _bstr_t IXMLDocument2::GetdtdURL ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_dtdURL(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline IXMLElement2Ptr IXMLDocument2::createElement ( const _variant_t & vType, const _variant_t & var1 ) {
    struct IXMLElement2 * _result = 0;
    HRESULT _hr = raw_createElement(vType, var1, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLElement2Ptr(_result, false);
}

inline VARIANT_BOOL IXMLDocument2::Getasync ( ) {
    VARIANT_BOOL _result = 0;
    HRESULT _hr = get_async(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IXMLDocument2::Putasync ( VARIANT_BOOL pf ) {
    HRESULT _hr = put_async(pf);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

//
// interface IXMLAttribute wrapper method implementations
//

inline _bstr_t IXMLAttribute::Getname ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_name(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline _bstr_t IXMLAttribute::Getvalue ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_value(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

//
// interface IXMLError wrapper method implementations
//

inline HRESULT IXMLError::GetErrorInfo ( struct _xml_error * pErrorReturn ) {
    HRESULT _hr = raw_GetErrorInfo(pErrorReturn);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

//
// interface IXMLDOMNode wrapper method implementations
//

inline _bstr_t IXMLDOMNode::GetnodeName ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_nodeName(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline _variant_t IXMLDOMNode::GetnodeValue ( ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = get_nodeValue(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _variant_t(_result, false);
}

inline void IXMLDOMNode::PutnodeValue ( const _variant_t & value ) {
    HRESULT _hr = put_nodeValue(value);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline DOMNodeType IXMLDOMNode::GetnodeType ( ) {
    DOMNodeType _result;
    HRESULT _hr = get_nodeType(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline IXMLDOMNodePtr IXMLDOMNode::GetparentNode ( ) {
    struct IXMLDOMNode * _result = 0;
    HRESULT _hr = get_parentNode(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLDOMNodePtr(_result, false);
}

inline IXMLDOMNodeListPtr IXMLDOMNode::GetchildNodes ( ) {
    struct IXMLDOMNodeList * _result = 0;
    HRESULT _hr = get_childNodes(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLDOMNodeListPtr(_result, false);
}

inline IXMLDOMNodePtr IXMLDOMNode::GetfirstChild ( ) {
    struct IXMLDOMNode * _result = 0;
    HRESULT _hr = get_firstChild(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLDOMNodePtr(_result, false);
}

inline IXMLDOMNodePtr IXMLDOMNode::GetlastChild ( ) {
    struct IXMLDOMNode * _result = 0;
    HRESULT _hr = get_lastChild(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLDOMNodePtr(_result, false);
}

inline IXMLDOMNodePtr IXMLDOMNode::GetpreviousSibling ( ) {
    struct IXMLDOMNode * _result = 0;
    HRESULT _hr = get_previousSibling(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLDOMNodePtr(_result, false);
}

inline IXMLDOMNodePtr IXMLDOMNode::GetnextSibling ( ) {
    struct IXMLDOMNode * _result = 0;
    HRESULT _hr = get_nextSibling(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLDOMNodePtr(_result, false);
}

inline IXMLDOMNamedNodeMapPtr IXMLDOMNode::Getattributes ( ) {
    struct IXMLDOMNamedNodeMap * _result = 0;
    HRESULT _hr = get_attributes(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLDOMNamedNodeMapPtr(_result, false);
}

inline IXMLDOMNodePtr IXMLDOMNode::insertBefore ( struct IXMLDOMNode * newChild, const _variant_t & refChild ) {
    struct IXMLDOMNode * _result = 0;
    HRESULT _hr = raw_insertBefore(newChild, refChild, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLDOMNodePtr(_result, false);
}

inline IXMLDOMNodePtr IXMLDOMNode::replaceChild ( struct IXMLDOMNode * newChild, struct IXMLDOMNode * oldChild ) {
    struct IXMLDOMNode * _result = 0;
    HRESULT _hr = raw_replaceChild(newChild, oldChild, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLDOMNodePtr(_result, false);
}

inline IXMLDOMNodePtr IXMLDOMNode::removeChild ( struct IXMLDOMNode * childNode ) {
    struct IXMLDOMNode * _result = 0;
    HRESULT _hr = raw_removeChild(childNode, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLDOMNodePtr(_result, false);
}

inline IXMLDOMNodePtr IXMLDOMNode::appendChild ( struct IXMLDOMNode * newChild ) {
    struct IXMLDOMNode * _result = 0;
    HRESULT _hr = raw_appendChild(newChild, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLDOMNodePtr(_result, false);
}

inline VARIANT_BOOL IXMLDOMNode::hasChildNodes ( ) {
    VARIANT_BOOL _result = 0;
    HRESULT _hr = raw_hasChildNodes(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline IXMLDOMDocumentPtr IXMLDOMNode::GetownerDocument ( ) {
    struct IXMLDOMDocument * _result = 0;
    HRESULT _hr = get_ownerDocument(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLDOMDocumentPtr(_result, false);
}

inline IXMLDOMNodePtr IXMLDOMNode::cloneNode ( VARIANT_BOOL deep ) {
    struct IXMLDOMNode * _result = 0;
    HRESULT _hr = raw_cloneNode(deep, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLDOMNodePtr(_result, false);
}

inline _bstr_t IXMLDOMNode::GetnodeTypeString ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_nodeTypeString(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline _bstr_t IXMLDOMNode::Gettext ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_text(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline void IXMLDOMNode::Puttext ( _bstr_t text ) {
    HRESULT _hr = put_text(text);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline VARIANT_BOOL IXMLDOMNode::Getspecified ( ) {
    VARIANT_BOOL _result = 0;
    HRESULT _hr = get_specified(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline IXMLDOMNodePtr IXMLDOMNode::Getdefinition ( ) {
    struct IXMLDOMNode * _result = 0;
    HRESULT _hr = get_definition(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLDOMNodePtr(_result, false);
}

inline _variant_t IXMLDOMNode::GetnodeTypedValue ( ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = get_nodeTypedValue(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _variant_t(_result, false);
}

inline void IXMLDOMNode::PutnodeTypedValue ( const _variant_t & typedValue ) {
    HRESULT _hr = put_nodeTypedValue(typedValue);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline _variant_t IXMLDOMNode::GetdataType ( ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = get_dataType(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _variant_t(_result, false);
}

inline void IXMLDOMNode::PutdataType ( _bstr_t dataTypeName ) {
    HRESULT _hr = put_dataType(dataTypeName);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline _bstr_t IXMLDOMNode::Getxml ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_xml(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline _bstr_t IXMLDOMNode::transformNode ( struct IXMLDOMNode * stylesheet ) {
    BSTR _result = 0;
    HRESULT _hr = raw_transformNode(stylesheet, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline IXMLDOMNodeListPtr IXMLDOMNode::selectNodes ( _bstr_t queryString ) {
    struct IXMLDOMNodeList * _result = 0;
    HRESULT _hr = raw_selectNodes(queryString, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLDOMNodeListPtr(_result, false);
}

inline IXMLDOMNodePtr IXMLDOMNode::selectSingleNode ( _bstr_t queryString ) {
    struct IXMLDOMNode * _result = 0;
    HRESULT _hr = raw_selectSingleNode(queryString, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLDOMNodePtr(_result, false);
}

inline VARIANT_BOOL IXMLDOMNode::Getparsed ( ) {
    VARIANT_BOOL _result = 0;
    HRESULT _hr = get_parsed(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline _bstr_t IXMLDOMNode::GetnamespaceURI ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_namespaceURI(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline _bstr_t IXMLDOMNode::Getprefix ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_prefix(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline _bstr_t IXMLDOMNode::GetbaseName ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_baseName(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline HRESULT IXMLDOMNode::transformNodeToObject ( struct IXMLDOMNode * stylesheet, const _variant_t & outputObject ) {
    HRESULT _hr = raw_transformNodeToObject(stylesheet, outputObject);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

//
// interface IXMLDOMNodeList wrapper method implementations
//

inline IXMLDOMNodePtr IXMLDOMNodeList::Getitem ( long index ) {
    struct IXMLDOMNode * _result = 0;
    HRESULT _hr = get_item(index, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLDOMNodePtr(_result, false);
}

inline long IXMLDOMNodeList::Getlength ( ) {
    long _result = 0;
    HRESULT _hr = get_length(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline IXMLDOMNodePtr IXMLDOMNodeList::nextNode ( ) {
    struct IXMLDOMNode * _result = 0;
    HRESULT _hr = raw_nextNode(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLDOMNodePtr(_result, false);
}

inline HRESULT IXMLDOMNodeList::reset ( ) {
    HRESULT _hr = raw_reset();
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline IUnknownPtr IXMLDOMNodeList::Get_newEnum ( ) {
    IUnknown * _result = 0;
    HRESULT _hr = get__newEnum(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IUnknownPtr(_result, false);
}

//
// interface IXMLDOMNamedNodeMap wrapper method implementations
//

inline IXMLDOMNodePtr IXMLDOMNamedNodeMap::getNamedItem ( _bstr_t name ) {
    struct IXMLDOMNode * _result = 0;
    HRESULT _hr = raw_getNamedItem(name, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLDOMNodePtr(_result, false);
}

inline IXMLDOMNodePtr IXMLDOMNamedNodeMap::setNamedItem ( struct IXMLDOMNode * newItem ) {
    struct IXMLDOMNode * _result = 0;
    HRESULT _hr = raw_setNamedItem(newItem, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLDOMNodePtr(_result, false);
}

inline IXMLDOMNodePtr IXMLDOMNamedNodeMap::removeNamedItem ( _bstr_t name ) {
    struct IXMLDOMNode * _result = 0;
    HRESULT _hr = raw_removeNamedItem(name, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLDOMNodePtr(_result, false);
}

inline IXMLDOMNodePtr IXMLDOMNamedNodeMap::Getitem ( long index ) {
    struct IXMLDOMNode * _result = 0;
    HRESULT _hr = get_item(index, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLDOMNodePtr(_result, false);
}

inline long IXMLDOMNamedNodeMap::Getlength ( ) {
    long _result = 0;
    HRESULT _hr = get_length(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline IXMLDOMNodePtr IXMLDOMNamedNodeMap::getQualifiedItem ( _bstr_t baseName, _bstr_t namespaceURI ) {
    struct IXMLDOMNode * _result = 0;
    HRESULT _hr = raw_getQualifiedItem(baseName, namespaceURI, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLDOMNodePtr(_result, false);
}

inline IXMLDOMNodePtr IXMLDOMNamedNodeMap::removeQualifiedItem ( _bstr_t baseName, _bstr_t namespaceURI ) {
    struct IXMLDOMNode * _result = 0;
    HRESULT _hr = raw_removeQualifiedItem(baseName, namespaceURI, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLDOMNodePtr(_result, false);
}

inline IXMLDOMNodePtr IXMLDOMNamedNodeMap::nextNode ( ) {
    struct IXMLDOMNode * _result = 0;
    HRESULT _hr = raw_nextNode(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLDOMNodePtr(_result, false);
}

inline HRESULT IXMLDOMNamedNodeMap::reset ( ) {
    HRESULT _hr = raw_reset();
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline IUnknownPtr IXMLDOMNamedNodeMap::Get_newEnum ( ) {
    IUnknown * _result = 0;
    HRESULT _hr = get__newEnum(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IUnknownPtr(_result, false);
}

//
// interface IXMLDOMDocumentType wrapper method implementations
//

inline _bstr_t IXMLDOMDocumentType::Getname ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_name(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline IXMLDOMNamedNodeMapPtr IXMLDOMDocumentType::Getentities ( ) {
    struct IXMLDOMNamedNodeMap * _result = 0;
    HRESULT _hr = get_entities(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLDOMNamedNodeMapPtr(_result, false);
}

inline IXMLDOMNamedNodeMapPtr IXMLDOMDocumentType::Getnotations ( ) {
    struct IXMLDOMNamedNodeMap * _result = 0;
    HRESULT _hr = get_notations(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLDOMNamedNodeMapPtr(_result, false);
}

//
// interface IXMLDOMAttribute wrapper method implementations
//

inline _bstr_t IXMLDOMAttribute::Getname ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_name(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline _variant_t IXMLDOMAttribute::Getvalue ( ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = get_value(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _variant_t(_result, false);
}

inline void IXMLDOMAttribute::Putvalue ( const _variant_t & attributeValue ) {
    HRESULT _hr = put_value(attributeValue);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

//
// interface IXMLDOMElement wrapper method implementations
//

inline _bstr_t IXMLDOMElement::GettagName ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_tagName(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline _variant_t IXMLDOMElement::getAttribute ( _bstr_t name ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = raw_getAttribute(name, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _variant_t(_result, false);
}

inline HRESULT IXMLDOMElement::setAttribute ( _bstr_t name, const _variant_t & value ) {
    HRESULT _hr = raw_setAttribute(name, value);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IXMLDOMElement::removeAttribute ( _bstr_t name ) {
    HRESULT _hr = raw_removeAttribute(name);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline IXMLDOMAttributePtr IXMLDOMElement::getAttributeNode ( _bstr_t name ) {
    struct IXMLDOMAttribute * _result = 0;
    HRESULT _hr = raw_getAttributeNode(name, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLDOMAttributePtr(_result, false);
}

inline IXMLDOMAttributePtr IXMLDOMElement::setAttributeNode ( struct IXMLDOMAttribute * DOMAttribute ) {
    struct IXMLDOMAttribute * _result = 0;
    HRESULT _hr = raw_setAttributeNode(DOMAttribute, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLDOMAttributePtr(_result, false);
}

inline IXMLDOMAttributePtr IXMLDOMElement::removeAttributeNode ( struct IXMLDOMAttribute * DOMAttribute ) {
    struct IXMLDOMAttribute * _result = 0;
    HRESULT _hr = raw_removeAttributeNode(DOMAttribute, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLDOMAttributePtr(_result, false);
}

inline IXMLDOMNodeListPtr IXMLDOMElement::getElementsByTagName ( _bstr_t tagName ) {
    struct IXMLDOMNodeList * _result = 0;
    HRESULT _hr = raw_getElementsByTagName(tagName, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLDOMNodeListPtr(_result, false);
}

inline HRESULT IXMLDOMElement::normalize ( ) {
    HRESULT _hr = raw_normalize();
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

//
// interface IXMLDOMCharacterData wrapper method implementations
//

inline _bstr_t IXMLDOMCharacterData::Getdata ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_data(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline void IXMLDOMCharacterData::Putdata ( _bstr_t data ) {
    HRESULT _hr = put_data(data);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline long IXMLDOMCharacterData::Getlength ( ) {
    long _result = 0;
    HRESULT _hr = get_length(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline _bstr_t IXMLDOMCharacterData::substringData ( long offset, long count ) {
    BSTR _result = 0;
    HRESULT _hr = raw_substringData(offset, count, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline HRESULT IXMLDOMCharacterData::appendData ( _bstr_t data ) {
    HRESULT _hr = raw_appendData(data);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IXMLDOMCharacterData::insertData ( long offset, _bstr_t data ) {
    HRESULT _hr = raw_insertData(offset, data);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IXMLDOMCharacterData::deleteData ( long offset, long count ) {
    HRESULT _hr = raw_deleteData(offset, count);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IXMLDOMCharacterData::replaceData ( long offset, long count, _bstr_t data ) {
    HRESULT _hr = raw_replaceData(offset, count, data);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

//
// interface IXMLDOMText wrapper method implementations
//

inline IXMLDOMTextPtr IXMLDOMText::splitText ( long offset ) {
    struct IXMLDOMText * _result = 0;
    HRESULT _hr = raw_splitText(offset, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLDOMTextPtr(_result, false);
}

//
// interface IXMLDOMProcessingInstruction wrapper method implementations
//

inline _bstr_t IXMLDOMProcessingInstruction::Gettarget ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_target(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline _bstr_t IXMLDOMProcessingInstruction::Getdata ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_data(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline void IXMLDOMProcessingInstruction::Putdata ( _bstr_t value ) {
    HRESULT _hr = put_data(value);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

//
// interface IXMLDOMDocument wrapper method implementations
//

inline IXMLDOMDocumentTypePtr IXMLDOMDocument::Getdoctype ( ) {
    struct IXMLDOMDocumentType * _result = 0;
    HRESULT _hr = get_doctype(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLDOMDocumentTypePtr(_result, false);
}

inline IXMLDOMImplementationPtr IXMLDOMDocument::Getimplementation ( ) {
    struct IXMLDOMImplementation * _result = 0;
    HRESULT _hr = get_implementation(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLDOMImplementationPtr(_result, false);
}

inline IXMLDOMElementPtr IXMLDOMDocument::GetdocumentElement ( ) {
    struct IXMLDOMElement * _result = 0;
    HRESULT _hr = get_documentElement(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLDOMElementPtr(_result, false);
}

inline void IXMLDOMDocument::PutRefdocumentElement ( struct IXMLDOMElement * DOMElement ) {
    HRESULT _hr = putref_documentElement(DOMElement);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline IXMLDOMElementPtr IXMLDOMDocument::createElement ( _bstr_t tagName ) {
    struct IXMLDOMElement * _result = 0;
    HRESULT _hr = raw_createElement(tagName, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLDOMElementPtr(_result, false);
}

inline IXMLDOMDocumentFragmentPtr IXMLDOMDocument::createDocumentFragment ( ) {
    struct IXMLDOMDocumentFragment * _result = 0;
    HRESULT _hr = raw_createDocumentFragment(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLDOMDocumentFragmentPtr(_result, false);
}

inline IXMLDOMTextPtr IXMLDOMDocument::createTextNode ( _bstr_t data ) {
    struct IXMLDOMText * _result = 0;
    HRESULT _hr = raw_createTextNode(data, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLDOMTextPtr(_result, false);
}

inline IXMLDOMCommentPtr IXMLDOMDocument::createComment ( _bstr_t data ) {
    struct IXMLDOMComment * _result = 0;
    HRESULT _hr = raw_createComment(data, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLDOMCommentPtr(_result, false);
}

inline IXMLDOMCDATASectionPtr IXMLDOMDocument::createCDATASection ( _bstr_t data ) {
    struct IXMLDOMCDATASection * _result = 0;
    HRESULT _hr = raw_createCDATASection(data, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLDOMCDATASectionPtr(_result, false);
}

inline IXMLDOMProcessingInstructionPtr IXMLDOMDocument::createProcessingInstruction ( _bstr_t target, _bstr_t data ) {
    struct IXMLDOMProcessingInstruction * _result = 0;
    HRESULT _hr = raw_createProcessingInstruction(target, data, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLDOMProcessingInstructionPtr(_result, false);
}

inline IXMLDOMAttributePtr IXMLDOMDocument::createAttribute ( _bstr_t name ) {
    struct IXMLDOMAttribute * _result = 0;
    HRESULT _hr = raw_createAttribute(name, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLDOMAttributePtr(_result, false);
}

inline IXMLDOMEntityReferencePtr IXMLDOMDocument::createEntityReference ( _bstr_t name ) {
    struct IXMLDOMEntityReference * _result = 0;
    HRESULT _hr = raw_createEntityReference(name, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLDOMEntityReferencePtr(_result, false);
}

inline IXMLDOMNodeListPtr IXMLDOMDocument::getElementsByTagName ( _bstr_t tagName ) {
    struct IXMLDOMNodeList * _result = 0;
    HRESULT _hr = raw_getElementsByTagName(tagName, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLDOMNodeListPtr(_result, false);
}

inline IXMLDOMNodePtr IXMLDOMDocument::createNode ( const _variant_t & type, _bstr_t name, _bstr_t namespaceURI ) {
    struct IXMLDOMNode * _result = 0;
    HRESULT _hr = raw_createNode(type, name, namespaceURI, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLDOMNodePtr(_result, false);
}

inline IXMLDOMNodePtr IXMLDOMDocument::nodeFromID ( _bstr_t idString ) {
    struct IXMLDOMNode * _result = 0;
    HRESULT _hr = raw_nodeFromID(idString, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLDOMNodePtr(_result, false);
}

inline VARIANT_BOOL IXMLDOMDocument::load ( const _variant_t & xmlSource ) {
    VARIANT_BOOL _result = 0;
    HRESULT _hr = raw_load(xmlSource, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline long IXMLDOMDocument::GetreadyState ( ) {
    long _result = 0;
    HRESULT _hr = get_readyState(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline IXMLDOMParseErrorPtr IXMLDOMDocument::GetparseError ( ) {
    struct IXMLDOMParseError * _result = 0;
    HRESULT _hr = get_parseError(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLDOMParseErrorPtr(_result, false);
}

inline _bstr_t IXMLDOMDocument::Geturl ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_url(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline VARIANT_BOOL IXMLDOMDocument::Getasync ( ) {
    VARIANT_BOOL _result = 0;
    HRESULT _hr = get_async(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IXMLDOMDocument::Putasync ( VARIANT_BOOL isAsync ) {
    HRESULT _hr = put_async(isAsync);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline HRESULT IXMLDOMDocument::abort ( ) {
    HRESULT _hr = raw_abort();
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline VARIANT_BOOL IXMLDOMDocument::loadXML ( _bstr_t bstrXML ) {
    VARIANT_BOOL _result = 0;
    HRESULT _hr = raw_loadXML(bstrXML, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline HRESULT IXMLDOMDocument::save ( const _variant_t & destination ) {
    HRESULT _hr = raw_save(destination);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline VARIANT_BOOL IXMLDOMDocument::GetvalidateOnParse ( ) {
    VARIANT_BOOL _result = 0;
    HRESULT _hr = get_validateOnParse(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IXMLDOMDocument::PutvalidateOnParse ( VARIANT_BOOL isValidating ) {
    HRESULT _hr = put_validateOnParse(isValidating);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline VARIANT_BOOL IXMLDOMDocument::GetresolveExternals ( ) {
    VARIANT_BOOL _result = 0;
    HRESULT _hr = get_resolveExternals(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IXMLDOMDocument::PutresolveExternals ( VARIANT_BOOL isResolving ) {
    HRESULT _hr = put_resolveExternals(isResolving);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline VARIANT_BOOL IXMLDOMDocument::GetpreserveWhiteSpace ( ) {
    VARIANT_BOOL _result = 0;
    HRESULT _hr = get_preserveWhiteSpace(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IXMLDOMDocument::PutpreserveWhiteSpace ( VARIANT_BOOL isPreserving ) {
    HRESULT _hr = put_preserveWhiteSpace(isPreserving);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline void IXMLDOMDocument::Putonreadystatechange ( const _variant_t & _arg1 ) {
    HRESULT _hr = put_onreadystatechange(_arg1);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline void IXMLDOMDocument::Putondataavailable ( const _variant_t & _arg1 ) {
    HRESULT _hr = put_ondataavailable(_arg1);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline void IXMLDOMDocument::Putontransformnode ( const _variant_t & _arg1 ) {
    HRESULT _hr = put_ontransformnode(_arg1);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

//
// interface IXMLDOMNotation wrapper method implementations
//

inline _variant_t IXMLDOMNotation::GetpublicId ( ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = get_publicId(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _variant_t(_result, false);
}

inline _variant_t IXMLDOMNotation::GetsystemId ( ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = get_systemId(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _variant_t(_result, false);
}

//
// interface IXMLDOMEntity wrapper method implementations
//

inline _variant_t IXMLDOMEntity::GetpublicId ( ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = get_publicId(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _variant_t(_result, false);
}

inline _variant_t IXMLDOMEntity::GetsystemId ( ) {
    VARIANT _result;
    VariantInit(&_result);
    HRESULT _hr = get_systemId(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _variant_t(_result, false);
}

inline _bstr_t IXMLDOMEntity::GetnotationName ( ) {
    BSTR _result = 0;
    HRESULT _hr = get_notationName(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

//
// interface IXTLRuntime wrapper method implementations
//

inline long IXTLRuntime::uniqueID ( struct IXMLDOMNode * pNode ) {
    long _result = 0;
    HRESULT _hr = raw_uniqueID(pNode, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline long IXTLRuntime::depth ( struct IXMLDOMNode * pNode ) {
    long _result = 0;
    HRESULT _hr = raw_depth(pNode, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline long IXTLRuntime::childNumber ( struct IXMLDOMNode * pNode ) {
    long _result = 0;
    HRESULT _hr = raw_childNumber(pNode, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline long IXTLRuntime::ancestorChildNumber ( _bstr_t bstrNodeName, struct IXMLDOMNode * pNode ) {
    long _result = 0;
    HRESULT _hr = raw_ancestorChildNumber(bstrNodeName, pNode, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline long IXTLRuntime::absoluteChildNumber ( struct IXMLDOMNode * pNode ) {
    long _result = 0;
    HRESULT _hr = raw_absoluteChildNumber(pNode, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline _bstr_t IXTLRuntime::formatIndex ( long lIndex, _bstr_t bstrFormat ) {
    BSTR _result = 0;
    HRESULT _hr = raw_formatIndex(lIndex, bstrFormat, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline _bstr_t IXTLRuntime::formatNumber ( double dblNumber, _bstr_t bstrFormat ) {
    BSTR _result = 0;
    HRESULT _hr = raw_formatNumber(dblNumber, bstrFormat, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline _bstr_t IXTLRuntime::formatDate ( const _variant_t & varDate, _bstr_t bstrFormat, const _variant_t & varDestLocale ) {
    BSTR _result = 0;
    HRESULT _hr = raw_formatDate(varDate, bstrFormat, varDestLocale, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

inline _bstr_t IXTLRuntime::formatTime ( const _variant_t & varTime, _bstr_t bstrFormat, const _variant_t & varDestLocale ) {
    BSTR _result = 0;
    HRESULT _hr = raw_formatTime(varTime, bstrFormat, varDestLocale, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _bstr_t(_result, false);
}

//
// interface IXMLDSOControl wrapper method implementations
//

inline IXMLDOMDocumentPtr IXMLDSOControl::GetXMLDocument ( ) {
    struct IXMLDOMDocument * _result = 0;
    HRESULT _hr = get_XMLDocument(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IXMLDOMDocumentPtr(_result, false);
}

inline void IXMLDSOControl::PutXMLDocument ( struct IXMLDOMDocument * ppDoc ) {
    HRESULT _hr = put_XMLDocument(ppDoc);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline long IXMLDSOControl::GetJavaDSOCompatible ( ) {
    long _result = 0;
    HRESULT _hr = get_JavaDSOCompatible(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline void IXMLDSOControl::PutJavaDSOCompatible ( long fJavaDSOCompatible ) {
    HRESULT _hr = put_JavaDSOCompatible(fJavaDSOCompatible);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
}

inline long IXMLDSOControl::GetreadyState ( ) {
    long _result = 0;
    HRESULT _hr = get_readyState(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

//}}AFX_CODEJOCK_PRIVATE