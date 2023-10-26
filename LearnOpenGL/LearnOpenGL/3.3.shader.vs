#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

out vec3 ourColor;

uniform float xOffset;
out vec4 vertexPosition;

void main()
{
	vertexPosition = gl_Position = vec4(aPos.x + xOffset, -aPos.y, aPos.z, 1.0);
	ourColor = aColor;
}