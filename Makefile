CPPFLAGS=$(shell sdl2-config --cflags) -g -I/usr/local/include/GLFW -Icommon 
LDLIBS=$(shell sdl2-config --libs) -lGLEW -lglfw3 -L/usr/local/lib -lGLEW -lGLU -lm -lGL -lm -lpthread -pthread -ldl -ldrm -lXdamage -lX11-xcb -lxcb-glx -lxcb-dri2 -lglfw3 -lrt -lm -ldl -lXrandr -lXinerama -lXxf86vm -lXext -lXcursor -lXrender -lXfixes -lX11 -lpthread -lxcb -lXau -lXdmcp

# triangle: ../common-sdl2/shader_utils.o
#main: main.o
#main.o: triangle.cpp common/shader.hpp

myprogram:
		g++ -o myprogram $(CPPFLAGS) $(LDLIBS) triangle.cpp common/shader.cpp


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
	rm -f *.o myprogram
# .PHONY: all clean
