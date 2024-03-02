CXX := g++
CXX_FLAGS := -std=c++11 -g -O0

BIN := bin
SRC := src
INCLUDE := include

LIBRARIES := 
EXECUTABLE := gmm

all : $(BIN)/$(EXECUTABLE)

run : clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -I $(INCLUDE) $^ -o $@ $(LIBRARIES)

clean:
	-rm -r $(BIN)/*
