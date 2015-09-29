ANGEL_DISABLE_FMOD := $(shell sed -rn 's/^[[:space:]]*\#define[[:space:]]+ANGEL_DISABLE_FMOD[[:space:]]+([[:digit:]])[[:space:]]*$$/\1/p' Angel/AngelConfig.h)
ANGEL_DISABLE_DEVIL := $(shell sed -rn 's/^[[:space:]]*\#define[[:space:]]+ANGEL_DISABLE_DEVIL[[:space:]]+([[:digit:]])[[:space:]]*$$/\1/p' Angel/AngelConfig.h)
CXX = g++ -std=c++0x -g -D ANGEL
TARGET = rogue-like
ANGEL_FLAGS = -D ANGEL_RELEASE
ARCH := $(shell uname -m)
ALLEGRO_LIBS := $(shell allegro-config --libs 2>/dev/null)
CWD := $(shell pwd)
CODE_DIR := $(shell dirname "$(CWD)")
LIBANGEL = Angel/libangel.a
LUA = Angel/Libraries/angel-lua-build/lua
WRAPPER = Angel/Scripting/Interfaces/AngelLuaWrapping.cpp

INCLUDE = 							\
	-IAngel						\
	-IAngel/Libraries/glfw-3.0.3/include			\
	-IAngel/Libraries/Box2D-2.2.1			\
	-IAngel/Libraries/FTGL/include			\
	-IAngel/Libraries/lua-5.2.1/src			\
	-I/usr/include/freetype2				\
	-I./Tools/jsoncpp/include				\
	-ISources/inc/							\
	-ILibraries/DevIL-SDK-1.7.8/mac/include/ \
	-IAngel/Infrastructure/

ifneq ($(ANGEL_DISABLE_FMOD), 1)
	INCLUDE += -IAngel/Libraries/FMOD/inc
endif

LIBS = 									\
	$(LIBANGEL)							\
	Angel/Libraries/glfw-3.0.3/src/libglfw3.a			\
	Angel/Libraries/Box2D-2.2.1/Build/Box2D/libBox2D.a		\
	Angel/Libraries/FTGL/unix/src/.libs/libftgl.a		\
Angel/Libraries/gwen/lib/linux/gmake/libgwen_static.a	\
	Angel/Libraries/angel-lua-build/liblua.a \
	Tools/jsoncpp/src/lib_json/libjsoncpp.a \
	#Angel/Libraries/DevIL-SDK-1.7.8/srcs/devil-1.7.8/lib/.libs/libIL.a

ifneq ($(ANGEL_DISABLE_FMOD), 1)
	ifeq ($(ARCH),x86_64)
		LIBS += Angel/Libraries/FMOD/lib/libfmodex64.so
	else
		LIBS += Angel/Libraries/FMOD/lib/libfmodex.so
	endif
endif

SHLIBS = -lGL -lGLU -ldl -lfreetype -lXrandr -lX11 -lpthread -lrt -lXxf86vm -lXi
SHLIBS += $(ALLEGRO_LIBS)
ifeq ($(ANGEL_DISABLE_FMOD), 1)
	SHLIBS += -lopenal -lvorbisfile
endif
ifneq ($(ANGEL_DISABLE_DEVIL),1)
	SHLIBS += -lIL -lILU -lILUT
else
	SHLIBS += -lpng
endif

SYSSRCS = 							\
	$(WRAPPER)

SRCS =	./Sources/src/Armor.cpp \
		./Sources/src/ArmorList.cpp \
		./Sources/src/Characters.cpp \
		./Sources/src/Consumable.cpp \
		./Sources/src/ContactFilter.cpp \
		./Sources/src/Elements.cpp \
		./Sources/src/Enemy.cpp \
		./Sources/src/EnemyList.cpp \
		./Sources/src/Equipment.cpp \
		./Sources/src/Game.cpp \
		./Sources/src/GameContactListener.cpp \
		./Sources/src/Hero.cpp \
		./Sources/src/Dealer.cpp \
		./Sources/src/Hitbox.cpp \
		./Sources/src/HUDTargeting.cpp \
		./Sources/src/HUDWindow.cpp \
		./Sources/src/Inventory.cpp \
		./Sources/src/LevelGenerator.cpp \
		./Sources/src/Log.cpp \
		./Sources/src/Loot.cpp \
		./Sources/src/main.cpp \
		./Sources/src/Map.cpp \
		./Sources/src/Maps.cpp \
		./Sources/src/Menu.cpp \
		./Sources/src/Object.cpp \
		./Sources/src/PassivePattern.cpp \
		./Sources/src/Pattern.cpp \
		./Sources/src/Projectile.cpp \
		./Sources/src/Quit.cpp \
		./Sources/src/Ring.cpp \
		./Sources/src/RingList.cpp \
		./Sources/src/Room.cpp \
		./Sources/src/SpecialMoves.cpp \
		./Sources/src/SpecialAttack.cpp \
		./Sources/src/Tooltip.cpp \
		./Sources/src/Weapon.cpp \
		./Sources/src/WeaponList.cpp \
		./Sources/src/InGameMenu.cpp \
		./Sources/src/Shopkeeper.cpp \
		./Sources/src/Shop.cpp \
		./Sources/src/MenuCharacter.cpp \
		./Sources/src/Drug.cpp \
		./Sources/src/Chest.cpp \
		./Sources/src/Door.cpp \
		./Sources/src/DrugList.cpp \
		./Sources/src/ControllerInputManager.cpp \
		./Sources/src/Boss.cpp

SYSOBJS = $(patsubst %.cpp,%.o,$(SYSSRCS))
OBJS = $(patsubst %.cpp,%.o,$(SRCS))
HOST = $(shell hostname)

.PHONY: clean all SWIG-Wrapper

%.o: %.cpp
	$(CXX) -c $(INCLUDE) -Wno-write-strings -Wno-deprecated -o $@ $^

all: $(TARGET)

publish: $(TARGET)
	$(LUA) Tools/BuildScripts/publish_linux.lua -i $(CWD) -e ClientGame

SWIG-Wrapper:
	$(LUA) Tools/BuildScripts/swig_wrap.lua -p "$(CWD)"

$(WRAPPER): SWIG-Wrapper

jsoncpp:
	if [ ! -f Sources/inc/Key.hpp ] ; \
	then \
			echo -n "#define KEY \"" >> Sources/inc/Key.hpp ; \
			echo -n "$(USER)@$(HOST)" | md5sum | cut -d' ' -f1 | xargs echo -n >> Sources/inc/Key.hpp ; \
			echo "\"" >> Sources/inc/Key.hpp ; \
	fi;
	cd Tools/jsoncpp && cmake . && make

$(TARGET): $(LIBANGEL) jsoncpp $(OBJS) $(SYSOBJS) $(WRAPPER)
	$(CXX) -o $@ $(OBJS) $(SYSOBJS) $(LIBS) $(SHLIBS) $(ANGEL_FLAGS)
	cp -p Angel/Scripting/EngineScripts/*.lua Resources/Scripts

clean:
	rm -f $(OBJS) $(SYSOBJS) $(TARGET)

$(LIBANGEL):
	cd Angel && make

