# Thanks to Job Vranish (https://spin.atomicobject.com/2016/08/26/makefile-c-projects/)
LEVEL?=0
TARGET_EXEC := basalt_linux.x11
TARGET_LIB := bullet_game.so

BUILD_DIR := ./build
SRC_DIR := ./src

SRCS := $(shell find $(SRC_DIR) -type f \( -name 'basalt_*.c' -o -name 'xorg_*.c' \))
SRCS_GAME := $(shell find $(SRC_DIR) -type f \( -name 'bullet_*.c' \))

OBJS := $(SRCS:%=$(BUILD_DIR)/%.o) $(BUILD_DIR)/$(SRC_DIR)/bullet_assets.dat.c.o
OBJS_GAME := $(SRCS_GAME:%=$(BUILD_DIR)/%.o)

INC_FLAGS := $(addprefix -I,$(SRC_DIR))

CFLAGS := $(INC_FLAGS) -ggdb -Wall -O$(LEVEL) -fPIC
LDFLAGS := -lX11 -lm -lXext

build: $(BUILD_DIR)/$(TARGET_EXEC) $(BUILD_DIR)/$(TARGET_LIB)

# Build the embedder
EMBEDDER_SRC := src/tooling/embedder.c
$(BUILD_DIR)/embedder: $(EMBEDDER_SRC)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(EMBEDDER_SRC) -O3 -o $(BUILD_DIR)/embedder

# Run the embedder
$(SRC_DIR)/bullet_assets.dat.c: $(BUILD_DIR)/embedder
	$(BUILD_DIR)/embedder ./assets ./src/bullet_assets.dat.c

# Linking step (standard)
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS) $(OBJS_GAME)
	$(CXX) $(OBJS) $(OBJS_GAME) -o $@ $(LDFLAGS)

# Linking step (shared library)
$(BUILD_DIR)/$(TARGET_LIB): $(OBJS_GAME)
	$(CXX) $(OBJS_GAME) -shared -o $@ -lm

# Build step for C source
$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)
	rm src/*.dat.c
