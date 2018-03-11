#include "view_source.h"

#include <curl/curl.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

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
	if (strlen((char *)stream) + strlen((char *)ptr) > kMaxFileLength - 1)
		return 0;
	strcat(stream, (char *)ptr);
	return size * nmemb;
}

char *Download(char *filename){
	static char buffer[kMaxFileLength];
	memset(buffer, 0, sizeof(char) * strlen(buffer));
	
	curl_easy_setopt(curl_handle, CUROPT_URL, filename);
	curl_easy_setopt(curl_handle, CUROPT_WRITEFUNCTION, &write_data);
	curl_easy_setopt(curl_handle, CUROPT_WRITEDATA, buffer);
	
	return_code = curl_easy_perform(curl);
	if (return_code != CURLE_OK)
		return NULL;
	return buffer;
}

void CloseViewer(){
	curl_easy_cleanup(curl_handle);
	curl_global_cleanup();
}
