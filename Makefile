all: bin/functions_and_macros bin/horrible_macros

bin/functions_and_macros: $(wildcard functions_and_macros/*.cpp)
	g++ functions_and_macros/*.cpp -o bin/functions_and_macros

bin/horrible_macros: horrible_macros/main.cpp
	g++ horrible_macros/main.cpp -o bin/horrible_macros

clean:
	rm -f bin/*

.PHONY: clean
