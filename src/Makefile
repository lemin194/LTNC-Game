OBJS = main.cpp GameBase.cpp Sprite.cpp LTexture.cpp TileSet.cpp \
		Character.cpp Weapon.cpp \
		Projectile.cpp shProjectile.cpp\
		Golem.cpp Goblin.cpp Enemy.cpp StormHead.cpp\
		GameFloor.cpp Floor1.cpp\
		LTimer.cpp\
		MainMenu.cpp Button.cpp \

#CC specifies which compiler we're using
CC = g++

#INCLUDE_PATHS specifies the additional include paths we'll need
INCLUDE_PATHS = -I ..\SDL2\include\SDL2

#LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS = -L ..\SDL2\lib

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
# -Wl,-subsystem,windows gets rid of the console window
COMPILER_FLAGS = -w

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -static-libstdc++

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = ..\\bin\\main

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)