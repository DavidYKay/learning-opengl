EXTRA_CPPFLAGS =-g -I/usr/local/include/GLFW -Icommon
#EXTRA_LDLIBS?=-lGL
EXTRA_LDLIBS?=`pkg-config --static --libs glew glfw3`
CPPFLAGS=$(shell sdl2-config --cflags) $(shell pkg-config --static --libs glew glfw3) $(EXTRA_CPPFLAGS)
LDLIBS=$(shell sdl2-config --libs) -lGLEW $(EXTRA_LDLIBS)

# triangle: ../common-sdl2/shader_utils.o
#main: main.o
#main.o: triangle.cpp common/shader.hpp

myprogram: triangle.o shader.o
		g++ -o myprogram $(CPPFLAGS) $(EXTRA_CPPFLAGS) $(LDLIBS) triangle.o shader.o

shader.o : common/shader.cpp common/shader.hpp
		g++ -c common/shader.cpp

triangle.o : shader.o triangle.cpp
		g++ -c triangle.cpp  


#main: triangle.o shader.o

#main: shader.o triangle.o
#cc -o app triangle.cpp

#CC = g++
#CFLAGS = -Wall
#LDFLAGS =
#OBJFILES = common/shader.o triangle.o
#TARGET = stacktest
#all: $(TARGET)
#$(TARGET): $(OBJFILES)
#$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)
#clean:
#rm -f $(OBJFILES) $(TARGET) *~


# all: triangle
# test: triangle
# 		./triangle
clean:
	rm -f *.o triangle
# .PHONY: all clean
