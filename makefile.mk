# makefile for client_ver1.c and kachimake.c
client: client_ver1.o kachimake.o
	gcc -o client client_ver1.o kachimake.o -lws2_32
client_ver1.o: client_ver1.c
	gcc -I include -c client_ver1.c -o client_ver1.o -fexec-charset=CP932
kachimake.o: kachimake.c
	gcc -I include -c kachimake.c -o kachimake.o -fexec-charset=CP932