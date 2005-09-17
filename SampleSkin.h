#pragma once

class CSampleSkin : public CWindowImpl<CSampleSkin, CWindow>
{
	CComPtr<IPicture> pPic;
	CBitmap mBgBmp;
	int type;
	BOOL needTimer;

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
	CSkinData m_skinData;

	CSampleSkin() : type(0), needTimer(TRUE) {}

	void ChangeSkin(CString& skin)
	{
		CString f;
		if (skin != _T("_default")) {
			CSkinData::GetSkinsDir(f);
			f += _T("\\");
			f += skin;
			f += _T(".ini");
			m_skinData.Load(f);
			LoadBitmap(m_skinData.Background.BitmapFile, m_skinData.Background.TransparentColor);
		}
		else {
			m_skinData.LoadDefaultSkin();
			LoadBitmap(IDB_ANGEL);
		}
		type = 0;
		if (IsWindow())
			InvalidateRect(NULL);
	}

	BEGIN_MSG_MAP(CSampleSkin)
		// MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
	END_MSG_MAP()

	//LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//{
	//	return 0;
	//}

	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if (needTimer == TRUE) {
			SetTimer(1, 2000);
			needTimer = FALSE;
		}

		CPaintDC dc(*this);
		dc.SetMapMode(MM_TEXT);
		RECT r;
		GetClientRect(&r);
		dc.BeginPath();
		dc.Rectangle(&r);
		dc.EndPath();
		dc.SelectClipPath(RGN_COPY);

		if(!mBgBmp.IsNull())
		{
			SIZE sz;
			mBgBmp.GetSize(sz);
			CDC dcMem;
			dcMem.CreateCompatibleDC(dc);
			HBITMAP hBmpOld = dcMem.SelectBitmap(mBgBmp);
			// dc.BitBlt(0, 0, sz.cx, sz.cy, dcMem, 0, 0, SRCCOPY);
			dc.TransparentBlt(0, 0, sz.cx, sz.cy, dcMem, 0, 0, sz.cx, sz.cy, m_skinData.Background.TransparentColor);
			dcMem.SelectBitmap(hBmpOld);
		}

		GraphPoints pts;
		for (int i = 0; i < 100; i++)
			pts.push_back(((i + 75) % 50) + 25 - ((100 - i) % 25));

		if (type == 0)
			m_skinData.Paint(dc, TRUE, _T("67ms"), _T("skyegg"), pts);
		else if (type == 1)
			m_skinData.PaintWarning(dc, TRUE, _T("134ms"), _T("skyegg"), pts);
		else if (type == 2)
			m_skinData.PaintError(dc, TRUE, _T("299ms"), _T("skyegg"), pts);
		else {
			CString s;
			s.LoadString(IDS_TIMEOUT);
			m_skinData.PaintError(dc, TRUE, s, _T("skyegg"), pts);
		}

		return 0;
	}

	LRESULT OnEraseBackground(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	{
		CDCHandle dc = (HDC)wParam;
		RECT r;
		GetClientRect(&r);
#if 0
		CBrush b;
		b.CreateSysColorBrush(COLOR_BTNFACE);
		dc.FillSolidRect(
		HBRUSH old = dc.SelectBrush(b);
		dc.Rectangle(&r);
		dc.SelectBrush(old);
#else
		dc.FillSolidRect(&r, GetSysColor(COLOR_BTNFACE));
#endif
		return 0;
	}

	LRESULT OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		type = (type + 1) % 4;
		InvalidateRect(NULL);
		return 0;
	}
};
