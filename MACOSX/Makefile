NAME 	= rogue-like
CC 		= g++
CFLAGS 	= -std=c++0x -arch i386 -Wno-deprecated
ANGEL_FLAG = -D ANGEL
SRCS 	= ../Sources/src/Elements.cpp \
		  ../Sources/src/Game.cpp \
		  ../Sources/src/Weapon.cpp \
		  ../Sources/src/Shooter.cpp \
		  ../Sources/src/Projectile.cpp \
		  ../Sources/src/Hero.cpp \
		  ../Sources/src/Maps.cpp \
		  ../Sources/src/main.cpp \
		  ../Angel/Scripting/Interfaces/AngelLuaWrapping.cpp \
		  ../Sources/src/GameContactListener.cpp \
		  ../Sources/src/ContactFilter.cpp

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
		   -I../Tools/jsoncpp/include/

LIBS	= ../Angel/build/libAngel.a \
		  ../Angel/build/libBox2D.a \
		  ../Angel/build/libFTGL.a \
		  ../Angel/build/libFreeType.a \
		  ../Angel/build/libGLFW.a \
		  ../Angel/build/libGWEN.a \
		  ../Angel/build/libHIDUtilities.a \
		  ../Angel/build/libLua.a \
		  ../Angel/build/libOgg.a

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

$(NAME): angel json
	$(CC) $(CFLAGS) $(ANGEL_FLAG) $(SRCS) $(INCLUDES) $(LIBS) $(FRAMEWORKS) $(LIBS_FLAG) $(LIBS_DIRS) -o build/$(NAME)
	cd build/ && ./rogue-like

quick:
	$(CC) $(CFLAGS) $(ANGEL_FLAG) $(SRCS) $(INCLUDES) $(LIBS) $(FRAMEWORKS) $(LIBS_FLAG) $(LIBS_DIRS) -o build/$(NAME)
	cd build/ && ./rogue-like

json:
	$(CC) $(CFLAGS) -I../Tools/jsoncpp/include/ -c $(wildcard ../Tools/jsoncpp/src/lib_json/*.cpp) \
		&& ar rcs libjson.a ./json_reader.o ./json_value.o ./json_writer.o && mv libjson.a ../Tools/jsoncpp/src/lib_json/.

json-clean:
	rm -rf $(wildcard *.o)

angel:
	cd ../Angel && xcodebuild -scheme Angel -project Angel.xcodeproj CONFIGURATION_BUILD_DIR='build' build

angel-clean:
	cd ../Angel && xcodebuild -scheme Angel -project Angel.xcodeproj CONFIGURATION_BUILD_DIR='build' clean

clean: angel-clean json-clean

exec:
	cd ../Angel/build/ && ./rogue-like

re: clean all
