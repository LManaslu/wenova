NAME = game
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

TARGET = $(BIN_DIR)/$(NAME)

CC = g++
CFLAGS = -pedantic -std=c++11 -MMD -g3 -g -fPIC\
		 -W -Wall -Wextra -Wshadow -Wcast-align -Wcast-qual -Wctor-dtor-privacy\
		 -Wdisabled-optimization -Wformat=2 -Wlogical-op -Wmissing-declarations\
		 -Wmissing-include-dirs -Wnoexcept -Woverloaded-virtual -Wredundant-decls\
		 -Wsign-promo -Wstrict-null-sentinel -Wundef\
		 -Wzero-as-null-pointer-constant -Wuseless-cast -Wnon-virtual-dtor
INCLUDES = -I$(INC_DIR) `sdl2-config --cflags`
LIBS = `sdl2-config --libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer -ldl

SRC = ${wildcard $(SRC_DIR)/*.cpp}
OBJ = ${addprefix $(OBJ_DIR)/, ${notdir ${SRC:.cpp=.o}}}

RMDIR = rm -rf

#--------------------------------------------------------------
ifeq ($(OS), Windows_NT)

SDL_PATH = C:\SDL-2.0.5

INCLUDES = -Iinclude/ -I$(SDL_PATH)\include

LIBS = -L $(SDL_PATH)\lib -lSDL2main\
	   -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lm

NAME := $(NAME).exe
#--------------------------------------------------------------
else

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Darwin)

LIBS = -lm -framework SDL2 -framework SDL2_image -framework SDL2_mixer\
	   -framework SDL_TTF

#-------------------------------------------------------------

endif
endif

.PHONY: clean depend dist-clean dist

all:
#-------------------------------------------------------------
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)
	$(MAKE) $(TARGET)
#-------------------------------------------------------------

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo Building $@
	@$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

$(TARGET): $(OBJ)
	@echo Building $@
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJ) -o $@ $(LIBS)
	@echo Done.

run:
	$(TARGET)

reset:
	make dist-clean
	make -j

crun:
	make dist-clean
	make -j
	make run

arun:
	make -j
	make run

clean:
	@echo Cleaning...
	@$(RMDIR) *~ *.o

dist-clean: clean
	@$(RMDIR) $(TARGET)/$(NAME)
	@$(RMDIR) *.tar.gz $(OBJ_DIR) $(BIN_DIR)

print-%:
	@echo $* = $($*)
