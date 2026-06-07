CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRCDIR = .
OBJDIR = obj
BINDIR = .

SOURCES = $(SRCDIR)/main.cpp $(SRCDIR)/Server/Server.cpp $(SRCDIR)/Client/Client.cpp
OBJECTS = $(OBJDIR)/main.o $(OBJDIR)/Server/Server.o $(OBJDIR)/Client/Client.o
TARGET = irc_server

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/$@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -I$(SRCDIR)/Headers -c $< -o $@

clean:
	rm -rf $(OBJDIR)
