#ifndef FILTER_H
#define FILTER_H

#include <cstring>

const string seed = string("http://info.ruc.edu.cn");

void SetPage(string *web_site, string *page_content);
void GetNextUrl();

#endif
