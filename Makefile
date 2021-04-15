CC = gcc

.PHONY: re, clean, fclean, all, bonus

NAME= termcaps

SRC_FILE=	./new_termcaps.c

OBJ = $(SRC:%.c=%.o)

all: $(NAME)

all bonus : $(NAME)

$(NAME): $(OBJ)
	$(CC) -ltermcap -o termcaps $(OBJ)

%.o: %.c
	$(CC) -ltermcap -Wall -Wextra -Werror -c -o $@ $<

clean:
	rm -f termcaps
	rm -f ./new_termcaps.o

fclean: clean

re: clean all