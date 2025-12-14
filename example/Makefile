# ---- basic config ----
CXX = g++
CXXFLAGS = -std=c++17 -Wall

suicune_src = $(wildcard *.cpp) $(wildcard ../suicune_src/*.cpp)
OUT = suicune

# macOS defaults (your current platform)
RAY_INC = /opt/homebrew/include
RAY_LIB = /opt/homebrew/lib

LIBS = -L$(RAY_LIB) -lraylib \
       -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

all:
	$(CXX) $(CXXFLAGS) -I$(RAY_INC) $(suicune_src) -o $(OUT) $(LIBS)

clean:
	rm -f $(OUT)
