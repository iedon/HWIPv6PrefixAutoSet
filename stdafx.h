// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
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
	#ifdef _WIN64 // Windows amd64(x86-64) ƽ̨

		#pragma comment(lib, "curl/libcurl_win64.lib")

	#else

		#ifdef _WIN32 // Windows x86 ƽ̨
			#pragma comment(lib, "curl/libcurl_win32.lib")
		#endif

	#endif
}

#define SERVER_URL "http://172.20.0.254:8080"
#define USER_NAME "telecomadmin"
#define USER_ENCRYPTED_PASSWORD "YWRtaW50ZWxlY29t"
