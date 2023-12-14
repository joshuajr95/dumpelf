

# Macros for compiler and flags.
CC=gcc
CFLAGS=



# Commonly used directories and filepaths
SRC_DIR=src
INC_DIR=include
BUILD_DIR=build
INT_DIR=$(BUILD_DIR)/intermediate
OBJ_DIR=$(BUILD_DIR)/obj
TEST_DIR=test
INSTALL_DIR=/usr/local/bin


FILENAMES= debug.c main.c readelf.c commands.c stringify.c


SOURCE_FILES= $(patsubst %.c, $(SRC_DIR)/%.c, $(FILENAMES))
INTERMEDIATE_FILES= $(patsubst %.c, $(INT_DIR)/%.i, $(FILENAMES))
OBJECT_FILES= $(patsubst %.c, $(OBJ_DIR)/%.o, $(FILENAMES))

OUTPUT_BINARY=dumpelf

TARGET=$(BUILD_DIR)/$(OUTPUT_BINARY)


# Recipes to build intermediate files, object files, and output binary

$(TARGET): $(OBJECT_FILES)
	$(CC) $(CFLAGS) $^ -o $@


$(OBJECT_FILES): $(OBJ_DIR)/%.o: $(INT_DIR)/%.i
	$(CC) $(CFLAGS) -c $< -o $@


$(INTERMEDIATE_FILES): $(INT_DIR)/%.i: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I$(INC_DIR) -E $< -o $@


# build the code for debugging purposes. Just includes debugging symbols.
debug:
	$(MAKE) -f Makefile $(TARGET) -e CFLAGS=-g



# build the unit tests for dumpelf
tests:
	$(MAKE) -f $(TEST_DIR)/tests.mk $(TEST_TARGET)





.PHONY: clean install uninstall

clean:
	rm $(TARGET) $(OBJECT_FILES) $(INTERMEDIATE_FILES)


install:
	sudo cp $(TARGET) $(INSTALL_DIR)/$(OUTPUT_BINARY)


uninstall:
	sudo rm $(INSTALL_DIR)/$(OUTPUT_BINARY)