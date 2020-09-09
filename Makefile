CXX := g++
INCLUDE := ./includes
OUTPUT := ./output
PROGRAM := geneEditor
FLAGS := -g -Wpedantic -Wextra -Wall -std=gnu++17
TIME := /usr/bin/time --format="\ttook %E %P\n"
SRC:= ./

DEFAULT: 
	$(TIME) $(MAKE) -j4 -C ./ RELEASE

RELEASE: $(OUTPUT)/$(PROGRAM).out

$(OUTPUT)/$(PROGRAM).out: $(SRC)main.cpp $(OUTPUT)/$(PROGRAM).o
	$(TIME) $(CXX) $(FLAGS) $^ -o $@ -lstdc++fs

$(OUTPUT)/$(PROGRAM).o: $(SRC)$(PROGRAM).cpp $(SRC)$(PROGRAM).hpp
	$(TIME) $(CXX) $(FLAGS) $^ -c
	mv $(PROGRAM).o $(OUTPUT)/

clean:
	rm -rf $(OUTPUT)/*.*
	rm -rf $(SRC)*.gch