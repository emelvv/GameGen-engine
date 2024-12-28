#version 450
in vec2 TexCoord;
out vec4 frag_color;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
   frag_color = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.5);
}