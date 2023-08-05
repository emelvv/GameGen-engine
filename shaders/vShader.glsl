#version 450
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_color;
layout(location = 2) in vec2 tex_position;
out vec3 coxlor;x
out vec2 TexCoord;
void main() {
   color = vertex_color;
   gl_Position = vec4(vertex_position, 1.0);
   TexCoord = tex_position;
}