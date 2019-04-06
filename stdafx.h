// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <iostream>
#include <string>
#include "curl/curl.h"

extern "C"
{
	#ifdef _WIN64 // Windows amd64(x86-64) 平台

		#pragma comment(lib, "curl/libcurl_win64.lib")

	#else

		#ifdef _WIN32 // Windows x86 平台
			#pragma comment(lib, "curl/libcurl_win32.lib")
		#endif

	#endif
}

#define SERVER_URL "http://172.20.0.254:8080"
#define USER_NAME "telecomadmin"
#define USER_ENCRYPTED_PASSWORD "YWRtaW50ZWxlY29t"
