NAME 	= rogue-like
CC 		= g++
CFLAGS 	= -std=c++0x -arch i386 -Wno-deprecated -g -Wno-error=unused-command-line-argument  -Wno-error=null-conversion -Wno-return-type
ANGEL_FLAG = -D ANGEL
SRCS =	../Sources/src/Armor.cpp \
		../Sources/src/ArmorList.cpp \
		../Sources/src/Characters.cpp \
		../Sources/src/Consumable.cpp \
		../Sources/src/ContactFilter.cpp \
		../Sources/src/Elements.cpp \
		../Sources/src/Enemy.cpp \
		../Sources/src/EnemyList.cpp \
		../Sources/src/Equipment.cpp \
		../Sources/src/Game.cpp \
		../Sources/src/GameContactListener.cpp \
		../Sources/src/Hero.cpp \
		../Sources/src/Dealer.cpp \
		../Sources/src/Hitbox.cpp \
		../Sources/src/HUDTargeting.cpp \
		../Sources/src/HUDWindow.cpp \
		../Sources/src/Inventory.cpp \
		../Sources/src/LevelGenerator.cpp \
		../Sources/src/Log.cpp \
		../Sources/src/Loot.cpp \
		../Sources/src/main.cpp \
		../Sources/src/Map.cpp \
		../Sources/src/Maps.cpp \
		../Sources/src/Menu.cpp \
		../Sources/src/Object.cpp \
		../Sources/src/PassivePattern.cpp \
		../Sources/src/Pattern.cpp \
		../Sources/src/Projectile.cpp \
		../Sources/src/Quit.cpp \
		../Sources/src/Ring.cpp \
		../Sources/src/RingList.cpp \
		../Sources/src/Room.cpp \
		../Sources/src/SpecialMoves.cpp \
		../Sources/src/Tooltip.cpp \
		../Sources/src/Weapon.cpp \
		../Sources/src/WeaponList.cpp \
		../Sources/src/InGameMenu.cpp \
		../Sources/src/Shopkeeper.cpp \
		../Sources/src/Shop.cpp \
		../Sources/src/MenuCharacter.cpp \
		../Sources/src/Drug.cpp \
		../Sources/src/DrugList.cpp \
		../Sources/src/Chest.cpp \
		../Sources/src/Door.cpp 

OBJS 	 = $(SRCS:.cpp=.o)

INCLUDES = -I../Angel/Libraries/FTGL/include \
		   -I../Angel/Libraries/freetype-2.4.8/include \
		   -I../Angel/Libraries/glfw-3.0.3/include \
		   -I../Angel/Libraries/gwen/include \
		   -I../Angel/Libraries/Box2D-2.2.1/ \
		   -I../Angel/Libraries/libogg-1.3.0/include \
		   -I../Angel/Libraries/libvorbis-1.3.2/include \
		   -I../Angel/Libraries/openal-soft-1.15-bin/include \
		   -I../Angel/Libraries/FMOD/inc \
		   -I../Angel/Libraries/HID\ Utilities \
		   -I../Angel/Libraries/lua-5.2.1/src \
		   -I../Tools/jsoncpp/include/			\
		   -I../Sources/inc/

LIBS	= ../Angel/build/libAngel.a \
		  ../Angel/build/libBox2D.a \
		  ../Angel/build/libFTGL.a \
		  ../Angel/build/libFreeType.a \
		  ../Angel/build/libGLFW.a \
		  ../Angel/build/libGWEN.a \
		  ../Angel/build/libHIDUtilities.a \
		  ../Angel/build/libLua.a \
		  ../Angel/build/libOgg.a \
		  ../Angel/Scripting/Interfaces/AngelLuaWrapping.cpp

FRAMEWORKS = -framework Cocoa \
			 -framework IOKit \
			 -framework OpenGL \
			 -framework OpenAL \
			 -framework CoreFoundation \
			 -framework ForceFeedback \
			 -framework AGL

LIBS_FLAG = -lAngel -lFtgl -lGlfw -lBox2d -lFreetype -lLua -lOgg -lVorbis -lHidUtilities -lGwen -lJson -lfmodex -lfmodexL \
			-lILU -lIL -lILUT -ljpeg -llcms -lmng -ltiff -lpng -lz

LIBS_DIRS = -L../Angel/build \
			-L../Angel/Libraries/FMOD \
			-L../Tools/jsoncpp/src/lib_json/ \
			-L../Angel/Libraries/DevIL-SDK-1.7.8/mac/lib


all: $(NAME)

$(NAME): angel json $(OBJS)
	$(CC) $(CFLAGS) $(ANGEL_FLAG) -o build/$@ $(OBJS) $(INCLUDES) $(LIBS) $(FRAMEWORKS) $(LIBS_FLAG) $(LIBS_DIRS)
	cd build/ && ./rogue-like

quick: $(OBJS)
	$(CC) $(CFLAGS) $(ANGEL_FLAG) -o build/$(NAME) $^ $(INCLUDES) $(LIBS) $(FRAMEWORKS) $(LIBS_FLAG) $(LIBS_DIRS)
	cd build/ && lldb ./rogue-like

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) $(FRAMEWORKS) $(LIBS_FLAG) $(LIBS_DIRS) -o $@ -c $^

json:
	$(CC) $(CFLAGS) -I../Tools/jsoncpp/include/ -c $(wildcard ../Tools/jsoncpp/src/lib_json/*.cpp) \
		&& ar rcs libjson.a ./json_reader.o ./json_value.o ./json_writer.o && mv libjson.a ../Tools/jsoncpp/src/lib_json/.

json-clean:
	rm -rf $(wildcard *.o)

angel:
	cd ../Angel && xcodebuild -scheme Angel -project Angel.xcodeproj CONFIGURATION_BUILD_DIR='build' build

angel-clean:
	cd ../Angel && xcodebuild -scheme Angel -project Angel.xcodeproj CONFIGURATION_BUILD_DIR='build' clean

clean: angel-clean json-clean quick-clean

quick-clean:
	rm $(OBJS)

exec:
	cd build/ && lldb ./rogue-like

re: clean all
