#version 330 core
layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 texCoord;
out vec2 texCoord2;

uniform vec4 camInfo;
uniform vec2 wSize;
uniform int isScreenCoordinates;


void main()
{
    vec2 pos2;
    if (isScreenCoordinates == 0)
    {
        pos2 = vec2((((pos.x - camInfo.x) / wSize.x) * 2.0 * camInfo.z) - 1.0, (((-pos.y + camInfo.y) / wSize.y) * 2.0 * camInfo.w) + 1.0);
    }
    else
    {
        pos2 = vec2(((pos.x / wSize.x) * 2.0) - 1.0, ((-pos.y / wSize.y) * 2.0) + 1.0);
    }
    gl_Position = vec4(pos2, 0.0, 1.0);
    texCoord2 = texCoord;
}