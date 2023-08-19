#pragma 
//texture mapping
//2D
//const char* vertexShaderSource = "#version 330 core\n"
//"layout(location = 0) in vec3 aPos;\n"
//"layout(location = 1) in vec3 aColor;\n"
//"layout(location = 2) in vec2 aTexCoord;\n"
//
//"out vec3 ourColor;\n"
//"out vec2 TexCoord;\n"
//
//"void main()\n"
//"{\n"
//"gl_Position = vec4(aPos, 1.0);\n"
//"ourColor = aColor;\n"
//"TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
//"}\n\0";

//3D
//const char* vertexShaderSource = "#version 330 core\n"
//"layout (location = 0) in vec3 aPos;\n"
//"layout (location = 1) in vec2 aTexCoord;\n"
//"layout (location = 2) in vec3 aNormal;\n"
//
//"out vec2 TexCoord;\n"
//"uniform mat4 rotation;\n"
//
//"void main()\n"
//"{\n"
//"   gl_Position = rotation * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
//"   TexCoord = aTexCoord;\n"
//"}\0";
//3D
//const char* fragmentShaderSource = "#version 330 core\n"
//"out vec4 FragColor;\n"
//"in vec2 TexCoord;\n"
//// texture samplers
//"uniform sampler2D texture1;\n"
//"uniform sampler2D texture2;\n"
//
//"void main()\n"
//"{\n"
//// linearly interpolate between both textures (60% container, 40% awesomeface)
//"   FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.4);\n"
//"}\n\0";
//3D
//texture mapping

//cube mapping
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"

"out vec3 TexCoord;\n"

"uniform mat4 projection;\n"
"uniform mat4 view;\n"

"void main()\n"
"{\n"
"   TexCoord = aPos;\n"
"   gl_Position = projection * view * vec4(aPos, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 TexCoord;\n"
// texture samplers
"uniform samplerCube skybox;\n"

"void main()\n"
"{\n"
"  FragColor = texture(skybox, TexCoord);\n"
"}\n\0";
//cube mapping