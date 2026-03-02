# Compiler
CXX = clang++

# Compiler flags
CXXFLAGS = -fcolor-diagnostics -fansi-escape-codes -Wall -Wextra -std=c++17 -Iinclude -g
# -Wextra

# Custom flags
# ADD LATER

# Source files
SRC = $(wildcard src/*.cpp)

# Object files (auto-generated from SRC)
OBJ = $(SRC:.cpp=.o)

# Output executable
EXEC = chip8

# Default rule
# all: $(EXEC)

$(EXEC): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(EXEC)

# # Link step
# $(EXEC): $(OBJ)
# 	$(CXX) $(OBJ) -o $(EXEC)

# # Compile step
# src/%.o: src/%.cpp
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	@rm -f $(OBJ) $(TARGET)
