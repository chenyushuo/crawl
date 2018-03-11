#include "crawl.h"

#include <cstdio>
#include <cstring>

#include <queue>
#include <set>

#include "filter.h"
#include "view_source.h"

queue<string> *queue_of_web_site;
set<string> *set_of_web_site;

void crawl(){
	queue_of_web_site = new queue<string>;
	set_of_web_site = new set<string>;
	while (!queue_of_web_site -> empty())
		queue_of_web_site -> pop();
		
	queue_of_web_site -> push(seed);
	set_of_web_site -> insert(seed);
	
	InitViewer();
	
	while (!queue_of_web_site -> empty()){
		string *web_site = new string(queue_of_web_site -> front());
		string *page_content = new string(*Download(web_site -> c_str()));
		SetPage(web_site, page_content);
		
		string *next_url;		
		while ((next_url = GetNextUrl()) != NULL){
			if (set_of_web_site -> count(*next_url) == 0){
				queue_of_web_site -> push(next_url);
				set_of_web_site -> insert(next_url);
			}
			delete next_url;
		}
		
		delete web_site;
		delete page_content;
	}
	
	CloseViewer();
}
