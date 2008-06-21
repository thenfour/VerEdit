// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once


#include <iostream>
#include <tchar.h>

// TODO: reference additional headers your program requires here
#import "libid:D63E0CE2-A0A2-11D0-9C02-00C04FC99C8E"// MSXML

#include "..\libcc\libcc\StringUtil.h"
#include "..\libcc\libcc\Result.h"
#include "..\libcc\libcc\Blob.h"
#include "..\libcc\libcc\Winapi.h"
#include <iostream>
#include <string>
#include <list>
#include <vector>

#include <windows.h>
#include <shlwapi.h>

using namespace std;
using namespace LibCC;

#pragma comment(lib, "version.lib")
#pragma comment(lib, "shlwapi.lib")
