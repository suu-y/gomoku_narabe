# makefile for client_stdio.c and kinzite.c
client: client_stdio.o kinzite.o
	gcc -o client client_stdio.o kinzite.o -lws2_32
client_stdio.o: client_stdio.c
	gcc -I include -c client_stdio.c -o client_stdio.o -fexec-charset=CP932
kinzite.o: kinzite.c
	gcc -I include -c kinzite.c -o kinzite.o -fexec-charset=CP932