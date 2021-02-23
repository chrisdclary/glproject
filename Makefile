# Build for Windows by default
TARGET := WINDOWS

# Compilation options
INC     := -Isrc
CPPFLAGS  := -static-libstdc++ -static-libgcc -std=c++17 -Wall -Wextra -Wpedantic -g -Og -march=native 
CFLAGS  := -std=gnu11 -Wall -Wextra -Wpedantic -g -Og -march=native 
LDFLAGS := -g
LIBS    := -lSDL2

# Options for specific targets
ifeq ($(TARGET), LINUX)
CC   := gcc
LD   := $(CC)
OUT  := testgame
LIBS += -lGL
else
CPP     := x86_64-w64-mingw32-g++
CC		:= x86_64-w64-mingw32-gcc
LD      := $(CPP)
OUT     := testgame.exe
INC     += -Ilib/SDL2/x86_64-w64-mingw32/include -Ilib/GLM -Ilib/GLEW 
LDFLAGS += -Llib/SDL2/x86_64-w64-mingw32/bin
LIBS    += -lopengl32
endif

# Get list of object files
EXT     := c cpp vert geom frag
SRCDIR  := $(shell find src -type d -print)
OBJDIR  := $(patsubst src%,obj%,$(SRCDIR))
SRC     := $(foreach ext,$(EXT),$(foreach sdir,$(SRCDIR),$(wildcard $(sdir)/*.$(ext))))
OBJ     := $(patsubst src%,obj%.o,$(SRC))

# Set vpath for each compiled file extension
define set-vpath-ext
vpath %.$1 $(SRCDIR)
endef
$(foreach ext,$(EXT),$(eval $(call set-vpath-ext,$(ext))))

# Normal targets
all: checkdirs build

clean:
	rm -f $(OUT)
	rm -rf obj/

build: $(OBJ)
	$(LD) $^ -o $(OUT) $(LDFLAGS) $(LIBS)

checkdirs: $(OBJDIR)

$(OBJDIR):
	@mkdir -p $@

# Compilation rules for each file type
define make-obj
$1/%.c.o: %.c
	$(CPP) $(CPPFLAGS) $(INC) -MMD -c $$< -o $$@
$1/%.cpp.o: %.cpp
	$(CPP) $(CPPFLAGS) $(INC) -MMD -c $$< -o $$@
$1/%.o: %
	xxd -i $$< | sed 's/ src_/ /g' | $(CPP) -x c -c - -o $$@
endef
$(foreach bdir,$(OBJDIR),$(eval $(call make-obj,$(bdir))))

# Virtual targets
.PHONY: all checkdirs clean build

# Dependency files
-include $(OBJ:%.o=%.d)
