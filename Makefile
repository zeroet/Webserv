NAME = webserv

CONFIG = BaseDirectives.cpp \
		 HttpBlock.cpp \
		 LocationBlock.cpp \
		 Parser.cpp \
		 PrintConfig.cpp \
		 ServerBlock.cpp \
		 Tokenizer.cpp

SERVER = connection.cpp \
		 epoll.cpp \
		 socket.cpp

REQUEST = request.cpp \
		  OperateRequest.cpp

RESPONSE = Response.cpp

SRCS = main.cpp \
	   $(addprefix config/src/, $(CONFIG)) \
	   $(addprefix server/, $(SERVER)) \
	   $(addprefix response/, $(RESPONSE)) \
	   $(addprefix request/, $(REQUEST))

CXX = c++

OBJS = $(SRCS:.cpp=.o)

DEBUGFLAG = #-g3

DEBUG = #-fsanitize=address

CXXFLAGS = #-Wall -Wextra -Werror -std=c++98

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
