# Build the campus program.
# On the UNT CSE CELL machines: make
# Then run: ./campus
# Or:       ./campus testdata/collision

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -Iinclude

SRCS = src/main.cpp src/StringUtil.cpp src/Facility.cpp src/HashTable.cpp src/FacilityDatabase.cpp

campus: $(SRCS)
	$(CXX) $(CXXFLAGS) -o campus $(SRCS)

clean:
	rm -f campus src/*.o *.o
