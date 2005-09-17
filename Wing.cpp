// Wing.cpp : main source file for Wing.exe
//

#include "stdafx.h"

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>

#include "resource.h"
#include "LayeredWindow.h"

volatile bool gTimeToQuit = false;

typedef struct {
	UCHAR Ttl;
	UCHAR Tos;
	UCHAR Flags;
	UCHAR OptionsSize;
	PUCHAR OptionsData;
} IP_OPTION_INFORMATION, *PIP_OPTION_INFORMATION;

typedef struct {
	ULONG Address;
	ULONG Status;
	ULONG RoundTripTime;
	USHORT DataSize;
	USHORT Reserved;
	PVOID Data;
	IP_OPTION_INFORMATION Options;
} ICMPECHO;

HMODULE hIcmpDll;
HANDLE (WINAPI *lpfnIcmpCreateFile)();
BOOL (WINAPI *lpfnIcmpCloseHandle)(HANDLE);
DWORD (WINAPI *lpfnIcmpSendEcho2)(HANDLE, HANDLE, FARPROC, PVOID, ULONG, LPVOID, WORD, PIP_OPTION_INFORMATION, LPVOID, DWORD, DWORD);
DWORD (WINAPI *lpfnIcmpParseReplies)(LPVOID, DWORD);

#include "aboutdlg.h"
#include "optionsdlg.h"
#include "SkinData.h"
#include "sampleskin.h"
#include "selectskindlg.h"
#include "MainFrm.h"

CAppModule _Module;

class CWingThreadManager
{
public:
	// thread init param
	struct _RunData
	{
		LPTSTR lpstrCmdLine;
		int nCmdShow;
		TCHAR WindowID[255];
	};

	// thread proc
	static DWORD WINAPI RunThread(LPVOID lpData)
	{
		CMessageLoop theLoop;
		_Module.AddMessageLoop(&theLoop);

		_RunData* pData = (_RunData*)lpData;
		CMainFrame wndFrame;

		CString s = pData->WindowID;
		s.TrimLeft();
		s.TrimRight();
		if (!s.IsEmpty())
			wndFrame.WindowID = s;

		//if(wndFrame.CreateEx() == NULL)
		if(wndFrame.CreateEx(NULL, NULL, WS_POPUP, WS_EX_LAYERED | WS_EX_NOACTIVATE) == NULL)
		{
			ATLTRACE(_T("Frame window creation failed!\n"));
			return 0;
		}

		wndFrame.ShowWindow(pData->nCmdShow);
		::SetForegroundWindow(wndFrame);	// Win95 needs this
		delete pData;

		int nRet = theLoop.Run();

		_Module.RemoveMessageLoop();
		return nRet;
	}

	DWORD m_dwCount;
	HANDLE m_arrThreadHandles[MAXIMUM_WAIT_OBJECTS - 1];

	CWingThreadManager() : m_dwCount(0)
	{ }

// Operations
	DWORD AddThread(LPTSTR lpstrCmdLine, int nCmdShow, LPTSTR winID = NULL)
	{
		if(m_dwCount == (MAXIMUM_WAIT_OBJECTS - 1))
		{
			::MessageBox(NULL, _T("ERROR: Cannot create ANY MORE threads!!!"), _T("Wing"), MB_OK);
			return 0;
		}

		_RunData* pData = new _RunData;
		pData->lpstrCmdLine = lpstrCmdLine;
		pData->nCmdShow = nCmdShow;
		if (winID != NULL)
			_tcscpy(pData->WindowID, winID);
		else
			pData->WindowID[0] = _T('\0');
		DWORD dwThreadID;
		HANDLE hThread = ::CreateThread(NULL, 0, RunThread, pData, 0, &dwThreadID);
		if(hThread == NULL)
		{
			::MessageBox(NULL, _T("ERROR: Cannot create thread!!!"), _T("Wing"), MB_OK);
			return 0;
		}

		m_arrThreadHandles[m_dwCount] = hThread;
		m_dwCount++;
		return dwThreadID;
	}

	void RemoveThread(DWORD dwIndex)
	{
		::CloseHandle(m_arrThreadHandles[dwIndex]);
		if(dwIndex != (m_dwCount - 1))
			m_arrThreadHandles[dwIndex] = m_arrThreadHandles[m_dwCount - 1];
		m_dwCount--;
	}

	int Run(LPTSTR lpstrCmdLine, int nCmdShow)
	{
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {
			::MessageBox(NULL, _T("ERROR: Could not initialize Windows Sockets"), _T("Error"), MB_OK);
			return 0;
		}

		hIcmpDll = LoadLibrary(_T("ICMP.DLL"));
		if (hIcmpDll == INVALID_HANDLE_VALUE) {
			::MessageBox(NULL, _T("ERROR: Unable to load ICMP DLL"), _T("Error"), MB_OK);
			WSACleanup();
			return 0;
		}

		lpfnIcmpCreateFile = (HANDLE (WINAPI *)()) GetProcAddress(hIcmpDll, "IcmpCreateFile");
		lpfnIcmpCloseHandle = (BOOL (WINAPI *)(HANDLE)) GetProcAddress(hIcmpDll, "IcmpCloseHandle");
		lpfnIcmpSendEcho2 = (DWORD (WINAPI *)(HANDLE, HANDLE, FARPROC, PVOID, ULONG, LPVOID, WORD, PIP_OPTION_INFORMATION, LPVOID, DWORD, DWORD)) GetProcAddress(hIcmpDll, "IcmpSendEcho2");
		lpfnIcmpParseReplies = (DWORD (WINAPI *)(LPVOID, DWORD)) GetProcAddress(hIcmpDll, "IcmpParseReplies");

		if (!lpfnIcmpCreateFile || !lpfnIcmpCloseHandle) {
			::MessageBox(NULL, _T("ERROR: Unable to find ICMP functions"), _T("Error"), MB_OK);
			FreeLibrary(hIcmpDll);
			WSACleanup();
			return 0;
		}

		MSG msg;
		// force message queue to be created
		::PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);

		CRegKey key;
		if (key.Create(HKEY_CURRENT_USER, _T("Software\\Lore\\Wing\\Windows")) == ERROR_SUCCESS) {
			TCHAR kname[256];
			DWORD sz = 255;
			DWORD i = 0;
			while (key.EnumKey(i, kname, &sz) == ERROR_SUCCESS) {
				AddThread(lpstrCmdLine, nCmdShow, kname);
				sz = 255;
				i++;
			}
			if (i == 0)
				AddThread(lpstrCmdLine, nCmdShow);
		}
		else
			AddThread(lpstrCmdLine, nCmdShow);

		int nRet = m_dwCount;
		DWORD dwRet;
		while(m_dwCount > 0)
		{
			dwRet = ::MsgWaitForMultipleObjects(m_dwCount, m_arrThreadHandles, FALSE, INFINITE, QS_ALLINPUT);

			if(dwRet == 0xFFFFFFFF)
			{
				::MessageBox(NULL, _T("ERROR: Wait for multiple objects failed!!!"), _T("Wing"), MB_OK);
			}
			else if(dwRet >= WAIT_OBJECT_0 && dwRet <= (WAIT_OBJECT_0 + m_dwCount - 1))
			{
				RemoveThread(dwRet - WAIT_OBJECT_0);
			}
			else if(dwRet == (WAIT_OBJECT_0 + m_dwCount))
			{
				if(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{
					if(msg.message == WM_USER)
						AddThread(lpstrCmdLine, SW_SHOWNORMAL);
				}
			}
			else
			{
				::MessageBeep((UINT)-1);
			}
		}

		FreeLibrary(hIcmpDll);
		WSACleanup();

		return nRet;
	}
};

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	int nRet = 0;
	// BLOCK: Run application
	{
		CWingThreadManager mgr;
		nRet = mgr.Run(lpstrCmdLine, nCmdShow);
	}

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
