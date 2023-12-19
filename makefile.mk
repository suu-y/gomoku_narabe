# makefile for client_auto2, client_ver2.c, kachimake.c, kinzite.c, defense.c, and offense.c
# copy this command: mingw32-make -f makefile.mk
all: client client_auto2
client: client_ver2.o kachimake.o kinzite.o defense.o offense.o
	gcc -o client client_ver2.o kachimake.o kinzite.o defense.o offense.o -lws2_32
client_ver2.o: client_ver2.c
	gcc -I include -c client_ver2.c -o client_ver2.o -fexec-charset=CP932
client_auto2: client_auto2.o kachimake.o kinzite.o defense.o offense.o
	gcc -o client_auto2 client_auto2.o kachimake.o kinzite.o defense.o offense.o -lws2_32
client_auto2.o: client_auto2.c
	gcc -I include -c client_auto2.c -o client_auto2.o -fexec-charset=CP932
kachimake.o: kachimake.c
	gcc -I include -c kachimake.c -o kachimake.o -fexec-charset=CP932
kinzite.o: kinzite.c
	gcc -I include -c kinzite.c -o kinzite.o -fexec-charset=CP932
defense.o: defense.c
	gcc -I include -c defense.c -o defense.o -fexec-charset=CP932
offense.o: offense.c
	gcc -I include -c offense.c -o offense.o -fexec-charset=CP932
