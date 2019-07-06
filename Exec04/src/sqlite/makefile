CXX      := -g++
#CXXFLAGS := -pedantic-errors -Wall -Wextra -Werror
#CXXFLAGS := -Wall -Wextra -Werror
#LDFLAGS  := -L/usr/lib -lstdc++ -lm -lws2_32 -pthread
LDFLAGS  := -lws2_32 -pthread
BUILD    := ./build
OBJ_DIR  := $(BUILD)/objects
APP_DIR  := $(BUILD)/apps
#TARGET   := s.exe
TARGET   := SQLiteTest_Main.exe
INCLUDE  := -Iinclude/
SKIP_FILE := $(wildcard Test*.c)
#SKIP_FILE += c.cpp
SKIP_FILE += s.c.c s1.c
SRC      :=                      \
	$(wildcard src/module1/*.c) \
	$(wildcard src/module2/*.c) \
	$(wildcard src/module3/*.c) \
	$(wildcard src/*.c)         \
	$(wildcard *.c)
SRC		:= $(filter-out $(SKIP_FILE) , $(SRC))

OBJECTS := $(SRC:%.cpp=$(OBJ_DIR)/%.o)
OBJECTS += $(SRC:%.c=$(OBJ_DIR)/%.o)
#OBJECTS := $(TARGET:%.exe=$(OBJ_DIR)/%.o)

all: build $(APP_DIR)/$(TARGET) release_this

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<


$(APP_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $(APP_DIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) 

.PHONY: all build clean debug release

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)

debug: CXXFLAGS += -DDEBUG -g
debug: all

release: CXXFLAGS += -O2
release: all

release_this:
	@cp $(APP_DIR)/$(TARGET) $(TARGET)
	@echo copy from $(APP_DIR)/$(TARGET) to $(TARGET)

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(APP_DIR)/*
	-@rm -vf $(TARGET)
