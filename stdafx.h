// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#pragma once

// Change these values to use different versions
#define WINVER		0x0500
#define _WIN32_WINNT	0x0500
#define _WIN32_IE	0x0501
#define _RICHEDIT_VER	0x0200

#include <vector>
#include <algorithm>

#include <atlbase.h>
#include <atlapp.h>

extern CAppModule _Module;

#include <atlwin.h>

#include <atlmisc.h>	// CString
#include <atlddx.h>
#include <atldlgs.h>

#include <winsock2.h>
#include <deque>
