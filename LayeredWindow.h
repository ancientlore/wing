// LayeredWindow.h: interface for the CLayeredWindow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LAYEREDWINDOW_H__62D69DC0_5D3E_430F_AB09_87A61191C7AD__INCLUDED_)
#define AFX_LAYEREDWINDOW_H__62D69DC0_5D3E_430F_AB09_87A61191C7AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

template <class T>
class ATL_NO_VTABLE CLayeredWindow {
protected:
	CComPtr<IPicture> pPic;
	CBitmap mBgBmp;
	SIZE mBgBmpSize;

	void LoadBitmap(LPCTSTR path, COLORREF crKey)
	{
		ClearBitmaps();
		wchar_t pt[512];
		mbstowcs(pt, path, 511);
		pt[511] = L'\0';
		IPicture* p;
		if (!FAILED(OleLoadPicturePath(pt, NULL, 0, crKey, IID_IPicture, (LPVOID*) &p))) {
			pPic.Attach(p);
			HBITMAP h;
			if (!FAILED(p->get_Handle((OLE_HANDLE*)&h)))
				mBgBmp.Attach(h);
		}
	}

	void LoadBitmap(UINT nResId)
	{
		ClearBitmaps();
		mBgBmp.LoadBitmap(nResId);
	}

	void ClearBitmaps()
	{
		// the documentation about IPicture is pretty unclear about who owns the handle
		// but it seems to work to delete it everywhere
		if (!mBgBmp.IsNull()) {
#if 0
			if (!pPic)
				mBgBmp.DeleteObject();
			else
				mBgBmp.Detach();
#else
			mBgBmp.DeleteObject();
#endif
		}
		pPic.Release();
	}

public:

	BEGIN_MSG_MAP(CLayeredWindow<T>)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
	END_MSG_MAP()

	LRESULT SetupLayer(UINT nResId, COLORREF crKey, BYTE bAlpha)
	{
		if (nResId != -1) {
			LoadBitmap(nResId);
			if (!mBgBmp.IsNull()) {
				mBgBmp.GetSize(mBgBmpSize);
				RECT r;
				static_cast<T*>(this)->GetWindowRect(&r);
				static_cast<T*>(this)->SetWindowPos(NULL, r.left, r.top, mBgBmpSize.cx, mBgBmpSize.cy, SWP_NOREDRAW);
			}
			else
				mBgBmpSize.cx = mBgBmpSize.cy = 1;
		}
		static_cast<T*>(this)->SetWindowLong(GWL_EXSTYLE, static_cast<T*>(this)->GetWindowLong(GWL_EXSTYLE) | WS_EX_LAYERED);
		ChangeLayer(crKey, bAlpha);
		return TRUE;
	}

	LRESULT SetupLayer(LPCTSTR path, COLORREF crKey, BYTE bAlpha)
	{
		if (path != NULL) {
			LoadBitmap(path, crKey);
			if (!mBgBmp.IsNull()) {
				mBgBmp.GetSize(mBgBmpSize);
				RECT r;
				static_cast<T*>(this)->GetWindowRect(&r);
				static_cast<T*>(this)->SetWindowPos(NULL, r.left, r.top, mBgBmpSize.cx, mBgBmpSize.cy, SWP_NOREDRAW);
			}
			else
				mBgBmpSize.cx = mBgBmpSize.cy = 1;
		}
		static_cast<T*>(this)->SetWindowLong(GWL_EXSTYLE, static_cast<T*>(this)->GetWindowLong(GWL_EXSTYLE) | WS_EX_LAYERED);
		ChangeLayer(crKey, bAlpha);
		return TRUE;
	}

	LRESULT ChangeLayer(COLORREF crKey, BYTE bAlpha)
	{
		DWORD flags = LWA_COLORKEY;
		if (bAlpha != 255)
			flags |= LWA_ALPHA;
		SetLayeredWindowAttributes(static_cast<T*>(this)->m_hWnd, crKey, bAlpha, flags);
		return TRUE;
	}

	LRESULT OnEraseBackground(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	{
		if(!mBgBmp.IsNull())
		{
			CDCHandle dc = (HDC)wParam;
			CDC dcMem;
			dcMem.CreateCompatibleDC(dc);
			HBITMAP hBmpOld = dcMem.SelectBitmap(mBgBmp);
			dc.BitBlt(0, 0, mBgBmpSize.cx, mBgBmpSize.cy, dcMem, 0, 0, SRCCOPY);
			dcMem.SelectBitmap(hBmpOld);
		}
		else
			bHandled = FALSE;

		return 0;
	}

	BYTE PercentToAlpha(DWORD pct) {
		DWORD alpha;
		if (pct == 0)
			alpha = 255;
		else
			alpha = 255 * (100 - pct) / 100;
		return (BYTE) alpha;
	}
};

#endif // !defined(AFX_LAYEREDWINDOW_H__62D69DC0_5D3E_430F_AB09_87A61191C7AD__INCLUDED_)
