OS := $(shell uname)

CXX = g++
CXX_DEBUG_FLAGS = -std=c++17 -Wall -g -O0 -DDEBUG
CXX_RELEASE_FLAGS = -std=c++17 -Wall -O3

SDL_CFLAGS = -I./include/ -I./include/SDL2 -L$(LIBS_DIR)
SDL_LIBS =
ifeq ($(OS), Windows_NT)
	SDL_LIBS = -lmingw32 -lSDL2main -lSDL2
else
	SDL_LIBS = -lSDL2 
endif

INCLUDES = -Iimgui -Iimgui/backends -Iimplot

DEBUG_OBJ_DIR = obj/debug
RELEASE_OBJ_DIR = obj/release
BIN_DEBUG_DIR = bin/debug
BIN_RELEASE_DIR = bin/release
LIBS_DIR = bin/libs

DEBUG_TARGET = $(BIN_DEBUG_DIR)/Detector
RELEASE_TARGET = $(BIN_RELEASE_DIR)/Detector

OTHER_DEPENDENCIES_FLAGS = -L$(LIBS_DIR) -limgui -limplot -lpantompkins

ifeq ($(OS),Windows_NT)
    CP := copy /Y
    MKDIR := mkdir
else
    CP := cp -f
    MKDIR := mkdir -p
endif

# =========================================================
# DIRECTORIES
# =========================================================

directories:
	@$(MKDIR) $(DEBUG_OBJ_DIR)
	@$(MKDIR) $(RELEASE_OBJ_DIR)
	@$(MKDIR) $(BIN_DEBUG_DIR)
	@$(MKDIR) $(BIN_RELEASE_DIR)
	@$(MKDIR) $(LIBS_DIR)

# Building Dependency files
# 1. building imgui

IMGUI_SOURCES = imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp imgui/imgui_widgets.cpp \
				imgui/backends/imgui_impl_sdl2.cpp imgui/backends/imgui_impl_opengl3.cpp \
				imgui/backends/imgui_impl_sdlrenderer2.cpp
IMGUI_OBJECTS = $(patsubst %.cpp, $(DEBUG_OBJ_DIR)/%.o, $(IMGUI_SOURCES))

$(DEBUG_OBJ_DIR)/imgui/%.o: imgui/%.cpp
	@$(MKDIR) $(dir $@)
	$(CXX) $(CXX_RELEASE_FLAGS) $(INCLUDES) $(SDL_CFLAGS) -c $< -o $@

imgui: $(IMGUI_OBJECTS)
	ar rcs $(LIBS_DIR)/libimgui.a $(IMGUI_OBJECTS)

# 2. building implot

IMPLOT_SOURCES = implot/implot.cpp implot/implot_items.cpp
IMPLOT_OBJECTS = $(patsubst %.cpp, $(DEBUG_OBJ_DIR)/%.o, $(IMPLOT_SOURCES))

$(DEBUG_OBJ_DIR)/implot/%.o: implot/%.cpp
	@$(MKDIR) $(dir $@)
	$(CXX) $(CXX_RELEASE_FLAGS) $(INCLUDES) $(SDL_CFLAGS) -c $< -o $@

implot: $(IMPLOT_OBJECTS) imgui
	ar rcs $(LIBS_DIR)/libimplot.a $(IMPLOT_OBJECTS)

# 3. building PanTompkins

PanTompkins: Cpp_Ecg_Detectors/**.cpp Cpp_Ecg_Detectors/**.h
	$(MAKE) -C Cpp_Ecg_Detectors release
	$(CP) Cpp_Ecg_Detectors/bin/release/libDetectorPanTompkins.a $(LIBS_DIR)/libpantompkins.a
	

# =========================================================
# BUILDING TARGETS
# =========================================================

MODULES_SOURCES = $(wildcard modules/*.cpp)
GUI_SOURCES = $(wildcard gui/*.cpp)
SOURCES = main.cpp $(MODULES_SOURCES) $(GUI_SOURCES)

DEBUG_OBJECTS = $(patsubst %.cpp, $(DEBUG_OBJ_DIR)/%.o, $(SOURCES))
RELEASE_OBJECTS = $(patsubst %.cpp, $(RELEASE_OBJ_DIR)/%.o, $(SOURCES))

# Compile GUI files (with imgui/implot includes)
$(DEBUG_OBJ_DIR)/gui/%.o: gui/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXX_DEBUG_FLAGS) $(INCLUDES) $(SDL_CFLAGS) -c $< -o $@

$(RELEASE_OBJ_DIR)/gui/%.o: gui/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXX_RELEASE_FLAGS) $(INCLUDES) $(SDL_CFLAGS) -c $< -o $@

# Compile non-GUI files
$(DEBUG_OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXX_DEBUG_FLAGS) $(INCLUDES) $(SDL_CFLAGS) -c $< -o $@

$(RELEASE_OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXX_RELEASE_FLAGS) $(INCLUDES) $(SDL_CFLAGS) -c $< -o $@

# Debug build
all: directories imgui implot PanTompkins $(DEBUG_OBJECTS) 
	$(CXX) $(CXX_DEBUG_FLAGS) -o $(DEBUG_TARGET) $(DEBUG_OBJECTS) $(OTHER_DEPENDENCIES_FLAGS) $(SDL_LIBS)

# Release build
release: directories imgui implot PanTompkins $(RELEASE_OBJECTS)
	$(CXX) $(CXX_RELEASE_FLAGS) -o $(RELEASE_TARGET) $(RELEASE_OBJECTS) $(OTHER_DEPENDENCIES_FLAGS) $(SDL_LIBS)

run: all
	$(DEBUG_TARGET)

clean:
	rm -rf obj bin/debug bin/release
