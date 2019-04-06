#pragma once

#ifndef __CCURLTASK_H__

	#define __CCURLTASK_H__

	class CCurlTask {
	public:
		CCurlTask();
		~CCurlTask();
		bool Exec(bool headonly, std::string url, std::string cookie = "", bool isPOST = false, std::string postdata = "");
		bool SetReferer(std::string & referer);
		char *GetResult();
		std::string & GetResultString();
		size_t GetLength();
	
	private:
		static size_t curl_receive(char *buffer, size_t size, size_t nmemb, std::string *stringclass);
		CURL* curl;
		std::string result;
	};

#endif