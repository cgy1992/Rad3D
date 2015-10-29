#include "DragDrop.h"

//
DataObject::DataObject(FORMATETC * fmtetc, STGMEDIUM * stgmed)
{
	mRefCount = 1;

	mFormatEtc = *fmtetc;
	mStgMedium = *stgmed;
}

DataObject::~DataObject()
{
	d_assert (mRefCount == 0);
}

HRESULT DataObject::QueryInterface(REFIID riid, void **ppvObject)
{
	if (riid == IID_IUnknown || riid == IID_IDataObject)
	{
		*ppvObject = this;

		AddRef();

		return S_OK;
	}
	else
	{
		*ppvObject = NULL;
		
		return E_NOINTERFACE;
	}
}

ULONG DataObject::AddRef(void)
{
	mRefCount += 1;

	return mRefCount;
}

ULONG DataObject::Release(void)
{
	int count = --mRefCount;

	if (mRefCount == 0)
	{
		delete this;
	}

	return count;
}

int DataObject::LookupData(FORMATETC *pformatetcIn)
{
	if (mFormatEtc.tymed == pformatetcIn->tymed &&
		mFormatEtc.cfFormat == pformatetcIn->cfFormat &&
		mFormatEtc.dwAspect == pformatetcIn->dwAspect)
	{
		return 0;
	}

	return -1;
}

HGLOBAL DupGlobalMem(HGLOBAL hmem)
{
	DWORD len = GlobalSize(hmem);
	PVOID source = GlobalLock(hmem);
	PVOID dest = GlobalAlloc(GMEM_FIXED, len);
	memcpy(dest, source, len);
	GlobalUnlock(hmem);

	return dest;
}

HRESULT DataObject::GetData(FORMATETC *pformatetcIn, STGMEDIUM *pmedium)
{
	if (LookupData(pformatetcIn) == -1)
		return DV_E_FORMATETC;

	pmedium->tymed = mFormatEtc.tymed;
	pmedium->pUnkForRelease = 0;

	if (mFormatEtc.tymed == TYMED_HGLOBAL)
	{
		pmedium->hGlobal = DupGlobalMem(mStgMedium.hGlobal);
		return S_OK;
	}
	else
	{
		return DV_E_FORMATETC;
	}
}

HRESULT DataObject::GetDataHere(FORMATETC *pformatetc, STGMEDIUM *pmedium)
{
	return DV_E_FORMATETC;
}

HRESULT DataObject::QueryGetData(FORMATETC *pformatetc)
{
	return LookupData(pformatetc) != -1 ? S_OK : DV_E_FORMATETC;
}

HRESULT DataObject::GetCanonicalFormatEtc(FORMATETC *pformatectIn, FORMATETC *pformatetcOut)
{
	pformatetcOut->ptd = NULL;
	return E_NOTIMPL;
}

HRESULT DataObject::SetData(FORMATETC *pformatetc, STGMEDIUM *pmedium, BOOL fRelease)
{
	return E_NOTIMPL;
}

HRESULT DataObject::EnumFormatEtc(DWORD dwDirection, IEnumFORMATETC **ppenumFormatEtc)
{
	if (dwDirection == DATADIR_GET)
	{
		*ppenumFormatEtc = new EnumFORMATETC(&mFormatEtc, 1);
		return S_OK;
	}
	else
	{
		return E_NOTIMPL;
	}
}

HRESULT DataObject::DAdvise(FORMATETC *pformatetc, DWORD advf, IAdviseSink *pAdvSink, DWORD *pdwConnection)
{
	return OLE_E_ADVISENOTSUPPORTED;
}

HRESULT DataObject::DUnadvise(DWORD dwConnection)
{
	return OLE_E_ADVISENOTSUPPORTED;
}

HRESULT DataObject::EnumDAdvise(IEnumSTATDATA **ppenumAdvise)
{
	return OLE_E_ADVISENOTSUPPORTED;
}

//
EnumFORMATETC::EnumFORMATETC(FORMATETC * pFormatEtc, int count)
{
	mRefCount = 1;
	mCount = count;
	mIndex = 0;

	mFormatEtc = new FORMATETC[count];

	for (int i = 0; i < count; ++i)
	{
		mFormatEtc[i] = pFormatEtc[i];

		if (pFormatEtc[i].ptd)
		{
			mFormatEtc[i].ptd = (DVTARGETDEVICE *)CoTaskMemAlloc(sizeof(DVTARGETDEVICE));
			*(mFormatEtc[i].ptd) = *(pFormatEtc[i].ptd);
		}
	}
}

EnumFORMATETC::~EnumFORMATETC()
{
	d_assert (mRefCount == 0);

	for (int i = 0; i < mCount; ++i)
	{
		if (mFormatEtc[i].ptd)
			CoTaskMemFree(mFormatEtc[i].ptd);
	}

	delete[] mFormatEtc;
}

HRESULT EnumFORMATETC::QueryInterface(REFIID riid, void **ppvObject)
{
	if (riid == IID_IUnknown || riid == IID_IEnumFORMATETC)
	{
		*ppvObject = this;

		AddRef();

		return S_OK;
	}
	else
	{
		*ppvObject = NULL;

		return E_NOINTERFACE;
	}
}

ULONG EnumFORMATETC::AddRef(void)
{
	mRefCount += 1;

	return mRefCount;
}

ULONG EnumFORMATETC::Release(void)
{
	int count = --mRefCount;

	if (mRefCount == 0)
	{
		delete this;
	}

	return count;
}

HRESULT EnumFORMATETC::Next(ULONG celt, FORMATETC *rgelt, ULONG *pceltFetched)
{
	ULONG copied = 0;

	while (mIndex < mCount && copied < celt)
	{
		rgelt[copied] = mFormatEtc[mIndex];
		if (mFormatEtc[mIndex].ptd)
		{
			rgelt[copied].ptd = (DVTARGETDEVICE *)CoTaskMemAlloc(sizeof(DVTARGETDEVICE));
			*(rgelt[copied].ptd) = *(mFormatEtc[mIndex].ptd);
		}

		copied++;
		mIndex++;
	}

	if (pceltFetched != NULL)
		*pceltFetched = copied;

	return copied == celt ? S_OK : S_FALSE;
}

HRESULT EnumFORMATETC::Skip(ULONG celt)
{
	mIndex += celt;
	return mIndex < mCount ? S_OK : S_FALSE;
}

HRESULT EnumFORMATETC::Reset(void)
{
	mIndex = 0;
	return S_OK;
}

HRESULT EnumFORMATETC::Clone(IEnumFORMATETC **ppenum)
{
	*ppenum = new EnumFORMATETC(mFormatEtc, mCount);

	((EnumFORMATETC *)(*ppenum))->mIndex = mIndex;

	return S_OK;
}

//
DropSource::DropSource()
{
	mRefCount = 1;
}

DropSource::~DropSource()
{
	d_assert (mRefCount == 0);
}

HRESULT DropSource::QueryInterface(REFIID riid, void **ppvObject)
{
	if (riid == IID_IUnknown || riid == IID_IDropSource)
	{
		*ppvObject = this;

		AddRef();

		return S_OK;
	}
	else
	{
		*ppvObject = NULL;

		return E_NOINTERFACE;
	}
}

ULONG DropSource::AddRef(void)
{
	mRefCount += 1;

	return mRefCount;
}

ULONG DropSource::Release(void)
{
	int count = --mRefCount;

	if (mRefCount == 0)
	{
		delete this;
	}

	return count;
}

HRESULT DropSource::QueryContinueDrag(BOOL fEscapePressed, DWORD grfKeyState)
{
	if (fEscapePressed)
	{
		return DRAGDROP_S_CANCEL;
	}

	if ((grfKeyState & MK_LBUTTON) == 0)
	{
		return DRAGDROP_S_DROP;
	}

	return S_OK;
}

HRESULT DropSource::GiveFeedback(DWORD dwEffect)
{
	return DRAGDROP_S_USEDEFAULTCURSORS;
}

//
HANDLE StringToHandle(const String & str)
{
	void * ptr;
	int len = str.Length() + 1;

	ptr = (void *)GlobalAlloc(GMEM_FIXED, len);

	memcpy(ptr, str.c_str(), len);

	return ptr;
}

void Util_SetDrapFile(const String & filename)
{
	FORMATETC ftmetc = { CF_TEXT, 0, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
	STGMEDIUM stgmed = { TYMED_HGLOBAL, 0, 0 };
	stgmed.hGlobal = StringToHandle(filename);

	IDropSource * pDropSource = new DropSource;
	IDataObject * pDataObject = new DataObject(&ftmetc, &stgmed);

	DWORD dwEffect = DROPEFFECT_MOVE;
	HRESULT hr = DoDragDrop(pDataObject, pDropSource, DROPEFFECT_COPY, &dwEffect);

	pDropSource->Release();
	pDataObject->Release();

	ReleaseStgMedium(&stgmed);
}

//
DRAGFILE_CALLBACK gDragFileCallBack = NULL;
DropTarget * gDropTarget = NULL;

DropTarget::DropTarget(HWND hWnd)
{
	mWnd = hWnd;
	mRefCount = 1;
}

DropTarget::~DropTarget()
{
	d_assert(mRefCount == 0);
}

HRESULT DropTarget::QueryInterface(REFIID riid, void **ppvObject)
{
	if (riid == IID_IUnknown || riid == IID_IDropTarget)
	{
		*ppvObject = this;

		AddRef();

		return S_OK;
	}
	else
	{
		*ppvObject = NULL;

		return E_NOINTERFACE;
	}
}

ULONG DropTarget::AddRef(void)
{
	mRefCount += 1;

	return mRefCount;
}

ULONG DropTarget::Release(void)
{
	int count = --mRefCount;

	if (mRefCount == 0)
	{
		delete this;
	}

	return count;
}

HRESULT DropTarget::DragEnter(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
{
	FORMATETC fmtetc = { CF_TEXT, 0, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
	if (pDataObj->QueryGetData(&fmtetc) == S_OK)
	{
		SetFocus(mWnd);
		*pdwEffect = DROPEFFECT_COPY;
	}

	FORMATETC fmtetc2 = { CF_HDROP, 0, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
	if (pDataObj->QueryGetData(&fmtetc2) == S_OK)
	{
		SetFocus(mWnd);
		*pdwEffect = DROPEFFECT_COPY;
	}

	return S_OK;
}

HRESULT DropTarget::DragOver(DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
{
	*pdwEffect = DROPEFFECT_COPY;

	SetFocus(mWnd);

	return S_OK;
}

HRESULT DropTarget::DragLeave(void)
{
	return S_OK;
}

HRESULT DropTarget::Drop(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
{
	/*
	IEnumFORMATETC * pEnum = NULL;
	if (pDataObj->EnumFormatEtc(DATADIR_GET, &pEnum) == S_OK &&
		pEnum->Reset() == S_OK)
	{
		FORMATETC fmtetc;
		ULONG fetchedCount = 0;
		memset(&fmtetc, 0, sizeof(fmtetc));

		while (pEnum->Next(1, &fmtetc, &fetchedCount) == S_OK)
		{
			STGMEDIUM stgmed;
			if (pDataObj->GetData(&fmtetc, &stgmed) == S_OK)
			{
				FixedString256 filename;
				
				if (fmtetc.cfFormat == CF_TEXT && fmtetc.tymed == TYMED_HGLOBAL)
				{
					PVOID source = GlobalLock(stgmed.hGlobal);
					filename = (const char *)source;
					GlobalUnlock(stgmed.hGlobal);
				}
				else if (fmtetc.cfFormat == CF_HDROP && fmtetc.tymed == TYMED_HGLOBAL)
				{
					TCHAR temp[MAX_PATH] = { 0 };
					int len = DragQueryFile((HDROP)stgmed.hGlobal, 0, temp, MAX_PATH);

					filename = temp;
				}

				if (gDragFileCallBack != NULL && filename != "")
				{
					gDragFileCallBack(filename.c_str());
				}

				ReleaseStgMedium(&stgmed);
			}

			if (fmtetc.ptd)
			{
				CoTaskMemFree(fmtetc.ptd);
			}
		}

		pEnum->Release();
	}
	*/

	STGMEDIUM stgmed;

	FORMATETC fmtetc = { CF_TEXT, 0, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
	if (pDataObj->GetData(&fmtetc, &stgmed) == S_OK)
	{
		FixedString256 filename;

		PVOID source = GlobalLock(stgmed.hGlobal);
		filename = (const char *)source;
		GlobalUnlock(stgmed.hGlobal);

		if (gDragFileCallBack != NULL && filename != "")
		{
			gDragFileCallBack(filename.c_str());
		}

		ReleaseStgMedium(&stgmed);
	}

	FORMATETC fmtetc2 = { CF_HDROP, 0, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
	if (pDataObj->GetData(&fmtetc2, &stgmed) == S_OK)
	{
		FixedString256 filename;

		TCHAR temp[MAX_PATH] = { 0 };
		int len = DragQueryFile((HDROP)stgmed.hGlobal, 0, temp, MAX_PATH);
		filename = temp;

		if (gDragFileCallBack != NULL && filename != "")
		{
			gDragFileCallBack(filename.c_str());
		}

		ReleaseStgMedium(&stgmed);
	}

	*pdwEffect = DROPEFFECT_COPY;

	return S_OK;
}

//
void Util_SetDragFileCallback(HWND hwnd, DRAGFILE_CALLBACK callback)
{
	HRESULT hr = S_OK;

	if (callback != NULL)
	{
		d_assert (gDragFileCallBack == NULL && gDropTarget == NULL);

		gDragFileCallBack = callback;
		gDropTarget = new DropTarget(hwnd);
		hr = RegisterDragDrop(hwnd, gDropTarget);
	}
	else
	{
		d_assert (gDragFileCallBack != NULL && gDropTarget != NULL);

		hr = RevokeDragDrop(hwnd);

		gDropTarget->Release();
		gDropTarget = NULL;
		gDragFileCallBack = NULL;
	}
}