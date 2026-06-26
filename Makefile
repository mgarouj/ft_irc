NAME        = ft_irc
BOT_NAME    = irc_bot

CXX         = c++
# FIXED TYPO: Changed CXXCXXFLAGSS to CXXFLAGS
CXXFLAGS    = -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g

# Add your source files here
SRCS        = main.cpp ./Client/Client.cpp ./Server/Server.cpp ./Commands/Pass.cpp \
              ./Commands/Nick.cpp ./Commands/User.cpp ./Channel/Channel.cpp \
              ./Commands/Join.cpp ./Commands/Privmsg.cpp ./Commands/Mode.cpp

OBJS        = $(SRCS:.cpp=.o)

SRCS_B      = ./bonus/bot.cpp
OBJS_B      = $(SRCS_B:.cpp=.o)


all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# FIXED: 'bonus' is now a rule that builds the 'irc_bot' executable
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
	./ft_irc 7777 aaaa

# FIXED: Added 'bonus' and 'run' to .PHONY
.PHONY: all clean fclean re bonus run