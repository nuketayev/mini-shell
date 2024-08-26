NAME = minishell
CFLAGS = -c

LIBFT = lib/libft
LIB = $(LIBFT)/libft.a
SOURCES = src/main.c src/parse_input.c src/execute.c

OBJECTS = ${SOURCES:.c=.o}
HEADERS = -I ../inc

all: $(LIB) $(NAME)

$(LIB):
	@make -C $(LIBFT)

$(NAME): $(OBJECTS) $(LIB)
	@cc -lreadline -o $@ $^

norm:
	norminette $(SOURCES) inc/minishell.h

%.o: %.c
	@cc $(CFLAGS) -o $@ $< $(HEADERS)

clean:
	@rm -rf $(OBJECTS)
	@make fclean -C $(LIBFT)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

