CC = gcc -g
objects = main.o backupfiles.o changepermissions.o date.o files.o logs.o watchingfiles.o
headers = backupfiles.h changepermissions.h date.h files.h watchingfiles.h logs.h

myprog: $(objects)
	$(CC) -o assignment_daemon $(objects) -lm -lrt

main.o: main.c $(headers)
	$(CC) -c -Wall main.c -lrt

backupfiles.o: backupfiles.c
	$(CC) -c -Wall backupfiles.c

changepermissions.o: changepermissions.c
	$(CC) -c -Wall changepermissions.c

date.o: date.c
	$(CC) -c -Wall date.c

files.o: files.c
	$(CC) -c -Wall files.c

logs.o: logs.c
	$(CC) -c -Wall logs.c

watchingfiles.o: watchingfiles.c
	$(CC) -c -Wall watchingfiles.c

clean:
	rm assignment_daemon $(objects)

