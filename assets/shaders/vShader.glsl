#version 450
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 tex_position;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
   gl_Position = projection * view * model * vec4(vertex_position, 1.0);
   TexCoord = tex_position;
}