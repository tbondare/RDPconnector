NAME = RDPconnector

C = gcc
CFLAG = -Wall -Wextra -Werror

SRC = main.c server.c client.c
OBJ = main.o server.o client.o
INC = client.h server.h wrap_unwrap.h

all:
	$(C) $(CFLAG) -c $(SRC)
	$(C) $(CFLAG) -o $(NAME) $(OBJ)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
