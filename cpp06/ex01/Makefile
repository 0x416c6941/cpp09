# Compiler options.
CXX = c++
CXXFLAGS = -I$(INC_DIR) -std=c++98				\
	 -Wall -Wextra -Wsign-conversion -pedantic -Werror	\
	 -g -fsanitize=address

# Headers.
INC_DIR = .

# Source files.
SRC_FILES = Serializer.cpp Data.cpp main.cpp

# Object files.
OBJ_FILES = $(patsubst %.cpp,%.o,$(SRC_FILES))

# Name of the resulting binary file.
NAME = ex00

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
