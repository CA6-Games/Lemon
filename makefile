BUILD_FILE := lemon.elf
SRC_DIR := ./
RES_DIR := ./res
OBJ_DIR := ./obj
BIN_DIR := ./bin
SHELL_FILE := ./src/minimal.html

# Determine if building for WebAssembly
ifeq ($(WEB),1)
  CC := emcc
  CXX := em++
  BUILD_FILE := web.html
  PRELOAD_FILES := $(shell find $(RES_DIR) -name "*.*")
  CC_OPT := -s WASM=2 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]'  -s USE_SDL_TTF=2 -s USE_SDL_MIXER=2 -s SDL2_MIXER_FORMATS='["mp3"]' -s FORCE_FILESYSTEM=1 $(foreach file,$(PRELOAD_FILES),--preload-file $(file)) -sABORTING_MALLOC=0 -s ALLOW_MEMORY_GROWTH=1
  CXX_OPT := -s WASM=2 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' -s USE_SDL_TTF=2 -s USE_SDL_MIXER=2 -s SDL2_MIXER_FORMATS='["mp3"]' -s FORCE_FILESYSTEM=1 $(foreach file,$(PRELOAD_FILES),--preload-file $(file)) -sABORTING_MALLOC=0 -s ALLOW_MEMORY_GROWTH=1
else
  CC := gcc
  CXX := g++
  CC_OPT := -O0 -Wall -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2_net -lm
  CXX_OPT := -O0
endif

# Find source files
SRCS := $(shell find $(SRC_DIR) -name '*.cpp' -or -name '*.c' -or -name '*.s')
OBJS := $(SRCS:%=$(OBJ_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIR) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
CPPFLAGS := $(INC_FLAGS) -MMD -MP

# Final build step
$(BIN_DIR)/$(BUILD_FILE): $(OBJS)
	$(CC) $(OBJS) $(CC_OPT) -o $@ $(LDFLAGS)

# Build C source
$(OBJ_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) -c $< -o $@

# Build C++ source
$(OBJ_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(BIN_DIR)/*
	rm -rf $(OBJ_DIR)/*

# Do not remove
-include $(DEPS)
