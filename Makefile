LIBFT_PATH = /home/melan/Projects/libft/Common-Core---Libft
NAME = main_v5
SOURCES = ./main_v5.c
OBJECTS = $(SOURCES:.c=.o)
CFLAGS = -Wall -Wextra -Werror -I $(LIBFT_PATH)
LDFLAGS = -L $(LIBFT_PATH)
LIBS = -lft

.PHONY: all clean fclean re

$(NAME): $(OBJECTS) $(LIBFT_PATH)/libft.a
	cc $(CFLAGS) $(OBJECTS) $(LDFLAGS) $(LIBS) -o $(NAME)

$(LIBFT_PATH)/libft.a:
	make -C $(LIBFT_PATH) all

%.o: %.c
	cc $(CFLAGS) -c $< -o $@

all: $(NAME)

clean:
	rm -f $(OBJECTS)
	make -C $(LIBFT_PATH) clean

fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_PATH) fclean

re: fclean all