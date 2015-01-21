CC = gcc
CFLAGS = -g -Wall
OBJECTS = *.c
TODELETE = *.o
NAME = RecursiveDescent

$(NAME) : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME)

.PHONY: clean
clean:
	rm -f $(NAME) $(TODELETE)