CC = g++

# all : .out customer.out supplier.out

restaurant.out :  restaurant.c
	$(CC) restaurant.c -o restaurant.out


customer.out : customer.c json.h
	$(CC) customer.c -o customer.out

supplier.out : supplier.c 
	$(CC) supplier.c -o supplier.out

a.out : cJSON.h cJSON.c temp.c
	$(CC) temp.c -o a.out


