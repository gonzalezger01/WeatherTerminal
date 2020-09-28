CFLAGS  = -g -Wall
weather: weather.o
	 gcc $(CFLAGS) main.c -o weatherTerminal

weather.o: cJSON.o
	gcc $(CFLAGS) -lcurl  weather.h weather.c -c


cJSON.o: cJSON.h cJSON.c
	gcc $(CFLAGS) -c cJSON.h cJSON.c


clean:
	rm  weather weatherTerminal Makefile~ *.c~ *.h~ *.gch *.o Weather*

