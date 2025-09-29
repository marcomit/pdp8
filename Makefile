# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g

# Folders
BUILD_DIR = target

# Executable name and path
TARGET = $(BUILD_DIR)/pdp8

# Source files
SRCS = main.c pdp8.c tokenizer.c

# Object files in BUILD_DIR
OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)

# Default target
all: $(TARGET)

# Link object files into executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile .c files to .o files in target/
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Ensure target directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean build
clean:
	rm -rf $(BUILD_DIR)

# Build and run
run: all
	echo ./$(TARGET)

# Clean then run
run-clean:
	make clean && make run

# Mark targets as not files
.PHONY: all clean run run-clean

