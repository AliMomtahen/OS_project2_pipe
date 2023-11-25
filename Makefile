CC = g++ -std=c++11

all : building.out master.out office.out Elec.out  Gas.out Water.out 

master.out : master.cpp function.hpp
	$(CC) master.cpp -o master.out

building.out :  building.cpp function.hpp
	$(CC) building.cpp -o building.out

office.out :  office.cpp function.hpp
	$(CC) office.cpp -o office.out

Elec.out :  Elec.cpp function.hpp
	$(CC) Elec.cpp -o Elec.out


Gas.out :  Gas.cpp function.hpp
	$(CC) Gas.cpp -o Gas.out

Water.out :  Water.cpp function.hpp
	$(CC) Water.cpp -o Water.out



