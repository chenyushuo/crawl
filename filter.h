#ifndef FILTER_H
#define FILTER_H

#include <string>

using namespace std;

const string seed = string("http://info.ruc.edu.cn");

void SetPage(std :: string *web_site, std :: string *page_content);
string *GetNextUrl();

#endif
