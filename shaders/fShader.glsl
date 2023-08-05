#version 450
in vec3 color;
in vec2 TexCoord;
out vec4 frag_color;

uniform sampler2D ourTexture;

void main() {
   frag_color = vec4(color, 1.0);
}