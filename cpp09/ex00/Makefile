# Compiler options.
CXX = clang++
CXXFLAGS = -I$(INC_DIR) -std=c++98					\
		-Wall -Wextra -Wsign-conversion -pedantic -Werror	\
		-g

# Headers.
INC_DIR = .

# Source files.
SRC_FILES = main.cpp			\
	    Date.cpp			\
	    BTCExchangeHistory.cpp

# Object files.
OBJ_FILES = $(patsubst %.cpp,%.o,$(SRC_FILES))

# Name of the resulting binary file.
NAME = btc

# Targets.
all: $(NAME)

$(NAME): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(OBJ_FILES)

.PHONY: fclean
fclean: clean
	rm -f $(NAME)

.PHONY: re
re: fclean $(NAME)
