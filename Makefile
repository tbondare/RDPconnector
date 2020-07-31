NAME = RDPconnector

C = gcc
CFLAG = -Wall -Wextra -Werror

SRC = main.c server.c client.c
OBJ = main.o server.o client.o
INC = client.h server.h wrap_unwrap.h

%.o: $(SRC) $(INC)
	$(C) $(CFLAG) -c $(NAME) 

all: $(NAME)

$(NAME): $(OBJ)
	$(C) -o $(NAME) $(CFLAG) $(OBJ)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
