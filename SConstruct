orig_libs = [
'X11',
'GL',
'GLEW',
'glfw3',
# 'x11',
]

libs=[
'GL',
'GLU',
'X11',
'X11-xcb',
'Xau',
'Xcursor',
'Xdamage',
'Xdmcp',
'Xext',
'Xfixes',
'Xinerama',
'Xrandr',
'Xrender',
'Xxf86vm',
'dl',
'drm',
'glfw3',
'm',
'pthread',
'rt',
'xcb',
'xcb-dri2',
'xcb-glx',
]

lib_path = ['/usr/lib', '/usr/local/lib', ]

include_path = ['common', '/usr/local/include/GLFW']

CCFLAGS   = "-g"
LINKFLAGS = " `pkg-config --static --libs glew glfw3 ` "

env = Environment(CCFLAGS=CCFLAGS, LINKFLAGS=LINKFLAGS, CPPPATH=include_path , LIBPATH=lib_path)
#env = Environment( CPPPATH=include_path , LIBPATH=lib_path, LIBS=libs)

env.Program('triangle.cpp')
