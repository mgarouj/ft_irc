NAME        = ircserv
BOT_NAME    = irc_bot

CXX         = c++

CXXFLAGS    = -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g


SRCS        = main.cpp ./Client/Client.cpp ./Server/Server.cpp ./Server/ErrorMsg.cpp  ./Commands/Pass.cpp \
              ./Commands/Nick.cpp ./Commands/User.cpp ./Channel/Channel.cpp \
              ./Commands/Join.cpp ./Commands/Privmsg.cpp ./Commands/Mode.cpp \
				./Commands/Invite.cpp ./Commands/Kick.cpp ./Commands/Topic.cpp

OBJS        = $(SRCS:.cpp=.o)

SRCS_B      = ./bonus/bot.cpp ./bonus/main.cpp
OBJS_B      = $(SRCS_B:.cpp=.o)


all: $(NAME)

$(NAME): $(OBJS) ./Headers/Server.hpp
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

bonus: $(BOT_NAME)

$(BOT_NAME): $(OBJS_B)
	$(CXX) $(CXXFLAGS) $(OBJS_B) -o $(BOT_NAME)


clean:
	rm -f $(OBJS) $(OBJS_B)

fclean: clean
	rm -f $(NAME) $(BOT_NAME)

re: fclean all

run: fclean all clean
	clear
	./ircserv 7777 aaaa

.PHONY: all clean fclean re bonus run