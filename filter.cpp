#include "filter.h"

#include <cstdio>
#include <cstring>

#include <iostream>

#include <string>

using namespace std;

static string content;
static string site_prefix;
static int cursor_position;

void SetPage(string *web_site, string *page_content){
	string :: size_type end = web_site -> find_last_of("/");
	if (web_site -> find("://") + strlen("://") < end && end != string :: npos)
		site_prefix = string(*web_site, 0, end + 1);
	else
		site_prefix = *web_site + "/";
	
	content = *page_content;
	cursor_position = 0;
	
	//cerr << site_prefix << endl;
}

string *GetNextUrl(){
	string :: size_type begin, end;
	string *result = new string;
	
	do{
		if (cursor_position >= (int)content . length()){
			delete result;
			return NULL;
		}
				
		begin = content . find("<a", cursor_position);
		if (begin == string :: npos){
			delete result;
			return NULL;
		}
		begin = content . find("href", begin);
		if (begin == string :: npos){
			delete result;
			return NULL;
		}
		begin = min(content . find("'", begin), content . find("\"", begin));
		if (begin == string :: npos){
			delete result;
			return NULL;
		}
		begin ++;
		
		end = min(content . find("'", begin), content . find("\"", begin));
		if (end == string :: npos){
			delete result;
			return NULL;
		}
		
		//cerr << begin << " " << end << " " << cursor_position << " " << content . length() << endl;
				
		*result = content . substr(begin, end - begin);
		
		if (result -> find("http://") == string :: npos && result -> find("https://") == string :: npos)
			*result = site_prefix + *result;
			
		cursor_position = end + 1;
	}while (result -> find(seed) == string :: npos);	
	return result;
}
