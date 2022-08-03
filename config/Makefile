NAME = config

SRCS_DIR = src/

INCLUDES = includes/

SRCS = main.cpp \
	PrintConfig.cpp \
       Tokenizer.cpp \
       Parser.cpp \
	BaseDirectives.cpp \
	HttpBlock.cpp \
	ServerBlock.cpp \
	LocationBlock.cpp \


OBJS_DIR = obj/

OBJS = $(SRCS:.cpp=.o)
OBJS := $(addprefix $(OBJS_DIR), $(OBJS))

CC = c++

SAM = -fsanitize=address 

CFLAGS = -Wall -Wextra -Werror -g #-std=c++98

RM = /bin/rm -rf

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) -o $(NAME) $(OBJS) -I $(INCLUDES)

$(OBJS_DIR)%.o: $(SRCS_DIR)%.cpp
	mkdir -p $(OBJS_DIR)
	$(CC) $(CFLAGS) -std=c++98 -c $< -o $@ 
#%.o:%.cpp
#	$(CC) -c -o $@ $<

clean :
	$(RM) $(OBJS) $(OBJS_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY : all clean fclean re

