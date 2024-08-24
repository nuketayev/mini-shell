NAME = pipex
CFLAGS = -c

LIBFT = lib/libft
LIB = $(LIBFT)/libft.a
SOURCES = pipex.c pipex_utils.c
B_SOURCES = pipex_bonus.c pipex_utils.c

OBJECTS = ${SOURCES:.c=.o}
B_OBJECTS = ${B_SOURCES:.c=.o}
HEADERS = -I inc

ARGS = "infile" "  " "     " "outfile.txt"
CMD = < infile "  " | "     " > outfile.txt

all: $(LIB) $(NAME)

$(LIB):
	make -C $(LIBFT)

$(NAME): $(OBJECTS) $(LIB)
	@cc -o $@ $^

run: $(NAME)
	@./$^ $(ARGS)

valgrind: $(NAME)
	@valgrind --leak-check=full --show-leak-kinds=all ./$^ $(ARGS)

command:
	@$(CMD)

norm:
	norminette $(SOURCES) inc/pipex.h

bonus: $(B_OBJECTS) $(LIB)
	@cc -o pipex $^

%.o: %.c
	@cc $(CFLAGS) -o $@ $< $(HEADERS)

clean:
	@rm -rf $(OBJECTS) $(B_OBJECTS)
	@make fclean -C $(LIBFT)

fclean: clean
	@rm -rf $(NAME)

re: fclean all run

