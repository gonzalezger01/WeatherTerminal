CFLAGS  = -g -Wall
weather: weather.o cJSON.o
	 gcc $(CFLAGS) -lcurl cJSON.o weather.o main.c -o weatherTerminal

weather.o: cJSON.o
	gcc $(CFLAGS)   weather.h weather.c -c


cJSON.o: cJSON.h cJSON.c
	gcc $(CFLAGS) -c cJSON.h cJSON.c


clean:
	rm  weather weatherTerminal Makefile~ *.c~ *.h~ *.gch *.o Weather*

