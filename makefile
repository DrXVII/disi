CXX = g++
CC = gcc
LD = g++
#LD = gcc
CXX_F = -std=c++17 -Wall -Wextra
CC_F = -std=c99 -Wall -Wextra
CC_DBG_F = -ggdb -DDEBUG
CXX_DBG_F = -ggdb -DDEBUG
NAME = disi_dbg
_OBJ = main.o
OBJ_D = o
OBJ = $(patsubst %, $(OBJ_D)/%, $(_OBJ))
SRC_D = src
INC_D = inc
INC = -I $(INC_D)
LIBS =

NAME_REL = disi
OBJ_D_REL = o/rel
OBJ_REL = $(patsubst %, $(OBJ_D_REL)/%, $(_OBJ))
CC_REL_F = -O2
CXX_REL_F = -O2

all: $(OBJ_D) $(NAME)

$(NAME): $(OBJ)
	echo "LD $@"
	$(LD) -o $@ $^  $(LIBS)

$(OBJ_D):
	echo "MKDIR $@"
	mkdir -p $@

$(OBJ_D)/%.o: $(SRC_D)/%.c
	echo "CC $@"
	$(CC) -o $@ -c $(CC_F) $(CC_DBG_F) $(INC) $<

$(OBJ_D)/%.o: $(SRC_D)/%.cpp
	echo "CXX $@"
	$(CXX) -o $@ -c $(CXX_F) $(CXX_DBG_F) $(INC) $<

#-------------------------------------------------------------------------------
release: $(OBJ_D_REL) $(NAME_REL)

$(NAME_REL): $(OBJ_REL)
	echo "LD $@"
	$(LD) -o $@ $^ $(LIBS)

$(OBJ_D_REL):
	echo "MKDIR $@"
	mkdir -p $@

$(OBJ_D_REL)/%.o: $(SRC_D)/%.c
	echo "CC $@"
	$(CC) -o $@ -c $(CC_F) $(CC_REL_F) $(INC) $<

$(OBJ_D_REL)/%.o: $(SRC_D)/%.cpp
	echo "CXX $@"
	$(CXX) -o $@ -c $(CXX_F) $(CXX_REL_F) $(INC) $<

#-------------------------------------------------------------------------------
ctags:
	bash -c "ctags -R {src,inc}/* lib/*/*.{c,h}"

clean:
	rm -rfv $(OBJ_D_REL)
	rm -rfv $(OBJ_D)
	rm -vf $(NAME_REL)
	rm -vf $(NAME)
