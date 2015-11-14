CC=g++
CFLAGS=-std=c++14 -fno-strict-aliasing
CFLAGS_LIB=-fPIC
CLIBS=-ldl
CLIBS_LIB=-shared
BIN=test



all:
	$(CC) $(CFLAGS) -c src/main.cpp -o src/main.o 
	$(CC) $(CFLAGS) -c src/interface.cpp -o src/interface.o 
	$(CC) $(CFLAGS) -c src/app_state.cpp -o src/app_state.o 
	$(CC) $(CLIBS) -o $(BIN) src/main.o src/interface.o src/app_state.o

	$(CC) $(CFLAGS) $(CFLAGS_LIB) `pkg-config --cflags gtk+-3.0` -c src/gui_gtk.cpp -o src/libgui_gtk.o
	$(CC) $(CLIBS_LIB) `pkg-config --libs gtk+-3.0` -o lib/libgui_gtk.so src/libgui_gtk.o

#	$(CC) $(CFLAGS) $(CFLAGS_LIB) -c src/gui_terminal.cpp -o src/libgui_terminal.o
#	$(CC) $(CLIBS_LIB) -o lib/libgui_terminal.so src/libgui_terminal.o

