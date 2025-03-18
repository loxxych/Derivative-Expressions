CXX = g++
CXXFLAGS = -std=c++20 -Wall

all: main.exe

main.exe: expression.cpp parser.cpp tests.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

test: tests.exe
	./tests.exe

tests.exe: expression.cpp parser.cpp tests.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f *.exe
