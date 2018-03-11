#include "filter.h"

#include <cstdio>
#include <cstring>

static string content;
static string site_prefix;
static int cursor_position;

void SetPage(string *web_site, string *page_content){
	string :: size_type end = web_site -> find_last_of("/");
	site_prefix = string(*web_site, 0, end);
	
	content = *page_content;
	cursor_position = 0;
}

string *GetNextUrl(){
	string :: size_type begin, end;
	string *result = new string;		
	
	do{
		begin = content . find("<a href=\"", cursor_position) + strlen("<a href=\"");
		end = content . find("\"", begin);
		
		if (begin == string :: npos || end == string :: npos){
			delete result;
			return NULL;
		}
		
		if (result -> find("http://") != 0 && result -> find("https://") != 0)
			*result = site_prefix + *result;
	}while (result -> find(seed) == 0);
	
	cursor_position = end + 1;
	
	return result;
}
