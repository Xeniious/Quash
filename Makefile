GP := g++
CC := -std=c++11
CLAGS := -Wall -g

Quash: Main.cpp test.h
	$(GP) $(CC) $(CFLAGS) -o $@ $^