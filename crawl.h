#ifndef CRAWL_H
#define CRAWL_H

#include <string>

using namespace std;

#ifdef CRAWL_CPP
const string seed = string("http://grs.ruc.edu.cn/third/Professor_emeritus.html");//string("http://www.ruc.edu.cn");
#endif

void crawl();

#endif
