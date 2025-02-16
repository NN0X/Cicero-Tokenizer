CXX = clang++
CFLAGS = -O3 -Wall -Wextra -Wpedantic
SRC = src
OUT = cicero

all:
	rm -f -r build
	mkdir build
	mkdir build/release
	$(CXX) $(CFLAGS) $(SRC)/*.cpp -o build/release/$(OUT) 2> build/release/make.log
	cp -f -r dictionaries build/release/dictionaries
	cp *.md build/release/
	
	mkdir build/lib
	$(CXX) $(CFLAGS) -c $(filter-out $(SRC)/main.cpp, $(wildcard $(SRC)/*.cpp)) 2> build/lib/make.log
	ar rcs build/lib/libcicero.a *.o
	rm *.o
	cp -f -r dictionaries build/lib/dictionaries
	cp src/cicero.h build/lib/cicero.h
	cp *.md build/lib/

	@echo "Build complete. Executable is located at build/release/$(OUT) and library is located at build/lib/libcicero.a"

release:
	rm -f -r build
	mkdir build
	$(CXX) $(CFLAGS) $(SRC)/*.cpp -o build/$(OUT) 2> build/make.log
	cp -f -r resources build/resources
	cp -f -r dictionaries build/dictionaries
	cp *.md build/
	@echo "Build complete. Executable is located at build/$(OUT)"

static:
	rm -f -r build
	mkdir build
	$(CXX) $(CFLAGS) -c $(filter-out $(SRC)/main.cpp, $(wildcard $(SRC)/*.cpp)) 2> build/make.log
	ar rcs build/libcicero.a *.o
	rm *.o
	cp -f -r dictionaries build/dictionaries
	cp src/cicero.h build/cicero.h
	cp *.md build/
	@echo "Build complete. Library is located at build/libcicero.a"
