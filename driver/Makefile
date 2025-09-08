# Define variables
CXX = g++
CFLAGS = -I ../include \
		 -I ../include/classes \
		 -I ../include/namespaces
LDFLAGS = -L ../build -lsciantix

SRC = main.c
OBJ = $(SRC:.c=.o)
TARGET = sciantix_driver

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
