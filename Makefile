NAME = minishell
CFLAGS = -c
LDFLAGS = -lreadline

LIBFT = lib/libft
LIB = $(LIBFT)/libft.a
SOURCES = src/main.c src/parse_input.c src/pipex.c src/pipex_utils.c

OBJECTS = ${SOURCES:.c=.o}
HEADERS = -I ../inc -I /usr/include/readline

all: $(NAME)

$(LIB):
	@make -C $(LIBFT)

$(NAME): $(OBJECTS) $(LIB) $(LDFLAGS)
	@cc -o $@ $^

norm:
	norminette $(SOURCES) inc/minishell.h

%.o: %.c
	@cc $(CFLAGS) $(HEADERS) -o $@ $<

clean:
	@rm -rf $(OBJECTS)
	@make fclean -C $(LIBFT)

fclean: clean
	@rm -rf $(NAME)

re: fclean all