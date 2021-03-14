CFLAGS  = -g -Wall

weather: cJSON.o weather.o constants.o utils.o
	clang -lcurl -lcmocka cJSON.o weather.o constants.o utils.o main.c -o weatherTerminal -O3

weather.o: cJSON.o
	clang $(CFLAGS)   weather.h weather.c -c


cJSON.o: cJSON.h cJSON.c
	clang $(CFLAGS) -c cJSON.h cJSON.c

constants.o: constants.h constants.c
	clang $(CFLAGS) -c constants.h constants.c

utils.o: utils.h utils.c
	clang $(CFLAGS) -c utils.h utils.c

debug: cJSON.o weather.o  constants.o utils.o
	clang $(CFLAGS) -lcurl -lcmocka cJSON.o weather.o constants.o utils.o main.c -o weatherTerminal

clean:
	rm  weather weatherTerminal Makefile~ *.c~ *.h~ *.gch *.o Weather*

test: weather.o cJSON.o constants.o
	 clang $(CFLAGS) -lcurl -lcmocka cJSON.o weather.o constants.o utils.o main.c -o weatherTerminal
