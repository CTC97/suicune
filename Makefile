# ---- basic config ----
CXX = g++
CXXFLAGS = -std=c++17 -Wall

SRC = main.cpp $(wildcard scenes/*.cpp) $(wildcard entities/*.cpp) $(wildcard src/*.cpp)
OUT = barley

# macOS defaults (your current platform)
RAY_INC = /opt/homebrew/include
RAY_LIB = /opt/homebrew/lib

LIBS = -L$(RAY_LIB) -lraylib \
       -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

all:
	$(CXX) $(CXXFLAGS) -I$(RAY_INC) $(SRC) -o $(OUT) $(LIBS)

clean:
	rm -f $(OUT)
