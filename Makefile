NAME        = ft_irc

CXX         = c++
CXXFLAGS    = -Wall -Wextra -Werror -std=c++98

# Add your source files here
SRCS        = main.cpp ./Client/Client.cpp ./Server/Server.cpp ./Commands/Pass.cpp ./Commands/Nick.cpp ./Commands/User.cpp ./Channel/Channel.cpp ./Commands/Join.cpp ./Commands/Privmsg.cpp ./Commands/Mode.cpp

OBJS        = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

run: fclean all clean
	clear
	./ft_irc 7777 aaaa
.PHONY: all clean fclean re