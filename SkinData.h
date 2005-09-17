#pragma once

typedef std::deque<ULONG> GraphPoints;
typedef std::deque<ULONG>::const_reverse_iterator GraphIterator;

class CTextData
{
public:
	BOOL Show;
	POINT Location;
	COLORREF Color;
	COLORREF ShadowColor;
	COLORREF ReflectColor;
	CString Font;
	DWORD FontSize;
	BOOL FontBold;
	BOOL FontItalic;
	int FontAngle;
	CString FontAlign;

	CTextData()
	{
		Reset();
	}

	void Reset(BOOL showDefault = TRUE)
	{
		Show = showDefault;
		Location.x = 0;
		Location.y = 0;
		Color = RGB(255, 255, 0);
		ShadowColor = RGB(0, 0, 0);
		ReflectColor = (COLORREF)-1;
		Font = _T("Arial");
		FontSize = 12;
		FontBold = TRUE;
		FontItalic = FALSE;
		FontAngle = 0;
		FontAlign = _T("left");
	}

	void Load(LPCTSTR section, LPCTSTR file, BOOL showDefault = TRUE)
	{
		TCHAR buf[MAX_PATH];
		Show = GetPrivateProfileInt(section, _T("Show"), showDefault, file);
		Location.x = GetPrivateProfileInt(section, _T("X"), 0, file);
		Location.y = GetPrivateProfileInt(section, _T("Y"), 0, file);
		Color = GetPrivateProfileInt(section, _T("Color"), RGB(255, 255, 0), file);
		ShadowColor = GetPrivateProfileInt(section, _T("ShadowColor"), -1, file);
		ReflectColor = GetPrivateProfileInt(section, _T("ReflectColor"), -1, file);
		Font.Empty();
		GetPrivateProfileString(section, _T("Font"), _T("Arial"), buf, MAX_PATH, file);
		Font = buf;
		FontSize = GetPrivateProfileInt(section, _T("Size"), 12, file);
		FontBold = GetPrivateProfileInt(section, _T("Bold"), TRUE, file);
		FontItalic = GetPrivateProfileInt(section, _T("Italic"), FALSE, file);
		FontAngle = GetPrivateProfileInt(section, _T("Angle"), 0, file);
		FontAlign.Empty();
		GetPrivateProfileString(section, _T("Align"), _T("left"), buf, MAX_PATH, file);
		FontAlign = buf;
		FontAlign.MakeLower();
		if (FontAlign.Compare(_T("left")) != 0 && FontAlign.Compare(_T("center")) != 0 && FontAlign.Compare(_T("right")) != 0)
			FontAlign = _T("left");
	}


	void Paint(CDC& dc, BOOL enableClearType, LPCTSTR text)
	{
		CFont f;
		HFONT old;
		if (Show) {
			dc.SetBkMode(TRANSPARENT);
			f.CreateFont(-(LONG) FontSize, 0, FontAngle, FontAngle, (FontBold ? FW_BOLD : FW_NORMAL), FontItalic, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, (enableClearType ? CLEARTYPE_QUALITY : NONANTIALIASED_QUALITY), DEFAULT_PITCH, Font);
			old = dc.SelectFont(f);
			UINT flags = TA_BASELINE | dc.GetTextAlign();
			if (FontAlign.CompareNoCase(_T("center")) == 0) {
				flags &= ~(TA_LEFT | TA_RIGHT);
				flags |= TA_CENTER;
			}
			else if (FontAlign.CompareNoCase(_T("right")) == 0) {
				flags &= ~(TA_LEFT | TA_CENTER);
				flags |= TA_RIGHT;
			}
			else {
				flags &= ~(TA_CENTER | TA_RIGHT);
				flags |= TA_LEFT;
			}
			dc.SetTextAlign(flags);
			if (ReflectColor != Color && ReflectColor != (COLORREF)-1) {
				dc.SetTextColor(ReflectColor);
				dc.TextOut(Location.x - 1, Location.y - 1, text);
			}
			if (ShadowColor != Color && ShadowColor != (COLORREF)-1) {
				dc.SetTextColor(ShadowColor);
				dc.TextOut(Location.x + 1, Location.y + 1, text);
			}
			dc.SetTextColor(Color);
			dc.TextOut(Location.x, Location.y, text);
			dc.SelectFont(old);
			f.DeleteObject();
		}
	}
};

class CBitmapData
{
public:
	CString BitmapFile;
	COLORREF TransparentColor;

	CBitmapData()
	{
		Reset();
	}

	void Reset()
	{
		BitmapFile.Empty();
		TransparentColor = RGB(255, 0, 255);
	}

	void Load(LPCTSTR section, LPCTSTR file);
};

class CGraphData
{
public:
	BOOL Show;
	RECT Rect;
	CString Style;
	COLORREF Color;
	COLORREF WarningColor;
	COLORREF ErrorColor;
	COLORREF ShadowColor;

	CGraphData()
	{
		Reset();
	}

	void Reset(BOOL showDefault = TRUE)
	{
		Show = showDefault;
		Style = _T("Horizontal");
		if (showDefault) {
			Rect.left = 58;
			Rect.top = 3;
			Rect.bottom = 28;
			Rect.right = 122;
			Color = RGB(255, 255, 255);
			WarningColor = RGB(255, 255, 255);
			ErrorColor = RGB(255, 255, 255);
			ShadowColor = (COLORREF)-1;
		}
		else {
			Rect.left = 2;
			Rect.top = 2;
			Rect.bottom = 14;
			Rect.right = 40;
			Color = RGB(0, 255, 0);
			WarningColor = RGB(255, 255, 0);
			ErrorColor = RGB(255, 0, 0);
			ShadowColor = RGB(0, 0, 0);
		}
	}

	void Load(LPCTSTR section, LPCTSTR file, BOOL showDefault = TRUE)
	{
		TCHAR buf[MAX_PATH];
		Show = GetPrivateProfileInt(section, _T("Show"), showDefault, file);
		Rect.top = GetPrivateProfileInt(section, _T("Top"), 2, file);
		Rect.left = GetPrivateProfileInt(section, _T("Left"), 2, file);
		Rect.bottom = GetPrivateProfileInt(section, _T("Bottom"), 18, file);
		Rect.right = GetPrivateProfileInt(section, _T("Right"), 34, file);
		Color = GetPrivateProfileInt(section, _T("Color"), RGB(0, 255, 0), file);
		WarningColor = GetPrivateProfileInt(section, _T("WarningColor"), RGB(255, 255, 0), file);
		ErrorColor = GetPrivateProfileInt(section, _T("ErrorColor"), RGB(255, 0, 0), file);
		ShadowColor = GetPrivateProfileInt(section, _T("ShadowColor"), RGB(0, 0, 0), file);
		GetPrivateProfileString(section, _T("Style"), _T("Horizontal"), buf, MAX_PATH, file);
		Style = buf;
		Style.MakeLower();
		if (Style.Compare(_T("horizontal")) != 0 && Style.Compare(_T("vertical")) != 0)
			Style = _T("horitonal");
	}

	void Paint(CDC& dc, const GraphPoints& pts)
	{
		if (Show) {
			// styles can be Horizontal, Vertical
			HPEN old;
			CPen pen, warningPen, errorPen, shadowPen;
			pen.CreatePen(PS_SOLID, 1, Color);
			warningPen.CreatePen(PS_SOLID, 1, WarningColor);
			errorPen.CreatePen(PS_SOLID, 1, ErrorColor);
			if (ShadowColor != Color && ShadowColor != (COLORREF)-1)
				shadowPen.CreatePen(PS_SOLID, 1, ShadowColor);
			old = dc.SelectPen(pen);
			int style = 0;
			if (Style.CompareNoCase(_T("vertical")) == 0)
				style = 1;
			else if (Style.CompareNoCase(_T("horizontal")) == 0)
				style = 0;
			else
				style = 0;
			
			GraphIterator iter = pts.rbegin();
			int i = (style ? Rect.top : Rect.right);
			while (iter != pts.rend()) {
				ULONG pct = *iter;
				CPen* p = &pen;
				if (pct > 66)
					p = &errorPen;
				else if (pct >= 33)
					p = &warningPen;

				++iter;
				ULONG next = 0;
				if (iter != pts.rend())
					next = *iter;

				if (style) {
					if (i <= Rect.bottom) {
						if (ShadowColor != Color && ShadowColor != (COLORREF)-1) {
							dc.SelectPen(shadowPen);
							dc.MoveTo(Rect.right + 1, i + 1);
							dc.LineTo(Rect.right - pct * (Rect.right - Rect.left + 1) / 100 + 1, i + 1);
						}
						dc.SelectPen(*p);
						dc.MoveTo(Rect.right, i);
						dc.LineTo(Rect.right - pct * (Rect.right - Rect.left + 1) / 100, i);
						i++;
					}
					else
						break;
				}
				else {
					if (i >= Rect.left) {
						if (ShadowColor != Color && ShadowColor != (COLORREF)-1) {
							dc.SelectPen(shadowPen);
							if (i == Rect.right) {
								dc.MoveTo(i + 1, Rect.bottom + 1);
								dc.LineTo(i + 1, Rect.bottom - pct * (Rect.bottom - Rect.top + 1) / 100 + 1);
							}
							dc.MoveTo(i, Rect.bottom + 1);
							dc.LineTo(i, Rect.bottom - next * (Rect.bottom - Rect.top + 1) / 100 + 1);
						}
						dc.SelectPen(*p);
						dc.MoveTo(i, Rect.bottom);
						dc.LineTo(i, Rect.bottom - pct * (Rect.bottom - Rect.top + 1) / 100);
						i--;
					}
					else
						break;
				}
			}

			dc.SelectPen(old);
			pen.DeleteObject();
			warningPen.DeleteObject();
			errorPen.DeleteObject();
			if (ShadowColor != Color && ShadowColor != (COLORREF)-1)
				shadowPen.DeleteObject();
		}
	}

};

class CSkinData
{
public:
	DWORD EnableClearType;
	CBitmapData Background;
	CTextData RoundTripTime;
	CTextData WarningRoundTripTime;
	CTextData ErrorRoundTripTime;
	CTextData Description;
	CTextData WarningDescription;
	CTextData ErrorDescription;
	CGraphData Graph;
	CString Credits;
	CString Notes;

	CSkinData() : EnableClearType(-1) {}

	void Reset()
	{
		EnableClearType = -1;
		Background.Reset();
		RoundTripTime.Reset();
		WarningRoundTripTime.Reset(FALSE);
		ErrorRoundTripTime.Reset(FALSE);
		Description.Reset();
		WarningDescription.Reset(FALSE);
		ErrorDescription.Reset(FALSE);
		Graph.Reset(TRUE);
		Credits = _T("by Michael D. Lore");
		Notes = _T("Default skin");
	}

	void Load(LPCTSTR file)
	{
		TCHAR buf[MAX_PATH];
		Background.Load(_T("Background"), file);
		RoundTripTime.Load(_T("RoundTripTime"), file);
		WarningRoundTripTime.Load(_T("WarningRoundTripTime"), file, FALSE);
		ErrorRoundTripTime.Load(_T("ErrorRoundTripTime"), file, FALSE);
		Description.Load(_T("Description"), file);
		WarningDescription.Load(_T("WarningDescription"), file, FALSE);
		ErrorDescription.Load(_T("ErrorDescription"), file, FALSE);
		Graph.Load(_T("Graph"), file, FALSE);
		Credits.Empty();
		GetPrivateProfileString(_T("General"), _T("Credits"), _T(""), buf, MAX_PATH, file);
		Credits = buf;
		Notes.Empty();
		GetPrivateProfileString(_T("General"), _T("Notes"), _T(""), buf, MAX_PATH, file);
		Notes = buf;
		EnableClearType = GetPrivateProfileInt(_T("General"), _T("ClearType"), -1, file);
	}

	void Paint(CDC& dc, BOOL enableClearType, LPCTSTR pingTime, LPCTSTR desc, const GraphPoints& pts)
	{
		if (EnableClearType != -1)
			enableClearType = EnableClearType;
		Graph.Paint(dc, pts);
		Description.Paint(dc, enableClearType, desc);
		RoundTripTime.Paint(dc, enableClearType, pingTime);
	}

	void PaintWarning(CDC& dc, BOOL enableClearType, LPCTSTR pingTime, LPCTSTR desc, const GraphPoints& pts)
	{
		if (EnableClearType != -1)
			enableClearType = EnableClearType;

		Graph.Paint(dc, pts);

		if (WarningDescription.Show == TRUE)
			WarningDescription.Paint(dc, enableClearType, desc);
		else
			Description.Paint(dc, enableClearType, desc);

		if (WarningRoundTripTime.Show == TRUE)
			WarningRoundTripTime.Paint(dc, enableClearType, pingTime);
		else
			RoundTripTime.Paint(dc, enableClearType, pingTime);
	}

	void PaintError(CDC& dc, BOOL enableClearType, LPCTSTR pingTime, LPCTSTR desc, const GraphPoints& pts)
	{
		if (EnableClearType != -1)
			enableClearType = EnableClearType;

		Graph.Paint(dc, pts);

		if (ErrorDescription.Show == TRUE)
			ErrorDescription.Paint(dc, enableClearType, desc);
		else if (WarningDescription.Show == TRUE)
			WarningDescription.Paint(dc, enableClearType, desc);
		else
			Description.Paint(dc, enableClearType, desc);
		
		if (ErrorRoundTripTime.Show == TRUE)
			ErrorRoundTripTime.Paint(dc, enableClearType, pingTime);
		else if (WarningRoundTripTime.Show == TRUE)
			WarningRoundTripTime.Paint(dc, enableClearType, pingTime);
		else
			RoundTripTime.Paint(dc, enableClearType, pingTime);
	}

	void LoadDefaultSkin()
	{
		Reset();
		RoundTripTime.Location.x = 124;
		RoundTripTime.Location.y = 14;
		RoundTripTime.FontSize = 20;
		RoundTripTime.FontAngle = 100;
		RoundTripTime.FontAlign = _T("right");
		Description.Location.x = 31;
		Description.Location.y = 9;
		Description.FontSize = 9;
		Description.FontItalic = TRUE;
	}

	static void GetSkinsDir(CString& s)
	{
		TCHAR buf[MAX_PATH];
		GetModuleFileName(_Module.GetModuleInstance(), buf, MAX_PATH);
		TCHAR* w = _tcsrchr(buf, _T('\\'));
		if (w != NULL)
			*(w+1) = _T('\0');
		s = buf;
		s += _T("skins\\");
	}
};

inline void CBitmapData::Load(LPCTSTR section, LPCTSTR file)
{
	TCHAR buf[MAX_PATH];
	BitmapFile.Empty();
	GetPrivateProfileString(section, _T("Bitmap"), _T(""), buf, MAX_PATH, file);
	CSkinData::GetSkinsDir(BitmapFile);
	BitmapFile += buf;
	TransparentColor = GetPrivateProfileInt(section, _T("TransparentColor"), RGB(255,0,255), file);
}
