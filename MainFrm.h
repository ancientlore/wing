// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#define REQBUFSIZE 8192
#define REQSENDSIZE 32

#define POOR_RESPONSE_DELAY 3000 // ms
#define BRIGHT_DELAY 1000 // ms
#define BRIGHT_INTERVAL 25	// ms
#define BRIGHT_TIME 500 // ms
#define GRAPH_POINTS 100

class CMainFrame : public CFrameWindowImpl<CMainFrame>, public CUpdateUI<CMainFrame>,
		public CMessageFilter, public CIdleHandler, public CLayeredWindow<CMainFrame>
{
	// Use for window moving
	BOOL isMoving;
	HCURSOR oldCursor;
	SIZE edge;
	SIZE size;
	BOOL isBright;

	// Window ID settings
	CString WindowReg;

	// Display/Window Settings
	DWORD alwaysOnTop;
	DWORD lockPosition;
	DWORD transparency;
	POINT position;
	DWORD tempTrans;
	DWORD enableClearType;
	DWORD opaqueOnPoorResponse;
	CString skin;

	// Monitor properties
	CString m_host;
	CString m_desc;
	INT m_hops;
	INT m_updateFreq;
	INT m_graphMax;

	// host to ping
	ULONG m_destAddr;
	HANDLE m_hIcmp;

	// For ping
	char m_reqData[REQBUFSIZE];
	char m_respData[sizeof(ICMPECHO) + REQBUFSIZE];
	IP_OPTION_INFORMATION m_pipOptions;
	ULONG m_lastPingTime;
	HANDLE m_hEvent;
	GraphPoints m_pts;

	// Skin Data
	CSkinData skinData;

public:
	// Window ID settings
	CString WindowID;
	CRecentDocumentList m_mru;
	CMenu menu;

	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		return CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg);
	}

	virtual BOOL OnIdle()
	{
		UISetCheck(ID_LOCK_POSITION, (lockPosition == TRUE));
		UISetCheck(ID_ALWAYS_ON_TOP, (alwaysOnTop == TRUE));
		UISetCheck(ID_VIEW_OPAQUEONPOORRESPONSE, (opaqueOnPoorResponse == TRUE));
		UISetCheck(ID_ENABLE_CLEARTYPE, (enableClearType == TRUE));
		UISetCheck(ID_TRANS_VISIBLE, (transparency == 0));
		UISetCheck(ID_TRANS_90, (transparency == 90));
		UISetCheck(ID_TRANS_80, (transparency == 80));
		UISetCheck(ID_TRANS_70, (transparency == 70));
		UISetCheck(ID_TRANS_60, (transparency == 60));
		UISetCheck(ID_TRANS_50, (transparency == 50));
		UISetCheck(ID_TRANS_40, (transparency == 40));
		UISetCheck(ID_TRANS_30, (transparency == 30));
		UISetCheck(ID_TRANS_20, (transparency == 20));
		UISetCheck(ID_TRANS_10, (transparency == 10));

		if (gTimeToQuit)
			PostMessage(WM_CLOSE);

		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CMainFrame)
		UPDATE_ELEMENT(ID_ALWAYS_ON_TOP, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_LOCK_POSITION, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_OPAQUEONPOORRESPONSE, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_TRANS_VISIBLE, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_TRANS_10, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_TRANS_20, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_TRANS_30, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_TRANS_40, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_TRANS_50, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_TRANS_60, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_TRANS_70, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_TRANS_80, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_TRANS_90, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_ENABLE_CLEARTYPE, UPDUI_MENUPOPUP)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_CONTEXTMENU, OnContextMenu)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
		COMMAND_ID_HANDLER(ID_FILE_CLOSE, OnFileClose)
		COMMAND_ID_HANDLER(ID_FILE_NEW_WINDOW, OnFileNewWindow)
		COMMAND_ID_HANDLER(ID_PROPERTIES, OnProperties)
		COMMAND_ID_HANDLER(ID_SELECT_SKIN, OnSelectSkin)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(ID_ALWAYS_ON_TOP, OnAlwaysOnTop)
		COMMAND_ID_HANDLER(ID_LOCK_POSITION, OnLockPosition)
		COMMAND_ID_HANDLER(ID_ENABLE_CLEARTYPE, OnEnableClearType)
		COMMAND_ID_HANDLER(ID_HELP_VISITWEBSITE, OnVisitWebSite)
		COMMAND_ID_HANDLER(ID_MENU_UPDATENOW, OnUpdateNow)
		COMMAND_ID_HANDLER(ID_VIEW_OPAQUEONPOORRESPONSE, OnOpaqueOnPoorResponse)
		COMMAND_RANGE_HANDLER(ID_TRANS_VISIBLE, ID_TRANS_90, OnTransparency)
		COMMAND_RANGE_HANDLER(ID_FILE_MRU_FIRST, ID_FILE_MRU_LAST, OnFileRecent)
		CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
		CHAIN_MSG_MAP(CLayeredWindow<CMainFrame>)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	CMainFrame()
	{
		m_host = _T("www.yahoo.com");
		m_desc = _T("Yahoo!");
		m_hops = OPTIONS_DEFAULT_HOPS;
		m_updateFreq = OPTIONS_DEFAULT_FREQ;
		m_graphMax = OPTIONS_GRAPH_DEFAULT;
	}

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		isMoving = FALSE;
		oldCursor = NULL;
		isBright = FALSE;

		// init points
		for (int i = 0; i < GRAPH_POINTS; i++)
			m_pts.push_back(0);

		// create window unique id
		if (WindowID.IsEmpty()) {
			OLECHAR pBuf[40];
			GUID aGuid;
			HRESULT r = CoCreateGuid(&aGuid);
			if (FAILED(r))
				WindowID = _T("Shared");
			else {
				if (StringFromGUID2(aGuid, pBuf, 40) <= 0)
					WindowID = _T("Shared");
				else
					WindowID = pBuf;
			}
		}
		WindowReg = _T("Software\\Lore\\Wing\\Windows\\");
		WindowReg += WindowID;

		// register object for message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);

		LoadSettings();
		LoadSkin();

		if (skinData.Background.BitmapFile.IsEmpty())
			SetupLayer(IDB_ANGEL, skinData.Background.TransparentColor, PercentToAlpha(transparency));
		else
			SetupLayer(skinData.Background.BitmapFile, skinData.Background.TransparentColor, PercentToAlpha(transparency));

		FixWindowPos(position);

		HWND w = NULL;
		if (alwaysOnTop)
			w = HWND_TOPMOST;
		SetWindowPos(w, position.x, position.y, mBgBmpSize.cx, mBgBmpSize.cy, SWP_NOREDRAW);

		menu.LoadMenu(IDR_CTXMENU);
		CMenuHandle menuPopup = menu.GetSubMenu(0);
		CMenuHandle menuRecent = menuPopup.GetSubMenu(1);
		m_mru.SetMenuHandle(menuRecent);
		m_mru.ReadFromRegistry(_T("Software\\Lore\\Wing"));

		m_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

		m_hIcmp = lpfnIcmpCreateFile();

		SetupPing();

		return 0;
	}

	BOOL FixWindowRect(RECT& r)
	{
		POINT p;
		p.x = r.left;
		p.y = r.top;
		BOOL res = FixWindowPos(p);
		r.right += p.x - r.left;
		r.left = p.x;
		r.bottom += p.y - r.top;
		r.top = p.y;
		return res;
	}

	BOOL FixWindowPos(POINT& p)
	{
		RECT r;
		BOOL changed = FALSE;
		
		SystemParametersInfo(SPI_GETWORKAREA, 0, &r, 0);
		
		if (p.x < r.left) {
			p.x = r.left;
			changed = TRUE;
		}
		if (p.y < r.top) {
			p.y = r.top;
			changed = TRUE;
		}
		if (p.x + mBgBmpSize.cx > r.right) {
			p.x = r.right - mBgBmpSize.cx;
			changed = TRUE;
		}
		if (p.y + mBgBmpSize.cy > r.bottom) {
			p.y = r.bottom - mBgBmpSize.cy;
			changed = TRUE;
		}
		
		return changed;
	}

	LRESULT OnContextMenu(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if ((HWND) wParam == m_hWnd) {
			CMenuHandle menuPopup = menu.GetSubMenu(0);
			m_mru.ReadFromRegistry(_T("Software\\Lore\\Wing"));
			SetForegroundWindow(m_hWnd);
			menuPopup.TrackPopupMenu(TPM_RIGHTBUTTON | TPM_VERPOSANIMATION | TPM_VERTICAL,
				GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), m_hWnd);
			PostMessage(WM_NULL);
		}
		else
			bHandled = FALSE;
		return 0;
	}

	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		KillTimer(1);
		KillTimer(2);
		CloseHandle(m_hEvent);
		lpfnIcmpCloseHandle(m_hIcmp);
		PostQuitMessage(0);
		bHandled = FALSE;
		return 0;
	}

	LRESULT OnFileClose(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CRegKey key;
		if (key.Create(HKEY_CURRENT_USER, _T("Software\\Lore\\Wing\\Windows")) == ERROR_SUCCESS) {
			TCHAR kname[256];
			DWORD sz = 255;
			DWORD i = 0;
			while (key.EnumKey(i, kname, &sz) == ERROR_SUCCESS) {
				sz = 255;
				i++;
			}
			if (i > 1)
				key.DeleteSubKey(WindowID);
		}

		PostMessage(WM_CLOSE);
		return 0;
	}

	LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		gTimeToQuit = true;
		PostMessage(WM_CLOSE);
		return 0;
	}

	LRESULT OnFileNewWindow(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		::PostThreadMessage(_Module.m_dwMainThreadID, WM_USER, 0, 0L);
		return 0;
	}

	LRESULT OnFileRecent(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		TCHAR szItem[MAX_PATH];
		szItem[0] = _T('\0');
		m_mru.GetFromList(wID, szItem);
		m_desc = szItem;
		LoadMonitor();
		m_mru.MoveToTop(wID);
		m_mru.WriteToRegistry(_T("Software\\Lore\\Wing"));
		CRegKey key;
		if (key.Create(HKEY_CURRENT_USER, WindowReg) == ERROR_SUCCESS)
			key.SetStringValue(_T("Monitor"), m_desc);
		SetupPing();
		return 0;
	}

	LRESULT OnVisitWebSite(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		ShellExecute(NULL, NULL, _T("http://software.skyegg.com/"), NULL, NULL, SW_SHOWNORMAL);
		return 0;
	}
	
	LRESULT OnSelectSkin(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CSelectSkinDlg dlg;
		dlg.m_skin = skin;
		if (dlg.DoModal() == IDOK) {
			skin = dlg.m_skin;
			LoadSkin();
			if (skinData.Background.BitmapFile.IsEmpty())
				SetupLayer(IDB_ANGEL, skinData.Background.TransparentColor, PercentToAlpha(transparency));
			else
				SetupLayer(skinData.Background.BitmapFile, skinData.Background.TransparentColor, PercentToAlpha(transparency));
			CRegKey key;
			if (key.Create(HKEY_CURRENT_USER, WindowReg) == ERROR_SUCCESS)
				key.SetStringValue(_T("Skin"), skin);
			RECT r;
			GetWindowRect(&r);
			//ClientToScreen(&r);
			BOOL changed = FixWindowRect(r);
			if (changed == TRUE)
				MoveWindow(&r);
			InvalidateRect(NULL);
		}

		return 0;
	}

	LRESULT OnProperties(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		COptionsDlg dlg;
		dlg.m_host = m_host;
		dlg.m_desc = m_desc;
		dlg.m_hops = m_hops;
		dlg.m_updateFreq = m_updateFreq;
		dlg.m_graphMax = m_graphMax;

		if (dlg.DoModal() == IDOK) {
			m_host = dlg.m_host;
			m_desc = dlg.m_desc;
			m_hops = dlg.m_hops;
			m_updateFreq = dlg.m_updateFreq;
			m_graphMax = dlg.m_graphMax;
			m_mru.AddToList(m_desc);
			m_mru.WriteToRegistry(_T("Software\\Lore\\Wing"));
			CRegKey key;
			if (key.Create(HKEY_CURRENT_USER, WindowReg) == ERROR_SUCCESS)
				key.SetStringValue(_T("Monitor"), m_desc);
			SaveMonitor();
			RemoveUnusedMonitors();
			SetupPing();
		}
		return 0;
	}

	LRESULT OnUpdateNow(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		SetupPing();
		return 0;
	}

	void SetupPing()
	{
		m_lastPingTime = -1;

		if (m_host.IsEmpty()) {
			m_lastPingTime = -3;
			m_pts.pop_front();
			m_pts.push_back(0);
			KillTimer(1);
			InvalidateRect(NULL);
			return;
		}

		LPHOSTENT lpstHost = NULL;

		m_destAddr = inet_addr(m_host);
		if (m_destAddr == INADDR_NONE) {
			lpstHost = gethostbyname(m_host);
			if (!lpstHost) {
				KillTimer(1);
				m_lastPingTime = -2;
				m_pts.pop_front();
				m_pts.push_back(100);
				InvalidateRect(NULL);
				SetTimer(4, 30000);
				return;
			}
			m_destAddr = *((ULONG*)lpstHost->h_addr);
		}

		KillTimer(4);

		//CString x;
		//x.FormatMessage("%1 == %2!d!", (LPCTSTR) m_host, m_destAddr.s_addr);
		//MessageBox(x);

		m_pipOptions.Ttl = m_hops;
		m_pipOptions.Tos = 0;
		m_pipOptions.Flags = 0;
		m_pipOptions.OptionsData = NULL;
		m_pipOptions.OptionsSize = 0;

		lpfnIcmpSendEcho2(m_hIcmp, m_hEvent, NULL, this, m_destAddr,
			m_reqData, REQSENDSIZE, &m_pipOptions, m_respData, sizeof(m_respData), 5000);

		SetTimer(2, 100);
		InvalidateRect(NULL);
	}

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CAboutDlg dlg;
		dlg.DoModal();
		return 0;
	}

	LRESULT OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		if (wParam == 1) {
			KillTimer(1);

			m_pipOptions.Ttl = m_hops;
			m_pipOptions.Tos = 0;
			m_pipOptions.Flags = 0;
			m_pipOptions.OptionsData = NULL;
			m_pipOptions.OptionsSize = 0;

			lpfnIcmpSendEcho2(m_hIcmp, m_hEvent, NULL, this, m_destAddr,
				m_reqData, REQSENDSIZE, &m_pipOptions, m_respData, sizeof(m_respData), 5000);

			SetTimer(2, 100);
		}
		else if (wParam == 2) {
			if (WaitForSingleObject(m_hEvent, 0) == WAIT_OBJECT_0) {
				KillTimer(2);
				DWORD count = lpfnIcmpParseReplies(m_respData, sizeof(m_respData));
				if (count > 0) {
					ICMPECHO* pEcho = (ICMPECHO*) m_respData;
					m_lastPingTime = pEcho->RoundTripTime;
					m_pts.pop_front();
					ULONG x = m_lastPingTime * 100 / m_graphMax;
					if (x > 100)
						x = 100;
					m_pts.push_back(x);
				}
				else {
					m_lastPingTime = -1;
					m_pts.pop_front();
					m_pts.push_back(100);
				}
				InvalidateRect(NULL);
				SetTimer(1, m_updateFreq);
				if (opaqueOnPoorResponse && (((ULONG) m_lastPingTime * 100 / m_graphMax) > 66 || m_lastPingTime < 0) && isBright == FALSE) {
					tempTrans = 0;
					ChangeLayer(skinData.Background.TransparentColor, PercentToAlpha(tempTrans));
					SetTimer(3, POOR_RESPONSE_DELAY);
				}

			}
		}
		else if (wParam == 3) {
			if (tempTrans >= transparency)
				KillTimer(3);
			else {
				if (tempTrans == 0)
					SetTimer(3, BRIGHT_INTERVAL);
				tempTrans += (BRIGHT_INTERVAL * transparency) / BRIGHT_TIME;
				if (tempTrans > transparency)
					tempTrans = transparency;
				ChangeLayer(skinData.Background.TransparentColor, PercentToAlpha(tempTrans));
			}
		}
		else if (wParam == 4) {
			SetupPing();
		}

		return 0;
	}

	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CPaintDC dc(*this);

		CString x;
		ULONG pct;
		switch ((LONG) m_lastPingTime) {
		case -1:
			x.LoadString(IDS_TIMEOUT);
			pct = 100;
			break;
		case -2:
			x.LoadString(IDS_RESOLVEHOST);
			pct = 50;
			break;
		case -3:
			x.LoadString(IDS_NOTHINGTODO);
			pct = 0;
			break;
		default:
			if (m_lastPingTime > 9999)
				x.LoadString(IDS_PINGEXCEEDED);
			else
				x.FormatMessage(IDS_PINGFMT, m_lastPingTime);
			pct = m_lastPingTime * 100 / m_graphMax;
			break;
		}

		if (pct > 66)
			skinData.PaintError(dc, enableClearType, x, m_desc, m_pts);
		else if (pct >= 33)
			skinData.PaintWarning(dc, enableClearType, x, m_desc, m_pts);
		else
			skinData.Paint(dc, enableClearType, x, m_desc, m_pts);

		return 0;
	}

	LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
	{
		RECT tmpRect;

		if (isMoving == FALSE  &&  lockPosition == FALSE) {
			isMoving = TRUE;
			HCURSOR cur = LoadCursor(NULL, IDC_SIZEALL);
			oldCursor = SetCursor(cur);
			GetWindowRect(&tmpRect);
			size.cx = tmpRect.right - tmpRect.left;
			size.cy = tmpRect.bottom - tmpRect.top;
			edge.cx = GET_X_LPARAM(lParam);
			edge.cy = GET_Y_LPARAM(lParam);
			SetCapture();
		}
		else
			bHandled = FALSE;
		return 0;
	}

	LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
	{
		if (isMoving == TRUE) {
			RECT r;
			r.left = GET_X_LPARAM(lParam) - edge.cx;
			r.right = r.left + size.cx;
			r.top = GET_Y_LPARAM(lParam) - edge.cy;
			r.bottom = r.top + size.cy;
			ClientToScreen(&r);
			FixWindowRect(r);
			MoveWindow(&r, TRUE);
		}
		else if (isBright == FALSE) {
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.dwFlags = TME_LEAVE;
			tme.dwHoverTime = 0;
			tme.hwndTrack = m_hWnd;
			isBright = TRUE;
			tempTrans = 0;
			KillTimer(3);
			ChangeLayer(skinData.Background.TransparentColor, PercentToAlpha(tempTrans));
			TrackMouseEvent(&tme);
		}
		else
			bHandled = FALSE;
		return 0;
	}

	LRESULT OnMouseLeave(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
	{
		//ChangeLayer(skinData.Background.TransparentColor, PercentToAlpha(transparency));
		SetTimer(3, BRIGHT_DELAY);
		isBright = FALSE;
		return 0;
	}

	LRESULT OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
	{
		if (isMoving == TRUE) {
			RECT r;
			r.left = GET_X_LPARAM(lParam) - edge.cx;
			r.right = r.left + size.cx;
			r.top = GET_Y_LPARAM(lParam) - edge.cy;
			r.bottom = r.top + size.cy;
			ClientToScreen(&r);
			FixWindowRect(r);
			MoveWindow(&r, TRUE);
			position.x = r.left;
			position.y = r.top;
			HCURSOR curs = SetCursor(oldCursor);
			CloseHandle(curs);
			isMoving = FALSE;
			ReleaseCapture();
			CRegKey key;
			if (key.Create(HKEY_CURRENT_USER, WindowReg) == ERROR_SUCCESS) {
				key.SetDWORDValue(_T("Left"), position.x);
				key.SetDWORDValue(_T("Top"), position.y);
			}
		}
		else
			bHandled = FALSE;
		return 0;
	}

	LRESULT OnAlwaysOnTop(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CRegKey key;
		if (key.Create(HKEY_CURRENT_USER, WindowReg) == ERROR_SUCCESS) {
			if (alwaysOnTop == FALSE) {
				alwaysOnTop = TRUE;
				SetWindowPos(HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			}
			else {
				alwaysOnTop = FALSE;
				SetWindowPos(HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			}
			key.SetDWORDValue(_T("Always On Top"), alwaysOnTop);
		}

		return 0;
	}

	LRESULT OnLockPosition(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CRegKey key;
		if (key.Create(HKEY_CURRENT_USER, WindowReg) == ERROR_SUCCESS) {
			if (lockPosition == FALSE)
				lockPosition = TRUE;
			else
				lockPosition = FALSE;
			key.SetDWORDValue(_T("Lock Position"), lockPosition);
		}
		return 0;
	}

	LRESULT OnEnableClearType(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CRegKey key;
		if (key.Create(HKEY_CURRENT_USER, WindowReg) == ERROR_SUCCESS) {
			if (enableClearType == FALSE)
				enableClearType = TRUE;
			else
				enableClearType = FALSE;
			key.SetDWORDValue(_T("Enable ClearType"), enableClearType);
			InvalidateRect(NULL);
		}
		return 0;
	}

	LRESULT OnOpaqueOnPoorResponse(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CRegKey key;
		if (key.Create(HKEY_CURRENT_USER, WindowReg) == ERROR_SUCCESS) {
			if (opaqueOnPoorResponse == FALSE)
				opaqueOnPoorResponse = TRUE;
			else
				opaqueOnPoorResponse = FALSE;
			key.SetDWORDValue(_T("Opaque On Poor Response"), opaqueOnPoorResponse);
			InvalidateRect(NULL);
		}
		return 0;
	}

	LRESULT OnTransparency(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		KillTimer(3);
		switch (wID) {
		case ID_TRANS_10:
			transparency = 10;
			break;
		case ID_TRANS_20:
			transparency = 20;
			break;
		case ID_TRANS_30:
			transparency = 30;
			break;
		case ID_TRANS_40:
			transparency = 40;
			break;
		case ID_TRANS_50:
			transparency = 50;
			break;
		case ID_TRANS_60:
			transparency = 60;
			break;
		case ID_TRANS_70:
			transparency = 70;
			break;
		case ID_TRANS_80:
			transparency = 80;
			break;
		case ID_TRANS_90:
			transparency = 90;
			break;
		case ID_TRANS_VISIBLE:
		default:
			transparency = 0;
			break;
		}

		CRegKey key;
		if (key.Create(HKEY_CURRENT_USER, WindowReg) == ERROR_SUCCESS) {
			key.SetDWORDValue(_T("Transparency"), transparency);
		}

		tempTrans = transparency;
		ChangeLayer(skinData.Background.TransparentColor, PercentToAlpha(transparency));

		return 0;
	}

	void SaveMonitor()
	{
		CRegKey key;
		CString kname = _T("Software\\Lore\\Wing\\Monitors\\");
		kname += m_desc;
		if (key.Create(HKEY_CURRENT_USER, kname) == ERROR_SUCCESS) {
			key.SetStringValue(_T("Host"), m_host);
			key.SetDWORDValue(_T("Hops"), m_hops);
			key.SetDWORDValue(_T("Frequency"), m_updateFreq);
			key.SetDWORDValue(_T("Graph Maximum"), m_graphMax);
		}
	}

	void RemoveUnusedMonitors()
	{
		std::vector<CString> used;

		CRegKey key;
		if (key.Create(HKEY_CURRENT_USER, _T("Software\\Lore\\Wing\\Windows")) != ERROR_SUCCESS)
			return;

		// get keys used by windows
		TCHAR kname[256];
		DWORD sz = 255;
		DWORD i = 0;
		while (key.EnumKey(i, kname, &sz) == ERROR_SUCCESS) {
			sz = 255;
			i++;
			CRegKey k;
			k.Open(key, kname);
			TCHAR descBuf[OPTIONS_DESCLEN + 1];
			descBuf[0] = _T('\0');
			ULONG cnt = OPTIONS_DESCLEN;
			k.QueryStringValue(_T("Monitor"), descBuf, &cnt);
			CString x = descBuf;
			x.MakeLower();
			used.insert(used.end(), x);
		}

		// get keys used by recent documents list
		if (key.Create(HKEY_CURRENT_USER, _T("Software\\Lore\\Wing\\Recent Document List")) == ERROR_SUCCESS) {
			for (int i = 0; i < 4; i++) {
				TCHAR descBuf[OPTIONS_DESCLEN + 1];
				descBuf[0] = _T('\0');
				ULONG cnt = OPTIONS_DESCLEN;
				cnt = OPTIONS_DESCLEN;
				CString name = _T("Document");
				name += TCHAR(i + _T('1'));
				key.QueryStringValue(name, descBuf, &cnt);
				CString x = descBuf;
				x.MakeLower();
				used.insert(used.end(), x);
			}
		}

		// now delete monitors not in the list
		if (key.Create(HKEY_CURRENT_USER, _T("Software\\Lore\\Wing\\Monitors")) != ERROR_SUCCESS)
			return;
		sz = 255;
		i = 0;
		std::sort<std::vector<CString>::iterator >(used.begin(), used.end());
		while (key.EnumKey(i, kname, &sz) == ERROR_SUCCESS) {
			sz = 255;
			i++;
			CString x = kname;
			x.MakeLower();
			if (std::find<std::vector<CString>::iterator >(used.begin(), used.end(), x) == used.end()) {
                key.RecurseDeleteKey(kname);
				//MessageBox(kname);
			}
		}
	}

	void LoadMonitor()
	{
		if (!m_desc.IsEmpty()) {
			CRegKey key;
			CString kname = _T("Software\\Lore\\Wing\\Monitors\\");
			kname += m_desc;
			if (key.Create(HKEY_CURRENT_USER, kname) == ERROR_SUCCESS) {
				TCHAR hostBuf[OPTIONS_HOSTLEN + 1];
				hostBuf[0] = _T('\0');
				ULONG cnt = OPTIONS_HOSTLEN;
				key.QueryStringValue(_T("Host"), hostBuf, &cnt);
				m_host = hostBuf;
				m_hops = OPTIONS_DEFAULT_HOPS;
				m_updateFreq = OPTIONS_DEFAULT_FREQ;
				m_graphMax = OPTIONS_GRAPH_DEFAULT;
				key.QueryDWORDValue(_T("Hops"), (DWORD&) m_hops);
				key.QueryDWORDValue(_T("Frequency"), (DWORD&) m_updateFreq);
				key.QueryDWORDValue(_T("Graph Maximum"), (DWORD&) m_graphMax);
			}
		}
	}

	void LoadSettings()
	{
		alwaysOnTop = TRUE;
		transparency = 0;
		lockPosition = FALSE;
		enableClearType = TRUE;
		opaqueOnPoorResponse = FALSE;
		position.x = 100;
		position.y = 100;
		skin.Empty();

		CRegKey key;
		if (key.Create(HKEY_CURRENT_USER, WindowReg) == ERROR_SUCCESS) {
			key.QueryDWORDValue(_T("Always On Top"), alwaysOnTop);
			key.QueryDWORDValue(_T("Lock Position"), lockPosition);
			key.QueryDWORDValue(_T("Opaque On Poor Response"), opaqueOnPoorResponse);
			key.QueryDWORDValue(_T("Transparency"), transparency);
			key.QueryDWORDValue(_T("Enable ClearType"), enableClearType);
			key.QueryDWORDValue(_T("Left"), (DWORD&)position.x);
			key.QueryDWORDValue(_T("Top"), (DWORD&)position.y);

			TCHAR descBuf[OPTIONS_DESCLEN + 1];
			descBuf[0] = _T('\0');
			ULONG cnt = OPTIONS_DESCLEN;
			key.QueryStringValue(_T("Monitor"), descBuf, &cnt);
			if (descBuf[0] != _T('\0'))
				m_desc = descBuf;

			TCHAR skinBuf[MAX_PATH];
			skinBuf[0] = _T('\0');
			cnt = 255;
			key.QueryStringValue(_T("Skin"), skinBuf, &cnt);
			if (skinBuf[0] != _T('\0'))
				skin = skinBuf;

			LoadMonitor();
		}
	}

	void GetSkinIni(CString& s)
	{
		CSkinData::GetSkinsDir(s);
		s += skin;
		s += _T(".ini");
	}

	void LoadSkin()
	{
		if (skin.IsEmpty())
			skinData.LoadDefaultSkin();
		else {
			CString s;
			GetSkinIni(s);
			if (::PathFileExists(s) == TRUE)
				skinData.Load(s);
			else
				skinData.LoadDefaultSkin();
		}
	}
};
