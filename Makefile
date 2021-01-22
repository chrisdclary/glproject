#
#	Makefile taken from my brother, who has a more
#	comprehensive project over at github.com/pclary/gltest
#

# Build for Windows by default
TARGET := WINDOWS

# Compilation options
INC     := -Isrc
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
CC      := x86_64-w64-mingw32-gcc
LD      := $(CC)
OUT     := testgame.exe
INC     += -ISDL2/x86_64-w64-mingw32/include 
LDFLAGS += -LSDL2/x86_64-w64-mingw32/bin
LIBS    += -lopengl32
endif

# Get list of object files
EXT     := c vert geom frag
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
	$(CC) $(CFLAGS) $(INC) -MMD -c $$< -o $$@
$1/%.o: %
	xxd -i $$< | sed 's/ src_/ /g' | $(CC) -x c -c - -o $$@
endef
$(foreach bdir,$(OBJDIR),$(eval $(call make-obj,$(bdir))))

# Virtual targets
.PHONY: all checkdirs clean build

# Dependency files
-include $(OBJ:%.o=%.d)
