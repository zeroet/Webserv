NAME = webserv

SRCS = main.cpp request.cpp epoll.cpp socket.cpp connection.cpp RequestHandler.cpp ../response/Response.cpp

CXX = c++

OBJS = $(SRCS:.cpp=.o)

DEBUGFLAG = -g3

DEBUG = -fsanitize=address

CXXFLAGS = -Wall -Wextra -Werror -std=c++98

RM = rm -f

all: $(NAME)

$(NAME): $(OBJS)
		$(CXX) $(CXXFLAGS) $(DEBUGFLAG) $(DEBUG) -o $(NAME) $(OBJS)

clean:
		$(RM) $(OBJS)

fclean: clean
		$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
