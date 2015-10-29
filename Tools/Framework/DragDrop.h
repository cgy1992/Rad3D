/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MInclude.h"

//
class DataObject : public IDataObject
{
public:
	DataObject(FORMATETC * fmtetc, STGMEDIUM * stgmed);
	virtual ~DataObject();

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject);

	virtual ULONG STDMETHODCALLTYPE AddRef(void);
	virtual ULONG STDMETHODCALLTYPE Release(void);

	virtual HRESULT STDMETHODCALLTYPE GetData(FORMATETC *pformatetcIn, STGMEDIUM *pmedium);
	virtual HRESULT STDMETHODCALLTYPE GetDataHere(FORMATETC *pformatetc, STGMEDIUM *pmedium);
	virtual HRESULT STDMETHODCALLTYPE QueryGetData(FORMATETC *pformatetc);
	virtual HRESULT STDMETHODCALLTYPE GetCanonicalFormatEtc(FORMATETC *pformatectIn, FORMATETC *pformatetcOut);
	virtual HRESULT STDMETHODCALLTYPE SetData(FORMATETC *pformatetc, STGMEDIUM *pmedium, BOOL fRelease);
	virtual HRESULT STDMETHODCALLTYPE EnumFormatEtc(DWORD dwDirection, IEnumFORMATETC **ppenumFormatEtc);
	virtual HRESULT STDMETHODCALLTYPE DAdvise(FORMATETC *pformatetc, DWORD advf, IAdviseSink *pAdvSink, DWORD *pdwConnection);
	virtual HRESULT STDMETHODCALLTYPE DUnadvise(DWORD dwConnection);
	virtual HRESULT STDMETHODCALLTYPE EnumDAdvise(IEnumSTATDATA **ppenumAdvise);

protected:
	int LookupData(FORMATETC *pformatetcIn);

protected:
	ULONG mRefCount;
	FORMATETC mFormatEtc;
	STGMEDIUM mStgMedium;
};

//
class DropSource : public IDropSource
{
public:
	DropSource();
	virtual ~DropSource();

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject);

	virtual ULONG STDMETHODCALLTYPE AddRef(void);
	virtual ULONG STDMETHODCALLTYPE Release(void);

	virtual HRESULT STDMETHODCALLTYPE QueryContinueDrag(BOOL fEscapePressed, DWORD grfKeyState);
	virtual HRESULT STDMETHODCALLTYPE GiveFeedback(DWORD dwEffect);

protected:
	ULONG mRefCount;
};

//
class EnumFORMATETC : public IEnumFORMATETC
{
public:
	EnumFORMATETC(FORMATETC * pFormatEtc, int count);
	virtual ~EnumFORMATETC();

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject);

	virtual ULONG STDMETHODCALLTYPE AddRef(void);
	virtual ULONG STDMETHODCALLTYPE Release(void);

	virtual HRESULT STDMETHODCALLTYPE Next(ULONG celt, FORMATETC *rgelt, ULONG *pceltFetched);
	virtual HRESULT STDMETHODCALLTYPE Skip(ULONG celt);
	virtual HRESULT STDMETHODCALLTYPE Reset(void);
	virtual HRESULT STDMETHODCALLTYPE Clone(IEnumFORMATETC **ppenum);

protected:
	ULONG mRefCount;

	int mIndex;
	int mCount;
	FORMATETC * mFormatEtc;
};

//
void Util_SetDrapFile(const String & filename);


//
class DropTarget : public IDropTarget
{
public:
	DropTarget(HWND hWnd);
	virtual ~DropTarget();

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject);

	virtual ULONG STDMETHODCALLTYPE AddRef(void);
	virtual ULONG STDMETHODCALLTYPE Release(void);

	virtual HRESULT STDMETHODCALLTYPE DragEnter(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect);
	virtual HRESULT STDMETHODCALLTYPE DragOver(DWORD grfKeyState, POINTL pt, DWORD *pdwEffect);
	virtual HRESULT STDMETHODCALLTYPE DragLeave(void);
	virtual HRESULT STDMETHODCALLTYPE Drop(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect);

protected:
	ULONG mRefCount;
	HWND mWnd;
};

//
typedef void (*DRAGFILE_CALLBACK)(const char * filename);
void Util_SetDragFileCallback(HWND hwnd, DRAGFILE_CALLBACK callback);
