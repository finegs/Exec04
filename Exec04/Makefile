CXX      := -g++
#CXXFLAGS := -pedantic-errors -Wall -Wextra -Werror
#CXXFLAGS := -Wall -Wextra -Werror
#LDFLAGS  := -L/usr/lib -lstdc++ -lm -lws2_32 -pthread
LDFLAGS  := -lws2_32 -pthread
BUILD    := ./build
OBJ_DIR  := $(BUILD)/objects
APP_DIR  := $(BUILD)/apps
#TARGET   := s.exe
TARGET   := c.exe
INCLUDE  := -Iinclude/
SKIP_FILE := $(wildcard Test*.cpp)
#SKIP_FILE += c.cpp
SKIP_FILE += s.cpp
SRC      :=                      \
	$(wildcard src/module1/*.cpp) \
	$(wildcard src/module2/*.cpp) \
	$(wildcard src/module3/*.cpp) \
	$(wildcard src/*.cpp)         \
	$(wildcard *.cpp)
SRC		:= $(filter-out $(SKIP_FILE) , $(SRC))

OBJECTS := $(SRC:%.cpp=$(OBJ_DIR)/%.o)

all: build $(APP_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: %.cpp
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

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(APP_DIR)/*
