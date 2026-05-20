OS := $(shell uname)

FLAGS = -Wall -g -O0

SDL_CFLAGS = -I./include/ -I./include/SDL2 -L$(LIBS_DIR)
SDL_LIBS =
ifeq ($(OS), Windows_NT)
	SDL_LIBS = -lmingw32 -lSDL2main -lSDL2
else
	SDL_LIBS = -lSDL2 
endif

INCLUDES = -Iimgui -Iimgui/backends -Iimplot
LIBS_DIR = bin/libs

PROGRAM = bin/Detector

IMPLOT_FLAGS = -L$(LIBS_DIR) -limgui -limplot

# Directories

directories:
	@mkdir -p obj
	@mkdir -p bin
	@mkdir -p $(LIBS_DIR)

# Building Dependency files
# 1. building imgui

IMGUI_SOURCES = imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp imgui/imgui_widgets.cpp \
				imgui/backends/imgui_impl_sdl2.cpp imgui/backends/imgui_impl_opengl3.cpp \
				imgui/backends/imgui_impl_sdlrenderer2.cpp
IMGUI_OBJECTS = $(patsubst %.cpp, obj/%.o, $(IMGUI_SOURCES))

obj/imgui/%.o: imgui/%.cpp
	@mkdir -p $(dir $@)
	g++ $(FLAGS) $(INCLUDES) $(SDL_CFLAGS) -c $< -o $@

imgui: $(IMGUI_OBJECTS)
	ar rcs $(LIBS_DIR)/libimgui.a $(IMGUI_OBJECTS)

# 2. building implot

IMPLOT_SOURCES = implot/implot.cpp implot/implot_items.cpp
IMPLOT_OBJECTS = $(patsubst %.cpp, obj/%.o, $(IMPLOT_SOURCES))

obj/implot/%.o: implot/%.cpp
	@mkdir -p $(dir $@)
	g++ $(FLAGS) $(INCLUDES) $(SDL_CFLAGS) -c $< -o $@

implot: $(IMPLOT_OBJECTS) imgui
	ar rcs $(LIBS_DIR)/libimplot.a $(IMPLOT_OBJECTS)

# 3. building PanTompkins

DETECTOR_SOURCES = $(wildcard Cpp_Ecg_Detectors/**/*.cpp)
DETECTOR_OBJECTS = $(patsubst %.cpp, obj/%.o, $(DETECTOR_SOURCES))

obj/detector/%.o: Cpp_Ecg_Detectors/%.cpp
	@mkdir -p $(dir $@)
	g++ $(FLAGS) $(INCLUDES) $(SDL_CFLAGS) -c $< -o $@

# Building Targets

MODULES_SOURCES = $(wildcard modules/*.cpp)
GUI_SOURCES = $(wildcard gui/*.cpp)
SOURCES = main.cpp $(MODULES_SOURCES) $(GUI_SOURCES)

OBJECTS = $(patsubst %.cpp, obj/%.o, $(SOURCES))

# Compile GUI files (with imgui/implot includes)
obj/gui/%.o: gui/%.cpp
	@mkdir -p $(dir $@)
	g++ $(FLAGS) $(INCLUDES) $(SDL_CFLAGS) -c $< -o $@

# Compile non-GUI files
obj/%.o: %.cpp
	@mkdir -p $(dir $@)
	g++ $(FLAGS) $(INCLUDES) $(SDL_CFLAGS) -c $< -o $@

# Debug build
all: directories imgui implot $(DETECTOR_OBJECTS) $(OBJECTS)
	g++ $(FLAGS) -o $(PROGRAM) $(OBJECTS) $(DETECTOR_OBJECTS) $(IMPLOT_FLAGS) $(SDL_LIBS)

run: all
	$(PROGRAM)

clean:
	rm -rf obj/main.o obj/modules bin/Detector.exe