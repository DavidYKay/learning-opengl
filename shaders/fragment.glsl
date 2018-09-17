#version 300 es
precision mediump float;
varying vec3 f_color;
out vec4 color;
void main(){
  //color = vec4(1,0,0,1);
  color = vec4(f_color.r, f_color.g, f_color.b, 1.0);
}
