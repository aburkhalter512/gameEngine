# Project Directories
SRC_DIR=src
OBJ_DIR=obj
BIN_DIR=bin
LIB_DIR=lib
RES_DIR=resources

# General Include Directoroes
INC_DIRS=$(patsubst %, -I%, include /usr/local/include)

# Game libraries and frameworks
GAME_EXE_LIB_DIRS=$(patsubst %, -L%, /usr/local/lib bin)
GAME_EXE_LIBS=$(patsubst %, -l%, iconv SDL2 c png GameEngine)
GAME_EXE_FRAMEWORKS=$(patsubst %, -framework %, OpenGL Foundation AppKit CoreAudio AudioToolbox CoreVideo IOKit ForceFeedback Carbon)

# Game engine libraries and frameworks
GAME_ENGINE_LIB_DIRS=$(patsubst %, -L%, /usr/local/lib)
GAME_ENGINE_LIBS=$(patsubst %, -l%,)
GAME_ENGINE_FRAMEWORKS=$(patsubst %, -framework %, OpenGL)

# Unit test libraries and frameworks
TEST_LIB_DIRS=$(patsubst %, -L%, /usr/local/lib bin)
TEST_LIBS=$(patsubst %, -l%, c png GameEngine)
TEST_FRAMEWORKS=$(patsubst %, -framework %, OpenGL)

# Binaries
GAME_LIB=libGameEngine.a
GAME_EXE=gameWithAnAwesomeName.out
TEST_EXE=unitTest.out

# Source File Paths (do not include $(SRC_DIR))
DATASTRUCTURE_FILES=datastructures/hashtable.c
DATASTRUCTURE_TEST_FILES=datastructures/unit/hashtable.unit.c

ENGINE_FILES=engine/collision.c engine/util.c engine/gameEnvironment.c engine/gameObject.c engine/render.c engine/texture.c
ENGINE_TEST_FILES=$(patsubst %, engine/unit/%, collision.unit.c)

ENGINE_MATH_FILES=engine/math/float.c engine/math/vec.c engine/math/matrix.c engine/math/polygon.c engine/math/transform.c
ENGINE_TEST_MATH_FILES=$(patsubst %, engine/unit/math/%, float.unit.c matrix.unit.c polygon.unit.c transform.unit.c vec.unit.c)

UTIL_FILES=util/string.c util/loadShaders.c util/msTimer.c

# File Groups
FILES=$(DATASTRUCTURE_FILES) $(ENGINE_MATH_FILES) $(ENGINE_FILES) $(UTIL_FILES)
TEST_FILES=util/unit.c $(ENGINE_TEST_FILES) $(ENGINE_TEST_MATH_FILES) $(DATASTRUCTURE_TEST_FILES)

# Main file groups
GAME_FILES=main.c game/paddle.c game/border.c game/ball.c
MAIN_TEST_FILES=main.unit.c $(FILES) $(TEST_FILES)

# This variable defines all files that can be compiled in any given reciped
ALL_FILES=$(GAME_FILES) main.unit.c $(FILES) $(TEST_FILES)

# Compilation and Linking Paths
LIB_SRC_PATHS=$(patsubst %, $(SRC_DIR)/%, $(FILES))
LIB_OBJ_PATHS=$(patsubst %.c, $(OBJ_DIR)/%.o, $(FILES))

TEST_SRC_PATHS=$(patsubst %, $(SRC_DIR)/%, $(MAIN_TEST_FILES))
TEST_OBJ_PATHS=$(patsubst %.c, $(OBJ_DIR)/%.o, $(MAIN_TEST_FILES))

GAME_SRC_PATHS=$(patsubst %, $(SRC_DIR)/%, $(GAME_FILES))
GAME_OBJ_PATHS=$(patsubst %.c, $(OBJ_DIR)/%.o, $(GAME_FILES))

ALL_OBJ_PATHS=$(patsubst %.c, $(OBJ_DIR)/%.o, $(ALL_FILES))

# Compiler Variables
CC=gcc

# Base flags
BASE_CFLAGS=-c -Wall $(INC_DIRS) -mmacosx-version-min=10.9
BASE_LFLAGS=

# Build variant flags
DEBUG_CFLAGS=-g -O0
DEBUG_LFLAGS=

TEST_CFLAGS=-g -O0 -DUNIT_TEST
TEST_LFLAGS=

RELEASE_CFLAGS=-O3
RELEASE_LFLAGS=

.PHONY: all clean runTest build buildRelease buildDebug setTargetDebug setTargetTest setTargetRelease .buildLib .buildTest .buildGame

# Main Recipes
all: runTest

clean:
	rm -rf bin/*
	rm -rf obj/*

runTest: buildTest
	"$(BIN_DIR)/$(TEST_EXE)"

build: buildRelease

# Build Variant Recipes
buildRelease: BUILD_VARIANT_CFLAGS=$(BASE_CFLAGS) $(RELEASE_CFLAGS)
buildRelease: BUILD_VARIANT_LFLAGS=$(BASE_LFLAGS) $(RELEASE_LFLAGS)
buildRelease: clean .buildLib

buildDebug: export BUILD_VARIANT_CFLAGS=$(BASE_CFLAGS) $(DEBUG_CFLAGS)
buildDebug: export BUILD_VARIANT_LFLAGS=$(BASE_LFLAGS) $(DEBUG_LFLAGS)
buildDebug: clean .buildLib

buildTest: export BUILD_VARIANT_CFLAGS=$(BASE_CFLAGS) $(TEST_CFLAGS)
buildTest: export BUILD_VARIANT_LFLAGS=$(BASE_LFLAGS) $(TEST_LFLAGS)
buildTest: clean .buildLib .buildTest

buildGame: BUILD_VARIANT_CFLAGS=$(BASE_CFLAGS) $(RELEASE_CFLAGS)
buildGame: BUILD_VARIANT_LFLAGS=$(BASE_LFLAGS) $(RELEASE_LFLAGS)
buildGame: clean .buildGame

# Project Recipes
.buildLib: export REAL_CFLAGS=$(BUILD_VARIANT_CFLAGS)
.buildLib: export REAL_LFLAGS=$(BUILD_VARIANT_LFLAGS) $(GAME_ENGINE_FRAMEWORKS) $(GAME_ENGINE_LIB_DIRS) $(GAME_ENGINE_LIBS)
.buildLib: $(LIB_SRC_PATHS) $(LIB_OBJ_PATHS)
	libtool -static $(REAL_LFLAGS) $(LIB_OBJ_PATHS) -o "$(BIN_DIR)/$(GAME_LIB)"

.buildTest: export REAL_CFLAGS=$(BUILD_VARIANT_CFLAGS)
.buildTest: export REAL_LFLAGS=$(BUILD_VARIANT_LFLAGS) $(TEST_FRAMEWORKS) $(TEST_LIB_DIRS) $(TEST_LIBS)
.buildTest: $(TEST_SRC_PATHS) $(TEST_OBJ_PATHS)
	$(CC) $(TEST_OBJ_PATHS) $(REAL_LFLAGS) -o "$(BIN_DIR)/$(TEST_EXE)"

.buildGame: .buildLib
.buildGame: REAL_CFLAGS=$(BUILD_VARIANT_CFLAGS)
.buildGame: REAL_LFLAGS=$(BUILD_VARIANT_LFLAGS) $(GAME_EXE_FRAMEWORKS) $(GAME_EXE_LIB_DIRS) $(GAME_EXE_LIBS)
.buildGame: $(GAME_SRC_PATHS) $(GAME_OBJ_PATHS)
	$(CC) $(GAME_OBJ_PATHS) $(REAL_LFLAGS) -o "$(BIN_DIR)/$(GAME_EXE)"
	cp -r "$(RES_DIR)" "$(BIN_DIR)/resources"

# Ensures that the obj file's directory exists
# -AND-
# Compiles .c --> .o
# 	The patsubst is needed since the pattern rule cannot automatically
# 	generate the necessary source file dependency. It replaces $(OBJ_DIR)/ with $(SRC_DIR)/
# 	and modifies the file extension.
#
# 	This bit of black magic allows you to create complex folder structures inside $(SRC_DIR)
# 	and easily compile it. The alternative solution is to manually specify each
# 	other source file recipe manually.
$(ALL_OBJ_PATHS):
	mkdir -p $(dir $@)

	$(CC) $(REAL_CFLAGS) $(patsubst $(OBJ_DIR)/%.o, "$(SRC_DIR)/%.c", $@) -o "$@"
