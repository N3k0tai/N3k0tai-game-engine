#version 330 core
out vec4 FragColor;

in vec2 texCoord2;
uniform sampler2D ourTexture;
uniform vec4 color;

void main()
{
    FragColor = texture(ourTexture, texCoord2) * color;
}