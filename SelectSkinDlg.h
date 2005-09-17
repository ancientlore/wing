// selectskindlg.h : interface of the CSelectSkinDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CSelectSkinDlg : public CDialogImpl<CSelectSkinDlg>, public CWinDataExchange<CSelectSkinDlg>
{
	CListBox m_list;
	CSampleSkin m_ctrl;

public:
	enum { IDD = IDD_SELECTSKIN };

	BEGIN_MSG_MAP(CAboutDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		COMMAND_HANDLER(IDC_SKINLIST, LBN_SELCHANGE, OnLbnSelchangeSkinlist)
//		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	CString m_skin;

//	BEGIN_DDX_MAP(CSelectSkinDlg)
//	DDX_TEXT(IDC_SKINLIST, m_skin)
//	END_DDX_MAP()

	CSelectSkinDlg() {}

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CenterWindow(GetParent());
		m_ctrl.SubclassWindow(GetDlgItem(IDC_SKINSAMPLE));
		m_list.Attach(GetDlgItem(IDC_SKINLIST));
		LoadList();
		if (m_skin.IsEmpty() || m_list.FindString(0, m_skin) < 0)
			m_skin = _T("_default");
		m_list.SelectString(-1, m_skin);
		
		UpdateSkin();
//		DoDataExchange(DDX_LOAD);
		return TRUE;
	}

	void LoadList()
	{
		m_list.AddString(_T("_default"));
		CString skinDir;
		CSkinData::GetSkinsDir(skinDir);
		skinDir += _T("\\*.ini");
		WIN32_FIND_DATA data;
		HANDLE hFind;
		hFind = FindFirstFile(skinDir, &data);
		if (hFind != INVALID_HANDLE_VALUE) {
			BOOL ret = TRUE;
			while (ret == TRUE) {
				// add to list
				CString s = data.cFileName;
				m_list.AddString(s.Left(s.GetLength() - 4));
				// get next file
				ret = FindNextFile (hFind, &data);
			}
		}
        FindClose (hFind);
	}

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CString msg; //(_T('\0'), 512);
		CString caption;
		caption.LoadString(IDS_ERROR_CAPTION);

		if (wID == IDOK) {
//			if (DoDataExchange(DDX_SAVE) == FALSE) {
//				MessageBox(msg, caption);
//				return 0;
//			}
			m_list.GetText(m_list.GetCurSel(), m_skin);
			if (m_skin == _T("_default"))
				m_skin.Empty();
		}

		EndDialog(wID);
		return 0;
	}

	void UpdateSkin()
	{
		CString s;
		m_list.GetText(m_list.GetCurSel(), s);
		m_ctrl.ChangeSkin(s);
		SetDlgItemText(IDC_CREDITS, m_ctrl.m_skinData.Credits);
		SetDlgItemText(IDC_NOTES, m_ctrl.m_skinData.Notes);
	}

	LRESULT OnLbnSelchangeSkinlist(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		UpdateSkin();
		return 0;
	}
};
