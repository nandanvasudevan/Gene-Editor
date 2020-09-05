CXX := g++
INCLUDE := ./includes
OUTPUT := ./output
PROGRAM := geneEditor
FLAGS := -g -Wpedantic -Wextra -Wall
SRC:= ./

DEFAULT: $(OUTPUT)/$(PROGRAM).out

$(OUTPUT)/$(PROGRAM).out: $(SRC)main.cpp $(OUTPUT)/$(PROGRAM).o
	$(CXX) $(FLAGS) $^ -o $@ -lstdc++fs

$(OUTPUT)/$(PROGRAM).o: $(SRC)$(PROGRAM).cpp $(SRC)$(PROGRAM).hpp
	$(CXX) $(FLAGS) $^ -c
	mv $(PROGRAM).o $(OUTPUT)/

clean:
	rm -rf $(OUTPUT)/*.*