# makefile for client_auto and client_ver1.c and kachimake.c kinzite.c
# copy this command: mingw32-make -f makefile.mk
all: client client_auto
client: client_ver1.o kachimake.o kinzite.o defense.o offense.o
	gcc -o client client_ver1.o kachimake.o kinzite.o defense.o offense.o -lws2_32
client_ver1.o: client_ver1.c
	gcc -I include -c client_ver1.c -o client_ver1.o -fexec-charset=CP932
client_auto: client_auto.o kachimake.o kinzite.o defense.o offense.o
	gcc -o client_auto client_auto.o kachimake.o kinzite.o defense.o offense.o -lws2_32
client_auto.o: client_auto.c
	gcc -I include -c client_auto.c -o client_auto.o -fexec-charset=CP932
kachimake.o: kachimake.c
	gcc -I include -c kachimake.c -o kachimake.o -fexec-charset=CP932
kinzite.o: kinzite.c
	gcc -I include -c kinzite.c -o kinzite.o -fexec-charset=CP932
defense.o: defense.c
	gcc -I include -c defense.c -o defense.o -fexec-charset=CP932
offense.o: offense.c
	gcc -I include -c offense.c -o offense.o -fexec-charset=CP932
