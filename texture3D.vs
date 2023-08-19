#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec2 nNormal;

out vec2 TexCoord;
uniform mat4 rotation;

void main()
{
   gl_Position = rotation * vec4(aPos.x, aPos.y, aPos.z, 1.0);
   TexCoord = aTexCoord;
}