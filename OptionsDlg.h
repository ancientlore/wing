// optionsdlg.h : interface of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#define OPTIONS_HOSTLEN 128
#define OPTIONS_DESCLEN 128
#define OPTIONS_MIN_HOPS 1
#define OPTIONS_MAX_HOPS 64
#define OPTIONS_DEFAULT_HOPS 32
#define OPTIONS_MIN_FREQ 500
#define OPTIONS_MAX_FREQ 3600000
#define OPTIONS_DEFAULT_FREQ 10000
#define OPTIONS_GRAPH_MIN 100
#define OPTIONS_GRAPH_MAX 10000
#define OPTIONS_GRAPH_DEFAULT 250

class COptionsDlg : public CDialogImpl<COptionsDlg>, public CWinDataExchange<COptionsDlg>
{
public:
	enum { IDD = IDD_OPTIONS };

	BEGIN_MSG_MAP(CAboutDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
	END_MSG_MAP()

	CString m_host;
	CString m_desc;
	INT m_hops;
	INT m_updateFreq;
	INT m_graphMax;

	BEGIN_DDX_MAP(COptionsDlg)
	DDX_TEXT_LEN(IDC_HOST, m_host, OPTIONS_HOSTLEN)
	DDX_TEXT_LEN(IDC_DESCRIPTION, m_desc, OPTIONS_DESCLEN)
	DDX_INT_RANGE(IDC_HOPS, m_hops, OPTIONS_MIN_HOPS, OPTIONS_MAX_HOPS)
	DDX_INT_RANGE(IDC_FREQUENCY, m_updateFreq, OPTIONS_MIN_FREQ, OPTIONS_MAX_FREQ)
	DDX_INT_RANGE(IDC_GRAPH_MAX, m_graphMax, OPTIONS_GRAPH_MIN, OPTIONS_GRAPH_MAX)
	END_DDX_MAP()

	COptionsDlg() : m_hops(OPTIONS_DEFAULT_HOPS), m_updateFreq(OPTIONS_DEFAULT_FREQ), m_graphMax(OPTIONS_GRAPH_DEFAULT) {}

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CenterWindow(GetParent());
		DoDataExchange(DDX_LOAD);
		return TRUE;
	}

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CString msg; //(_T('\0'), 512);
		CString caption;
		caption.LoadString(IDS_ERROR_CAPTION);

		if (wID == IDOK) {
			if (DoDataExchange(DDX_SAVE) == FALSE) {
				if (m_hops < OPTIONS_MIN_HOPS || m_hops > OPTIONS_MAX_HOPS) {
					msg.FormatMessage(IDS_BADHOPS, OPTIONS_MIN_HOPS, OPTIONS_MAX_HOPS);
					MessageBox(msg, caption);
					return 0;
				}
				if (m_updateFreq < OPTIONS_MIN_FREQ || m_updateFreq > OPTIONS_MAX_FREQ) {
					msg.FormatMessage(IDS_BADFREQ, OPTIONS_MIN_FREQ, OPTIONS_MAX_FREQ);
					MessageBox(msg, caption);
					return 0;
				}
				if (m_graphMax < OPTIONS_GRAPH_MIN || m_graphMax > OPTIONS_GRAPH_MAX) {
					msg.FormatMessage(IDS_BADGRAPHMAX, OPTIONS_GRAPH_MIN, OPTIONS_GRAPH_MAX);
					MessageBox(msg, caption);
					return 0;
				}
				if (m_host.GetLength() > OPTIONS_HOSTLEN) {
					msg.FormatMessage(IDS_BADHOST, OPTIONS_HOSTLEN);
					MessageBox(msg, caption);
					return 0;
				}
				if (m_desc.GetLength() > OPTIONS_DESCLEN) {
					msg.FormatMessage(IDS_BADDESC, OPTIONS_DESCLEN);
					MessageBox(msg, caption);
					return 0;
				}
				msg.LoadString(IDS_BADOPTIONS);
				MessageBox(msg, caption); // IDS_BADOPTIONS
				return 0;
			}
			m_host.TrimLeft();
			m_host.TrimRight();
			m_desc.TrimLeft();
			m_desc.TrimRight();
			if (m_host.IsEmpty()) {
				msg.LoadString(IDS_EMPTYHOST);
				MessageBox(msg, caption);
				return 0;
			}
			if (m_desc.IsEmpty())
				m_desc = m_host;

			if (m_desc.Find(_T('\\')) >= 0) {
				msg.LoadString(IDS_BADDESCCHAR);
				MessageBox(msg, caption);
				return 0;
			}
		}
		EndDialog(wID);
		return 0;
	}

};
