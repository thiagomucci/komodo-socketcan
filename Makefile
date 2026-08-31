CXX = g++
CXXFLAGS = -Wall -Wextra -O2 -std=c++17

all: socketcan-komodo test_bridge

socketcan-komodo: bridge.cpp
	$(CXX) $(CXXFLAGS) src/bridge.cpp -o socketcan-komodo

test_bridge: tests/test_bridge.cpp
	$(CXX) $(CXXFLAGS) tests/test_bridge.cpp -o test_bridge

clean:
	rm -f socketcan-komodo test_bridge

.PHONY: all clean