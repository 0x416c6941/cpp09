# Compiler options.
CXX = clang++
CXXFLAGS = -I$(INC_DIR) -std=c++98					\
	 -Wall -Wextra -Weffc++ -Wsign-conversion -pedantic -Werror	\
	 -g -fsanitize=address

# Headers.
INC_DIR = .

# Source files.
SRC_FILES = main.cpp BitcoinExchange.cpp

# Object files.
OBJ_FILES = $(patsubst %.cpp,%.o,$(SRC_FILES))

# Name of the resulting binary file.
NAME = btc

# Targets.
all: $(NAME)

$(NAME): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $(OBJ_FILES) -o $@

%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(OBJ_FILES)

.PHONY: fclean
fclean: clean
	rm -f $(NAME)

.PHONY: re
re: fclean all
