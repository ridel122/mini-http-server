CC = gcc

CFLAGS = -Wall -Wextra

TARGET = server

OBJ = main.o core_network.o http_parser.o response_builder.o

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

main.o: main.c core_network.h response_builder.h
	$(CC) $(CFLAGS) -c main.c

core_network.o: core_network.c core_network.h
	$(CC) $(CFLAGS) -c core_network.c

http_parser.o: http_parser.c http_parser.h
	$(CC) $(CFLAGS) -c http_parser.c

response_builder.o: response_builder.c response_builder.h http_parser.h
	$(CC) $(CFLAGS) -c response_builder.c

clean:
	rm -f *.o $(TARGET)

run:
	./$(TARGET) 8080
