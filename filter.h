#ifndef FILTER_H
#define FILTER_H

#include <string>

using namespace std;

#ifdef FILTER_CPP
const string key_word = string("ruc");
const char *forbidden_string[] = {"@", "#", "(", ")", "[", "]", "{", "}",
					".txt", ".doc", ".ppt", ".xls", ".pdf", ".PDF", "ftp://", "file://", "telnet://",
					".jpg", ".png", ".gif", ".bmp", ".rar", ".zip", ".7z", ".mp3", ".mp4", ".MP4",
					"javascript:void(0)", "javascript:;",
					".flv", ".wmv", ".avi", ".rmvb", ".mpeg",
					"twitter", "google", "facebook", "END"};
const char space_char[] = "\t" "\f" "\v" "\n" "\r" " ";
#endif

void SetPage(std :: string *web_site, std :: string *page_content);
string *GetNextUrl();

#endif
