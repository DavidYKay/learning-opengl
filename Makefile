
EXTRA_CPPFLAGS =-g
CPPFLAGS=$(shell sdl2-config --cflags) $(EXTRA_CPPFLAGS)
LDLIBS=$(shell sdl2-config --libs) -lGLEW $(EXTRA_LDLIBS)
EXTRA_LDLIBS?=-lGL

# triangle: ../common-sdl2/shader_utils.o
#main: main.o
#main.o: triangle.cpp common/shader.hpp

app: triangle common/shader.hpp
#cc -o app triangle.cpp

all: triangle
test: triangle
		./triangle
clean:
	rm -f *.o triangle
.PHONY: all clean
