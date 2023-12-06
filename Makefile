

# Macros for compiler and flags.
CC=gcc
CFLAGS=



# Commonly used directories and filepaths
SRC_DIR=src
INC_DIR=include
BUILD_DIR=build
INT_DIR=$(BUILD_DIR)/intermediate
OBJ_DIR=$(BUILD_DIR)/obj


FILENAMES= debug.c main.c readelf.c


SOURCE_FILES= $(patsubst %.c, $(SRC_DIR)/%.c, $(FILENAMES))
INTERMEDIATE_FILES= $(patsubst %.c, $(INT_DIR)/%.i, $(FILENAMES))
OBJECT_FILES= $(patsubst %.c, $(OBJ_DIR)/%.o, $(FILENAMES))

TARGET=$(BUILD_DIR)/dumpelf


# Recipes to build intermediate files, object files, and output binary

$(TARGET): $(OBJECT_FILES)
	$(CC) $(CFLAGS) $^ -o $@


$(OBJECT_FILES): $(OBJ_DIR)/%.o: $(INT_DIR)/%.i
	$(CC) $(CFLAGS) -c $< -o $@


$(INTERMEDIATE_FILES): $(INT_DIR)/%.i: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -E $< -o $@



debug:
	$(MAKE) -f Makefile $(TARGET) -e CFLAGS=-g


.PHONY: clean

clean:
	rm $(TARGET) $(OBJECT_FILES) $(INTERMEDIATE_FILES)
