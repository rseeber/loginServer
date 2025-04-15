main: util.cpp
	g++ main.cpp util.cpp -o bin/main

clean:
	gio trash bin/main