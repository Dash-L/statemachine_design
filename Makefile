EXE := test

$(EXE): $(wildcard src/*.cpp)
	$(CXX) src/*.cpp -o $(EXE)

clean:
	rm -f $(EXE)

.PHONY: clean
