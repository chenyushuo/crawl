#include "view_source.h"

#include <curl/curl.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <iostream>

#include <string>

using namespace std;

const static int kMaxFileLength = 10000000;

static CURL *curl_handle;
static CURLcode return_code;

void InitViewer(){
	return_code = curl_global_init(CURL_GLOBAL_ALL);
	if (return_code != CURLE_OK){
		fprintf(stderr, "global_init failed!\n");
		exit(1);
	}
	
	curl_handle = curl_easy_init();
	if (curl_handle == NULL){
		fprintf(stderr, "get a handle failed!\n");
		curl_global_cleanup();
		exit(2);
	}
}

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream){
	if (strlen((char *)stream) + size * nmemb > kMaxFileLength - 1) return 0;
	strncat((char *)stream, (char *)ptr, size * nmemb);
	return size * nmemb;
}

static void SaveFile(const char *web_site, char *buffer){
	string *site = new string(web_site);
	string :: size_type begin = site -> find("://", 0) + strlen("://");
	string :: size_type end = site -> find_last_of("/");
	FILE *fp;
	string *directory, *file_name;
	if (begin != string :: npos){
		if (end == string :: npos || begin >= end){
			directory = new string(site -> substr(begin));
			file_name = new string("index.html");
		}
		else{
			directory = new string(site -> substr(begin, end - begin));
			file_name = new string(site -> substr(end));
			if (file_name -> find(".") == string :: npos)
				*file_name = *file_name + ".html";
		}
		
		system(("mkdir -p '/media/workspace/download_from_info/" + *directory + "'") . c_str());		
		fp = fopen(("/media/workspace/download_from_info/" + *directory + "/" + *file_name) . c_str(), "wb");
		
		fwrite(buffer, sizeof(char), strlen(buffer), fp);
		
		fclose(fp);
		delete directory;
		delete file_name;
	}
	delete site;
}

char *Download(const char *web_site){
	static char buffer[kMaxFileLength];
	memset(buffer, 0, sizeof(char) * strlen(buffer));
	
	curl_easy_setopt(curl_handle, CURLOPT_URL, web_site);
	curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, 3);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, &write_data);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, buffer);
	
	return_code = curl_easy_perform(curl_handle);
	if (return_code != CURLE_OK){
		fprintf(stderr, "%s curl_easy_perform() failed: %s\n",
			web_site,
			curl_easy_strerror(return_code));
		return NULL;
	}
	
	SaveFile(web_site, buffer);
	
	return buffer;
}

void CloseViewer(){
	curl_easy_cleanup(curl_handle);
	curl_global_cleanup();
}
