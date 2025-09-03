# Define variables
CXX = g++
CFLAGS = -I /home/fbsmith/sciantix-official/include \
		 -I /home/fbsmith/sciantix-official/include/classes \
		 -I /home/fbsmith/sciantix-official/include/namespaces
LDFLAGS = -lm

SRC = main.c
OBJ = $(SRC:.c=.o)
TARGET = main

# Default target: builds the executable
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Rule to compile .c files into .o files
%.o: %.c
	$(CXX) $(CFLAGS) -c $< -o $@

.INTERMEDIATE: $(OBJ)

# Clean target: removes compiled files
.PHONY: clean
clean:
	rm -f $(OBJ) $(TARGET)
