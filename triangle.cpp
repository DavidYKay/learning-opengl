/* Using standard C++ output libraries */
#include <cstdlib>
#include <iostream>

/* Use glew.h instead of gl.h to get all the GL prototypes declared */
#include <GL/glew.h>
/* Using SDL2 for the base window and OpenGL context init */
#include <SDL.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

//#include "../common/shader_utils.h"

using namespace std;
using namespace glm;

GLuint64 program;
GLuint vbo_triangle, vbo_triangle_colors;
GLint attribute_coord2d, attribute_v_color;

static const GLfloat g_vertex_buffer_data[] = {
                                               -1.0f,-1.0f,-1.0f, // triangle 1 : begin
                                               -1.0f,-1.0f, 1.0f,
                                               -1.0f, 1.0f, 1.0f, // triangle 1 : end
                                               1.0f, 1.0f,-1.0f, // triangle 2 : begin
                                               -1.0f,-1.0f,-1.0f,
                                               -1.0f, 1.0f,-1.0f, // triangle 2 : end
                                               1.0f,-1.0f, 1.0f,
                                               -1.0f,-1.0f,-1.0f,
                                               1.0f,-1.0f,-1.0f,
                                               1.0f, 1.0f,-1.0f,
                                               1.0f,-1.0f,-1.0f,
                                               -1.0f,-1.0f,-1.0f,
                                               -1.0f,-1.0f,-1.0f,
                                               -1.0f, 1.0f, 1.0f,
                                               -1.0f, 1.0f,-1.0f,
                                               1.0f,-1.0f, 1.0f,
                                               -1.0f,-1.0f, 1.0f,
                                               -1.0f,-1.0f,-1.0f,
                                               -1.0f, 1.0f, 1.0f,
                                               -1.0f,-1.0f, 1.0f,
                                               1.0f,-1.0f, 1.0f,
                                               1.0f, 1.0f, 1.0f,
                                               1.0f,-1.0f,-1.0f,
                                               1.0f, 1.0f,-1.0f,
                                               1.0f,-1.0f,-1.0f,
                                               1.0f, 1.0f, 1.0f,
                                               1.0f,-1.0f, 1.0f,
                                               1.0f, 1.0f, 1.0f,
                                               1.0f, 1.0f,-1.0f,
                                               -1.0f, 1.0f,-1.0f,
                                               1.0f, 1.0f, 1.0f,
                                               -1.0f, 1.0f,-1.0f,
                                               -1.0f, 1.0f, 1.0f,
                                               1.0f, 1.0f, 1.0f,
                                               -1.0f, 1.0f, 1.0f,
                                               1.0f,-1.0f, 1.0f
}

// One color for each vertex. They were generated randomly.
static const GLfloat g_color_buffer_data[] = {
    0.583f,  0.771f,  0.014f,
    0.609f,  0.115f,  0.436f,
    0.327f,  0.483f,  0.844f,
    0.822f,  0.569f,  0.201f,
    0.435f,  0.602f,  0.223f,
    0.310f,  0.747f,  0.185f,
    0.597f,  0.770f,  0.761f,
    0.559f,  0.436f,  0.730f,
    0.359f,  0.583f,  0.152f,
    0.483f,  0.596f,  0.789f,
    0.559f,  0.861f,  0.639f,
    0.195f,  0.548f,  0.859f,
    0.014f,  0.184f,  0.576f,
    0.771f,  0.328f,  0.970f,
    0.406f,  0.615f,  0.116f,
    0.676f,  0.977f,  0.133f,
    0.971f,  0.572f,  0.833f,
    0.140f,  0.616f,  0.489f,
    0.997f,  0.513f,  0.064f,
    0.945f,  0.719f,  0.592f,
    0.543f,  0.021f,  0.978f,
    0.279f,  0.317f,  0.505f,
    0.167f,  0.620f,  0.077f,
    0.347f,  0.857f,  0.137f,
    0.055f,  0.953f,  0.042f,
    0.714f,  0.505f,  0.345f,
    0.783f,  0.290f,  0.734f,
    0.722f,  0.645f,  0.174f,
    0.302f,  0.455f,  0.848f,
    0.225f,  0.587f,  0.040f,
    0.517f,  0.713f,  0.338f,
    0.053f,  0.959f,  0.120f,
    0.393f,  0.621f,  0.362f,
    0.673f,  0.211f,  0.457f,
    0.820f,  0.883f,  0.371f,
    0.982f,  0.099f,  0.879f
};

/**
 * Display compilation errors from the OpenGL shader compiler
 */
void print_log(GLuint object) {
	GLint log_length = 0;
	if (glIsShader(object)) {
		glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
	} else if (glIsProgram(object)) {
		glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
	} else {
		cerr << "printlog: Not a shader or a program" << endl;
		return;
	}

	char* log = (char*)malloc(log_length);

	if (glIsShader(object))
		glGetShaderInfoLog(object, log_length, NULL, log);
	else if (glIsProgram(object))
		glGetProgramInfoLog(object, log_length, NULL, log);

	cerr << log;
	free(log);
}

/**
 * Store all the file's contents in memory, useful to pass shaders
 * source code to OpenGL.  Using SDL_RWops for Android asset support.
 */
char* file_read(const char* filename) {
  SDL_RWops *rw = SDL_RWFromFile(filename, "rb");
  if (rw == NULL) return NULL;

  Sint64 res_size = SDL_RWsize(rw);
  char* res = (char*)malloc(res_size + 1);

  Sint64 nb_read_total = 0, nb_read = 1;
  char* buf = res;
  while (nb_read_total < res_size && nb_read != 0) {
    nb_read = SDL_RWread(rw, buf, 1, (res_size - nb_read_total));
    nb_read_total += nb_read;
    buf += nb_read;
  }
  SDL_RWclose(rw);
  if (nb_read_total != res_size) {
    free(res);
    return NULL;
  }

  res[nb_read_total] = '\0';
  return res;
}

/**
 * Compile the shader from file 'filename', with error handling
 */
GLuint create_shader(const char* filename, GLenum type) {
	const GLchar* source = file_read(filename);
	if (source == NULL) {
		cerr << "Error opening " << filename << ": " << SDL_GetError() << endl;
		return 0;
	}
	GLuint res = glCreateShader(type);
	const GLchar* sources[] = {
                             //"#version 120\n",
                             source};
	glShaderSource(res, 1, sources, NULL);
  std::cout << source;
	//free((void*)source);
  std::cout << "About to compile shader\n";
	glCompileShader(res);
	GLint compile_ok = GL_FALSE;
	glGetShaderiv(res, GL_COMPILE_STATUS, &compile_ok);
	if (compile_ok == GL_FALSE) {
		cerr << filename << ":";
		print_log(res);
		glDeleteShader(res);
		return 0;
	}

	return res;
}

bool init_resources(void) {
  GLfloat triangle_colors[] = {
                               1.0, 1.0, 0.0,
                               0.0, 0.0, 1.0,
                               1.0, 0.0, 0.0,
  };


  GLuint colorbuffer;
  glGenBuffers(1, &colorbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

  GLint compile_ok = GL_FALSE, link_ok = GL_FALSE;

  GLuint vs, fs;
	if ((vs = create_shader("shaders/vertex.glsl", GL_VERTEX_SHADER))   == 0) return false;
	if ((fs = create_shader("shaders/fragment.glsl", GL_FRAGMENT_SHADER)) == 0) return false;

  program = glCreateProgram();
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
  if (!link_ok) {
    cerr << "Error in glLinkProgram" << endl;
    return false;
  }

  //const std::string attribute_name = "v_color";
  const GLchar * attribute_name = "v_color";
  attribute_v_color = glGetAttribLocation(program, attribute_name);
  if (attribute_v_color == -1) {
    cerr << "Could not bind attribute " << attribute_name << endl;
    return false;
  }

  return true;
}

void render(SDL_Window* window) {
  /* Clear the background as white */
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(program);

  glEnableVertexAttribArray(attribute_v_color);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle_colors);
  glVertexAttribPointer(
                        attribute_v_color, // attribute
                        3,                 // number of elements per vertex, here (r,g,b)
                        GL_FLOAT,          // the type of each element
                        GL_FALSE,          // take our values as-is
                        0,                 // no extra data between each position
                        0                  // offset of first element
                        );


  // Draw the triangle !
  //glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 indices starting at 0 -> 12 triangles -> 6 squares

  glEnableVertexAttribArray(attribute_coord2d);
  GLfloat triangle_vertices[] = {
                                 0.0,  0.8,
                                 -0.8, -0.8,
                                 0.8, -0.8,
  };
  /* Describe our vertices array to OpenGL (it can't guess its format automatically) */
  glVertexAttribPointer(
                        attribute_coord2d, // attribute
                        2,                 // number of elements per vertex, here (x,y)
                        GL_FLOAT,          // the type of each element
                        GL_FALSE,          // take our values as-is
                        0,                 // no extra data between each position
                        triangle_vertices  // pointer to the C array
                        );

  /* Push each element in buffer_vertices to the vertex shader */
  glDrawArrays(GL_TRIANGLES, 0, 3);

  // glDisableVertexAttribArray(attribute_v_color);
  glDisableVertexAttribArray(attribute_coord2d);

  /* Display the result */
  SDL_GL_SwapWindow(window);
}

void free_resources() {
  glDeleteProgram(program);
}

void mainLoop(SDL_Window* window) {
  while (true) {
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
      if (ev.type == SDL_QUIT)
        return;
    }
    render(window);
  }
}

int main(int argc, char* argv[]) {
  /* SDL-related initialising functions */
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window* window = SDL_CreateWindow("My First Triangle",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    640, 480,
    SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
  SDL_GL_CreateContext(window);

  /* Extension wrangler initialising */
  GLenum glew_status = glewInit();
  if (glew_status != GLEW_OK) {
    cerr << "Error: glewInit: " << glewGetErrorString(glew_status) << endl;
    return EXIT_FAILURE;
  }

  /* When all init functions run without errors,
     the program can initialise the resources */
  if (!init_resources())
    return EXIT_FAILURE;

  /* We can display something if everything goes OK */
  mainLoop(window);

  /* If the program exits in the usual way,
     free resources and exit with a success */
  free_resources();
  return EXIT_SUCCESS;
}
