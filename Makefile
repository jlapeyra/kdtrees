# Makefile for building kdtree and median

CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -O2
SRC = kdtree.cc median.cc
OBJ = $(SRC:.cc=.o)
TARGET = kdtree

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean