# Define variables
CXX = g++
CFLAGS = -I include \
		 -I include/classes \
		 -I include/namespaces
LDFLAGS = -L build -lsciantix

SRC = main.C
OBJ = $(SRC:.C=.o)
TARGET = sciantix_driver

# Default target: builds the executable
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)
	
# Rule to compile .C files into .o files
%.o: %.C
	$(CXX) $(CFLAGS) -c $< -o $@

# Clean target: removes compiled files
.PHONY: clean
clean:
	rm -f $(OBJ) $(TARGET)
