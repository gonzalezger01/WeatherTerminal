CFLAGS  = -g -Wall

weather: cJSON.o weather.o constants.o utils.o
	 gcc -lcurl cJSON.o weather.o main.c constants.o utils.o -o weatherTerminal -O3

weather.o: cJSON.o
	gcc $(CFLAGS)   weather.h weather.c -c


cJSON.o: cJSON.h cJSON.c
	gcc $(CFLAGS) -c cJSON.h cJSON.c

constants.o: constants.h constants.c
	gcc $(CFLAGS) -c constants.h constants.c

utils.o: utils.h utils.c
	gcc $(CFLAGS) -c utils.h utils.c

debug: cJSON.o weather.o  constants.o utils.o
	 gcc $(CFLAGS) -lcurl cJSON.o weather.o constants.o utils.o main.c -o weatherTerminal

clean:
	rm  weather weatherTerminal Makefile~ *.c~ *.h~ *.gch *.o Weather*

test: weather.o cJSON.o constants.o
	 gcc $(CFLAGS) -lcurl cJSON.o weather.o constants.o main.c -o weatherTerminal
