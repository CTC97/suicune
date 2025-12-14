# -------- project config --------
CXX      ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra

TARGET   ?= build/suicune

# Source layout (edit these globs if you move folders)
SRC := $(wildcard *.cpp) \
       $(wildcard ../suicune_src/*.cpp) \
       $(wildcard ./src/*.cpp)

OBJDIR := build/obj
OBJ := $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRC))

# -------- platform detection --------
UNAME_S := $(shell uname -s)

# Prefer pkg-config when available (most portable)
PKG_CONFIG ?= pkg-config
HAVE_PKGCONFIG := $(shell command -v $(PKG_CONFIG) >/dev/null 2>&1 && echo yes || echo no)

# Defaults (can be overridden: make RAY_INC=... RAY_LIB=...)
RAY_INC ?=
RAY_LIB ?=

ifeq ($(HAVE_PKGCONFIG),yes)
  # If raylib is installed with a .pc file, this is the cleanest path.
  RAY_CFLAGS := $(shell $(PKG_CONFIG) --cflags raylib 2>/dev/null)
  RAY_LIBS   := $(shell $(PKG_CONFIG) --libs raylib 2>/dev/null)
endif

# Fallbacks when pkg-config doesn't know raylib
ifeq ($(strip $(RAY_LIBS)),)
  ifeq ($(UNAME_S),Darwin)
    # Homebrew paths: Apple Silicon is /opt/homebrew, Intel is /usr/local
    ifeq ($(wildcard /opt/homebrew/include),/opt/homebrew/include)
      RAY_INC ?= /opt/homebrew/include
      RAY_LIB ?= /opt/homebrew/lib
    else
      RAY_INC ?= /usr/local/include
      RAY_LIB ?= /usr/local/lib
    endif

    RAY_CFLAGS += -I$(RAY_INC)
    RAY_LIBS   += -L$(RAY_LIB) -lraylib \
                  -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
  else
    # Linux fallback (varies by distro, but this is common)
    RAY_INC ?= /usr/include
    RAY_LIB ?= /usr/lib

    RAY_CFLAGS += -I$(RAY_INC)
    RAY_LIBS   += -L$(RAY_LIB) -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
  endif
endif

CPPFLAGS += $(RAY_CFLAGS)
LDFLAGS  += $(RAY_LIBS)

# -------- rules --------
.PHONY: all clean run print-config

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $@ $(LDFLAGS)

# Compile .cpp -> .o into build/obj/ preserving subfolders
$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf build

run: all
	./$(TARGET)

print-config:
	@echo "UNAME_S     = $(UNAME_S)"
	@echo "HAVE_PKGCFG = $(HAVE_PKGCONFIG)"
	@echo "CPPFLAGS    = $(CPPFLAGS)"
	@echo "LDFLAGS     = $(LDFLAGS)"
	@echo "SRC         = $(SRC)"
