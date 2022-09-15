CXX = g++
CXXFLAGS = -g -Wall $(FLAGS) -fexceptions -std=c++17 
SRC = ./src
OBJ = ./obj

TARGET = matery

SOURCES := $(wildcard $(SRC)/*.cpp)
OBJECTS := $(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SOURCES))

$(OBJ)/%.o: $(SRC)/%.cpp 
	$(CXX) -I $(SRC) -I ./mtalloctor/src -o $@ -c $^ $(CXXFLAGS)

OUT = ./matery

matery: $(OBJECTS) 
	$(CXX) $(CXXFLAGS) -o $(OUT)  $(OBJECTS) -L. -lmtalloc -Wl,-rpath=.

clean:
	rm -rf matery
	rm ./obj/*

test: matery
	$(OUT) test/union.mt
	$(OUT) test/cast.mt
	$(OUT) test/copy_move.mt
