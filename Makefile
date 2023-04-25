# Thanks to Job Vranish (https://spin.atomicobject.com/2016/08/26/makefile-c-projects/)
LEVEL?=0
TARGET_EXEC := basalt_linux.x11

BUILD_DIR := ./build
SRC_DIR := ./src

SRCS := $(shell find $(SRC_DIR) -type f \( -name 'basalt_*.c' -o -name 'xorg_*.c' \))
SRCS_GAME := $(shell find $(SRC_DIR) -type f \( -name 'bullet_*.c' \))

OBJS := $(SRCS:%=$(BUILD_DIR)/%.o) $(BUILD_DIR)/$(SRC_DIR)/assets_custom.dat.c.o $(BUILD_DIR)/$(SRC_DIR)/locale_custom.dat.c.o
OBJS_GAME := $(SRCS_GAME:%=$(BUILD_DIR)/%.o)

INC_FLAGS := $(addprefix -I,$(SRC_DIR))

CFLAGS := $(INC_FLAGS) -ggdb -Wall -O$(LEVEL)
LDFLAGS := -lX11 -lm -lXext

build: $(BUILD_DIR)/$(TARGET_EXEC)

# Build the embedder
EMBEDDER_SRC := src/tooling/embedder.c
$(BUILD_DIR)/embedder: $(EMBEDDER_SRC)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(EMBEDDER_SRC) -O3 -o $(BUILD_DIR)/embedder

# Build the localization generator
LOCALE_SRC := src/tooling/embedder_locale.d
$(BUILD_DIR)/localegen: $(LOCALE_SRC)
	mkdir -p $(BUILD_DIR)
	dmd $(LOCALE_SRC) -O -inline -release -of=$(BUILD_DIR)/localegen

# Run the embedder
$(SRC_DIR)/assets_custom.dat.c: $(BUILD_DIR)/embedder
	$(BUILD_DIR)/embedder ./assets ./src/assets_custom.dat.c

# Run the localization generator
$(SRC_DIR)/locale_custom.dat.c: $(BUILD_DIR)/localegen
	$(BUILD_DIR)/localegen -i ./assets -o ./src/locale_custom.dat.c

# Linking step
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS) $(OBJS_GAME)
	$(CXX) $(OBJS) $(OBJS_GAME) -o $@ $(LDFLAGS)

# Build step for C source
$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)
	rm src/*.dat.c
