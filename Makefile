CC = gcc
CFLAGS = -g
EXECUTABLE = mySA

$(EXECUTABLE): mySA.c mySA_functions.c mySA_functions.h
	$(CC) -o $(EXECUTABLE) mySA.c mySA_functions.c

debug: mySA.c mySA_functions.c mySA_functions.h
	$(CC) $(CFLAGS) -o $(EXECUTABLE) mySA.c mySA_functions.c

clean:
	rm -f $(EXECUTABLE)