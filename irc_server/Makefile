CXX		= c++
CXX_F	= -Wall -Wextra -Werror -std=c++98
NAME	= ircserv
SRC		= main.cpp server.cpp
OBJS 	= $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXX_F) $(OBJS) -o $(NAME)

%.o: %.cpp server.hpp
	$(CXX) $(CXX_F) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all
