#include "filter.h"

#include <cstdio>
#include <cstring>

#include <iostream>

#include <string>

using namespace std;

static string content;
static string site_prefix;
static string site_root;
static int cursor_position;
static bool web_site_error = false;

void SetPage(string *web_site, string *page_content){
	string :: size_type begin = web_site -> find("://");
	string :: size_type end = web_site -> find_last_of("/");
	if (begin == string :: npos){
		FILE *fp = fopen("error.log", "a+");
		fprintf(fp, "%s is illegal!\n", web_site -> c_str());
		fclose(fp);
		web_site_error = true;
		return;
	}
	else
		web_site_error = false;
	
	begin += strlen("://");
	if (begin < end && end != string :: npos){
		site_prefix = string(*web_site, 0, end + 1);
		string :: size_type root = web_site -> find("/", begin);
		site_root = string(*web_site, 0, root);
	}
	else{
		site_prefix = *web_site + "/";
		site_root = *web_site;
	}
	
	content = *page_content;
	cursor_position = 0;
	
	//cerr << site_prefix << endl;
}

static bool ValidUrl(string *result){
	if (result -> find("http") != 0)
		return false;
	if (result -> find("://") == string :: npos)
		return false;
	if (result -> find(key_word) == string :: npos)
		return false;
	for (const char **ptr = forbidden_string; strcmp(*ptr, "END"); ptr ++){
		if (result -> find(*ptr) != string :: npos)
			return false;
	}
	return true;
}

static void DeleteRelativePath(string *web_site){
	if (web_site -> at(web_site -> length() - 1) != '/')
		web_site -> append("/");
	
	string :: size_type head = web_site -> find("://") + strlen("://");
	string :: size_type last = head, begin = head, end = web_site -> find('/', head);
	
	string *result = new string(*web_site, 0, head);
	string section;
	
	do{
		section = web_site -> substr(begin, end - begin + 1);
		if (section == "./"){
			
		}
		else if (section == "../"){
			if (last != head){
				result -> erase(last);
				last = result -> find('/', result -> length() - 2) + 1;
			}
		}
		else{
			last = result -> length();
			result -> append(section);
		}
		
		begin = end + 1;
		end = web_site -> find('/', begin);
	}while (end != string :: npos);
	
	if (result -> at(result -> length() - 1) == '/')
		result -> erase(result -> length() - 1);
		
	*web_site = *result;
	delete result;
}

static string :: size_type GetSpaceChar(string *web_site){
	string :: size_type ptr = string :: npos;
	for (int i = 0; space_char[i]; i ++)
		ptr = min(ptr, web_site -> find(space_char[i]));
	return ptr;
}

static void EraseSpaceChar(string *web_site){
	string :: size_type ptr;
	while ((ptr = GetSpaceChar(web_site)) != string :: npos)
		web_site -> erase(ptr, 1);
}

static void DeleteArgv(string *web_site){
	string :: size_type begin, end, equal;
	begin = web_site -> find("?");
	if (begin == string :: npos)
		return;
	
	string *result = new string(*web_site, 0, begin);
	do{
		begin ++;
		end = web_site -> find("&", begin);
		equal = web_site -> find("=", begin);
		if (end == string :: npos)
			end = web_site -> length();
		
		string key_name = web_site -> substr(begin, equal - begin);
		string the_whole_key = web_site -> substr(begin , end - begin);
		string :: size_type result_begin = result -> find(key_name);
		
		if (result_begin != string :: npos){
			string :: size_type result_end = result -> find("&", result_begin);
			if (result_end == string :: npos)
				result_end = result -> length();
			result -> replace(result_begin, result_end - result_begin, the_whole_key);
		}
		else{
			result -> append(web_site -> substr(begin - 1, 1));
			result -> append(the_whole_key);
		}
		
		begin = end;
	}while (begin != web_site -> length());
	
	*web_site = *result;
	delete result;
}

static void NormalizeWebSite(string *web_site){
	DeleteRelativePath(web_site);		
	EraseSpaceChar(web_site);
	DeleteArgv(web_site);
}

string *GetNextUrl(){
	if (web_site_error)
		return NULL;
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
		
		if (result -> find("http://") == string :: npos && result -> find("https://") == string :: npos){
			if (result -> length() == 0 || result -> at(0) == '/')
				*result = site_root + *result;
			else
				*result = site_prefix + *result;
		}
			
		cursor_position = end + 1;
	}while (ValidUrl(result) == false);
	
	NormalizeWebSite(result);
	
	return result;
}
