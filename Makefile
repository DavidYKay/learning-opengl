EXTRA_CPPFLAGS =-g -I/usr/local/include/GLFW -Icommon
#EXTRA_LDLIBS?=-lGL
EXTRA_LDLIBS?=`pkg-config --static --libs glew glfw3`
CPPFLAGS=$(shell sdl2-config --cflags) $(shell pkg-config --static --libs glew glfw3) $(EXTRA_CPPFLAGS)
LDLIBS=$(shell sdl2-config --libs) -lGLEW $(EXTRA_LDLIBS)

# triangle: ../common-sdl2/shader_utils.o
#main: main.o
#main.o: triangle.cpp common/shader.hpp

#cc -o app triangle.cpp

• gcc –Wall –c main.c
• gcc –Wall –c stack.c
• gcc –Wall –o stacktest stack.o main.o

all: triangle
test: triangle
		./triangle
clean:
	rm -f *.o triangle
.PHONY: all clean
