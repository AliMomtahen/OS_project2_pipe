CC = g++

all : building.out master.out 

master.out : master.cpp 
	$(CC) master.cpp -o master.out

building.out :  building.cpp
	$(CC) building.cpp -o building.out





