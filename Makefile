NAME = minishell
CFLAGS = -c
LDFLAGS = -lreadline

LIBFT = lib/libft
LIB = $(LIBFT)/libft.a
SOURCES = src/placeholder/main.c src/placeholder/parse_input.c src/placeholder/pipex.c src/placeholder/pipex_utils.c \
src/placeholder/signal_handler.c src/placeholder/handle_quotes.c src/placeholder/get_last_token.c \
src/placeholder/redirections.c src/placeholder/execute.c \
src/builtins/cd.c src/builtins/echo.c src/builtins/env.c src/builtins/exit.c src/builtins/export.c src/builtins/pwd.c \
src/builtins/unset.c src/placeholder/test_utils.c src/placeholder/expand.c src/builtins/print_exit_int.c

OBJECTS = ${SOURCES:.c=.o}
HEADERS = -I inc -I /usr/include/readline

all: $(NAME)

$(LIB):
	@make -C $(LIBFT)

$(NAME): $(OBJECTS) $(LIB) $(LDFLAGS)
	@cc -o $@ $^

norm:
	norminette $(SOURCES) inc/minishell.h

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --suppressions=minishell.supp ./minishell

%.o: %.c
	@cc $(CFLAGS) $(HEADERS) -o $@ $<

clean:
	@rm -rf $(OBJECTS)
	@make fclean -C $(LIBFT)

fclean: clean
	@rm -rf $(NAME)

re: fclean all