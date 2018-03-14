run : main
	./main
	
objects = main.o crawl.o filter.o view_source.o

main : $(objects)
	g++ -o main $(objects) -Wall -lcurl
	
main.o : main.cpp crawl.h
	g++ -c main.cpp -Wall
	
crawl.o : crawl.cpp crawl.h filter.h view_source.h
	g++ -c crawl.cpp -Wall
	
filter.o : filter.cpp filter.h
	g++ -c filter.cpp -Wall
	
view_source.o : view_source.cpp view_source.h
	g++ -c view_source.cpp -Wall -lcurl
