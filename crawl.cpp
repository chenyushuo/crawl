#include "crawl.h"

#include <cstdio>
#include <cstring>

#include <iostream>

#include <string>
#include <queue>
#include <set>

#include "filter.h"
#include "view_source.h"

using namespace std;

queue<string> *queue_of_web_site;
set<string> *set_of_web_site;

void crawl(){
	queue_of_web_site = new queue<string>;
	set_of_web_site = new set<string>;
	while (!queue_of_web_site -> empty())
		queue_of_web_site -> pop();
	set_of_web_site -> clear();
		
	queue_of_web_site -> push(seed);
	set_of_web_site -> insert(seed);
	
	InitViewer();
	
	//int times = 0;
	
	while (!queue_of_web_site -> empty()){
		//if (++ times == 10)
		//	break;
		string *web_site = new string(queue_of_web_site -> front());
		queue_of_web_site -> pop();
		
		char *buffer = Download(web_site -> c_str());
		if (buffer == NULL){
			delete web_site;
			continue;
		}
		string *page_content = new string(buffer);
		SetPage(web_site, page_content);
		
		cerr << *web_site << " catched!" << endl;
		
		string *next_url;
		while ((next_url = GetNextUrl()) != NULL){
			//cerr << "next_url = " << *next_url << endl;
			if (set_of_web_site -> count(*next_url) == 0){
				queue_of_web_site -> push(*next_url);
				set_of_web_site -> insert(*next_url);
			}
			delete next_url;
		}
		
		cerr << "end of this file" << endl;
		/*for (int i = 1; i <= 2000000000; i ++)
			;*/
		
		delete web_site;
		delete page_content;
	}
	
	CloseViewer();
}
