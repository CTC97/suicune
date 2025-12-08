# ---- basic config ----
CXX = g++
CXXFLAGS = -std=c++17 -Wall

barley_src = main.cpp $(wildcard scenes/*.cpp) $(wildcard entities/*.cpp) $(wildcard barley_src/*.cpp)
OUT = barley

# macOS defaults (your current platform)
RAY_INC = /opt/homebrew/include
RAY_LIB = /opt/homebrew/lib

LIBS = -L$(RAY_LIB) -lraylib \
       -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

all:
	$(CXX) $(CXXFLAGS) -I$(RAY_INC) $(barley_src) -o $(OUT) $(LIBS)

clean:
	rm -f $(OUT)
