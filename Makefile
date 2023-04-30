# Thanks to Job Vranish (https://spin.atomicobject.com/2016/08/26/makefile-c-projects/)
LEVEL?=0
TARGET_EXEC := basalt_linux.x11
TARGET_LIB := bullet_game.so

BUILD_DIR := ./build
SRC_DIR := ./src

SRCS := $(shell find $(SRC_DIR) -type f \( -name 'basalt_*.c' -o -name 'xorg_*.c' \))
SRCS_GAME := $(shell find $(SRC_DIR) -type f \( -name 'bullet_*.c' \))

OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
OBJS_GAME := $(SRCS_GAME:%=$(BUILD_DIR)/%.o)

INC_FLAGS := $(addprefix -I,$(SRC_DIR))

CFLAGS := $(INC_FLAGS) -ggdb -Wall -O$(LEVEL) -fPIC
LDFLAGS := -lX11 -lm -lXext

build: $(BUILD_DIR)/$(TARGET_EXEC) 
lib: $(BUILD_DIR)/$(TARGET_LIB)

# Download deflation
$(DEFLATION_SRC):
	echo "TODO: Wget deflation"

# Build deflation 
ASSET_DEF := $(BUILD_DIR)/bullet_assets.def
DEFLATION_SRC := src/external/deflation.h src/external/deflate_cli.c
$(BUILD_DIR)/deflate: $(DEFLATION_SRC)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(DEFLATION_SRC) -O3 -o $(BUILD_DIR)/deflate

# Run deflation 
$(ASSET_DEF): $(BUILD_DIR)/deflate
	bash -c 'cd assets; ../$(BUILD_DIR)/deflate . ../build/bullet_assets.def'

# Linking step (standard)
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS) $(OBJS_GAME) $(ASSET_DEF)
	$(CXX) $(OBJS) $(OBJS_GAME) -o $@ $(LDFLAGS)

# Linking step (shared library)
$(BUILD_DIR)/$(TARGET_LIB): $(OBJS_GAME) $(ASSET_DEF)
	$(CXX) $(OBJS_GAME) -shared -o $@ -lm

# Build step for C source
$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)
	rm src/*.dat.c
