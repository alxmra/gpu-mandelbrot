CXX := g++
CXXFLAGS := -std=c++17 -Wall -O2
LDFLAGS := -lglfw -lGLEW -lGL

TARGET := mandelbrot
SOURCES := main.cpp Shader.cpp
OBJECTS := $(SOURCES:.cpp=.o)

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)
