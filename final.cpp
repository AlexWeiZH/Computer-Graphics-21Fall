#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <conio.h>
#include <GLM/glm/glm.hpp>
#include <GLM/glm/gtx/transform.hpp> 
#include <GLM/glm/gtc/matrix_transform.hpp>
#include <GLM/glm/gtc/type_ptr.hpp>
#include <stb_image.h>
#include "shader.h"


using namespace std;


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

class Dot {
public:
    float x, y, z;

    Dot(float a, float b, float c) : x(a), y(b), z(c) {};
};
//With normal
bool loadOBJ(const char* filepath, std::vector<float> &out_vertices, std::vector<unsigned int> &out_indices, unsigned int& vertexCount, int &triangleCount)
{
    vector<string*> coord;
    vector<Dot*> vertex;
    vector<Dot*> normals;
    vector<Dot*> texture;
    vector<unsigned int*> vertexIndices, uvIndices, normalIndices;


    bool hasVn, hasVt;
    hasVn = hasVt = false;

    std::ifstream in(filepath);
    if (!in.is_open())
    {
        cout << "Cannot Open File" << endl;
        return false;
    }
    char buffer[256];           //file read buffer
    while (!in.eof())
    {

        in.getline(buffer, 256);

        coord.push_back(new string(buffer));
        // read the first word of the line

    }
    for (int i = 0; i < coord.size(); i++)
    {
        if ((*coord[i])[0] == '#')
        {
            continue;
        }
        else if ((*coord[i])[0] == 'v' && (*coord[i])[1] == ' ')                             //vertex coordinates
        {
            float tmpx, tmpy, tmpz;
            sscanf_s(coord[i]->c_str(), "v %f %f %f", &tmpx, &tmpy, &tmpz);
            vertex.push_back(new Dot(tmpx, tmpy, tmpz));
        }
        else if ((*coord[i])[0] == 'v' && (*coord[i])[1] == 't')                             //texture
        {
            hasVt = true;
            float tmpx, tmpy, tmpz;
            sscanf_s(coord[i]->c_str(), "vt %f %f %f", &tmpx, &tmpy, &tmpz);
            texture.push_back(new Dot(tmpx, tmpy, tmpz));
        }
        else if ((*coord[i])[0] == 'v' && (*coord[i])[1] == 'n')                             //normal
        {
            hasVn = true;
            float tmpx, tmpy, tmpz;
            sscanf_s(coord[i]->c_str(), "vn %f %f %f", &tmpx, &tmpy, &tmpz);
            normals.push_back(new Dot(tmpx, tmpy, tmpz));

        }
        else if ((*coord[i])[0] == 'f')                                                   //faces
        {
            if (count(coord[i]->begin(), coord[i]->end(), ' ') == 3)                      //Triangle
            {
                triangleCount++;
                if (hasVt == true && hasVn == true)
                {
                    int vertexIndex1, uvIndex1, normalIndex1, vertexIndex2, uvIndex2, normalIndex2, vertexIndex3, uvIndex3, normalIndex3;

                    sscanf_s(coord[i]->c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &vertexIndex1, &uvIndex1, &normalIndex1, &vertexIndex2, &uvIndex2, &normalIndex2, &vertexIndex3, &uvIndex3, &normalIndex3);
                    vertexIndices.push_back(new unsigned int(vertexIndex1 - 1));
                    vertexIndices.push_back(new unsigned int(vertexIndex2 - 1));
                    vertexIndices.push_back(new unsigned int(vertexIndex3 - 1));

                    uvIndices.push_back(new unsigned int(uvIndex1 - 1));
                    uvIndices.push_back(new unsigned int(uvIndex2 - 1));
                    uvIndices.push_back(new unsigned int(uvIndex3 - 1));

                    normalIndices.push_back(new unsigned int(normalIndex1 - 1));
                    normalIndices.push_back(new unsigned int(normalIndex2 - 1));
                    normalIndices.push_back(new unsigned int(normalIndex3 - 1));
                }
                else if (hasVt == false && hasVn == true)
                {
                    int vertexIndex1, normalIndex1, vertexIndex2, normalIndex2, vertexIndex3, normalIndex3;
                    sscanf_s(coord[i]->c_str(), "f %d//%d %d//%d %d//%d", &vertexIndex1, &normalIndex1, &vertexIndex2, &normalIndex2, &vertexIndex3, &normalIndex3);
                    vertexIndices.push_back(new unsigned int(vertexIndex1 - 1));
                    vertexIndices.push_back(new unsigned int(vertexIndex2 - 1));
                    vertexIndices.push_back(new unsigned int(vertexIndex3 - 1));

                    normalIndices.push_back(new unsigned int(normalIndex1 - 1));
                    normalIndices.push_back(new unsigned int(normalIndex2 - 1));
                    normalIndices.push_back(new unsigned int(normalIndex3 - 1));

                }
                else if (hasVt == false && hasVn == false)
                {
                    int vertexIndex1, vertexIndex2, vertexIndex3;
                    sscanf_s(coord[i]->c_str(), "f %d %d %d", &vertexIndex1, &vertexIndex2, &vertexIndex3);
                    vertexIndices.push_back(new unsigned int(vertexIndex1 - 1));
                    vertexIndices.push_back(new unsigned int(vertexIndex2 - 1));
                    vertexIndices.push_back(new unsigned int(vertexIndex3 - 1));
                }

            }
        }
        else { continue; }
    }

    glm::vec3 vertices;
    glm::vec2 uvs;
    vertexCount = (unsigned int)vertex.size();

    for (unsigned int i = 0; i < vertex.size(); i++)
    {
        vertices.x = vertex[i]->x * 0.2f;
        vertices.y = vertex[i]->y * 0.2f;
        vertices.z = vertex[i]->z * 0.2f;
        out_vertices.push_back(vertices.x);
        out_vertices.push_back(vertices.y);
        out_vertices.push_back(vertices.z);

        uvs.x = texture[i]->x;
        uvs.y = texture[i]->y;
        out_vertices.push_back(uvs.x);
        out_vertices.push_back(uvs.y);

        vertices.x = normals[i]->x;
        vertices.y = normals[i]->y;
        vertices.z = normals[i]->z;
        out_vertices.push_back(vertices.x);
        out_vertices.push_back(vertices.y);
        out_vertices.push_back(vertices.z);
    }

    for (int i = 0; i < vertexIndices.size(); i+=3)
    {
        unsigned int vertexIndex1 = *vertexIndices[i];
        unsigned int vertexIndex2 = *vertexIndices[i+1];
        unsigned int vertexIndex3 = *vertexIndices[i+2];
        out_indices.push_back(vertexIndex1);
        out_indices.push_back(vertexIndex2);
        out_indices.push_back(vertexIndex3);
    }

    for (int i = 0; i < coord.size(); i++) { delete coord[i]; }
    for (int i = 0; i < vertex.size(); i++) { delete vertex[i]; }
    for (int i = 0; i < normals.size(); i++) { delete normals[i]; }
    for (int i = 0; i < texture.size(); i++) { delete texture[i]; }
    for (int i = 0; i < vertexIndices.size(); i++) { delete vertexIndices[i]; }
    for (int i = 0; i < uvIndices.size(); i++) { delete uvIndices[i]; }
    for (int i = 0; i < normalIndices.size(); i++) { delete normalIndices[i]; }

    return true;
}

unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

unsigned int loadTexture(const char* path);
void renderScene(const readShader& shader,glm::vec3 translation);
void renderCube();
void renderQuad();
unsigned int planeVAO;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{  // glfw: initialize and configure
    // -----------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // // glew: load all OpenGL function pointers
    glewInit();
    //shaders
    //texture mapping 2D
    //readShader texture2Dshader("./texture2D.vs", "./texture.fs");
    
    //texture mapping 3D
    //readShader texture3Dshader("./texture3D.vs", "./texture.fs");

    ////cube mapping
    readShader skyboxshader("./skybox.vs", "./skybox.fs");

    ////environment mapping
    readShader texture3Dshader("./environmentMap.vs", "./environmentMap.fs");

    ////shadow mapping
    readShader simpleDepthShader("shadow_mapping_depth.vs", "shadow_mapping_depth.fs");
    readShader shadowShader("shadow_mapping.vs", "shadow_mapping.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    //texture mapping
    ////2D
    //float vertices[] = {
    //    // positions          // colors           // texture coords
    //     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
    //     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
    //    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
    //    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    //};
    //unsigned int indices[] = {
    //    0, 1, 3, // first triangle
    //    1, 2, 3  // second triangle
    //};

    //unsigned int texture2DVBO, texture2DVAO, texture2DEBO;
    //glGenVertexArrays(1, &texture2DVAO);
    //glGenBuffers(1, &texture2DVBO);
    //glGenBuffers(1, &texture2DEBO);

    //glBindVertexArray(texture2DVAO);
    //glBindBuffer(GL_ARRAY_BUFFER, texture2DVBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, texture2DEBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //// position attribute
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);
    //// color attribute
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);
    //// texture coord attribute
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    //glEnableVertexAttribArray(2);
    ////2D
    //----------------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------------
    //3D
    unsigned int vertexCount = 0;                                                                        //3D model
    int triangleCount = 0;

    std::vector <float> out_vertices;
    std::vector<unsigned int> out_indices;

    const char* filepath = "./head.obj";

    bool load = loadOBJ(filepath, out_vertices, out_indices, vertexCount, triangleCount);
    double xpos, ypos;
    //rotation
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
    float rotationAngel = 1.0f;                                                                         //3D model
    //3D

    unsigned int texture3DVBO, texture3DVAO, texture3DEBO;
    glGenVertexArrays(1, &texture3DVAO);
    glGenBuffers(1, &texture3DVBO);
    glGenBuffers(1, &texture3DEBO);
    
    glBindVertexArray(texture3DVAO);
    glBindBuffer(GL_ARRAY_BUFFER, texture3DVBO);
    glBufferData(GL_ARRAY_BUFFER, out_vertices.size() * sizeof(float), &out_vertices[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, texture3DEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, out_indices.size() * sizeof(unsigned int), &out_indices[0], GL_STATIC_DRAW);  

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // normal attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
    //3D
   //----------------------------------------------------------------------------------------
    // load and create a texture 
    // -------------------------
    unsigned int texture1, texture2;
    // texture 1
    // ---------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    //stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char* data = stbi_load("./Century-Tower.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    // texture 2
    // ---------
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = stbi_load("./ufl.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

   // ----------------------------------------------------------------------------------------
    ////2D
    //texture2Dshader.use();
    ////2D
    //glUniform1i(glGetUniformLocation(texture2Dshader.ID, "texture1"), 0);
    //glUniform1i(glGetUniformLocation(texture2Dshader.ID, "texture2"), 1);
    //----------------------------------------------------------------------------------------
    //3D
    texture3Dshader.use();
    glUniform1i(glGetUniformLocation(texture3Dshader.ID, "texture1"), 0);
    glUniform1i(glGetUniformLocation(texture3Dshader.ID, "texture2"), 1);
    //3D
    //texture mapping
    
//----------------------------------------------------------------------------------------
    //cube mapping
    vector<std::string> faces
    {
        "./skybox/right.jpg",
        "./skybox/left.jpg",
        "./skybox/top.jpg",
        "./skybox/bottom.jpg",
        "./skybox/front.jpg",
        "./skybox/back.jpg"
    };
  /* vector<std::string> faces
    {
        "./uf_class/right.jpg",
        "./uf_class/left.jpg",
        "./uf_class/top.jpg",
        "./uf_class/bottom.jpg",
        "./uf_class/front.jpg",
        "./uf_class/back.jpg"
    };*/
    unsigned int cubemapTexture = loadCubemap(faces);

    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
    float Yaw = 0.0f;
    float Pitch = 0.0f;
    glm::mat4 Model(1.0f);
    glm::mat4 scaling = glm::scale(glm::mat4(1.0f), glm::vec3(0.3f, 0.3f, 0.3f));

    //glm::vec3 cameraPosition(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraPosition(0.0f, 0.3f, -0.5f);
    glm::vec3 cameraTarget(0.0f, 0.0f, -1.0f);
    glm::vec3 upvector(0.0f, 1.0f, 0.0f);
    glm::mat4 View = glm::lookAt(
        cameraPosition,  // cameraPosition:  the position of your camera, in world space
        cameraTarget,    // cameraTarget: where you want to look at, in world space
        upvector         // (upvector) head is up (set to 0,-1,0 to look upside-down)
    );
    glm::vec3 front;
    // Generates a really hard-to-read matrix, but a normal, standard 4x4 matrix nonetheless
    glm::mat4 Projection = glm::perspective(
        glm::radians(90.0f), // The vertical Field of View, in radians: the amount of "zoom". Think "camera lens". Usually between 90&deg; (extra wide) and 30&deg; (quite zoomed in)
        (float)SCR_WIDTH / (float)SCR_HEIGHT,      // Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
        0.1f,              // Near clipping plane. Keep as big as possible, or you'll get precision issues.
        100.0f             // Far clipping plane. Keep as little as possible.
    );
    float lastX = SCR_WIDTH / 2.0f;
    float lastY = SCR_HEIGHT / 2.0f;
 
    unsigned int skyboxVBO, skyboxVAO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);

    glBindVertexArray(skyboxVAO);
  
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
   //cube mapping
    
    //Shadow mapping
     // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float planeVertices[] = {
        // positions            // normals         // texcoords
         25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

         25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
         25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
    };

    // plane VAO
    unsigned int planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindVertexArray(0);
    // load textures
   // -------------
    unsigned int woodTexture = loadTexture("./ufl.jpg");
    // configure depth map FBO
    // -----------------------
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    // create depth texture
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // shader configuration
      // --------------------
    shadowShader.use();
    shadowShader.setInt("diffuseTexture", 0);
    shadowShader.setInt("shadowMap", 1);

    glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);
    // Shadow mapping
   
    //depth test     
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // render loop
    while (!glfwWindowShouldClose(window))
    {
       
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //texture mapping
        // bind Texture
        ////2D 
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, texture1);
        //glActiveTexture(GL_TEXTURE1);
        //glBindTexture(GL_TEXTURE_2D, texture2);
        //// draw triangle
        //texture2Dshader.use();
        //glBindVertexArray(texture2DVAO);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        ////2D
        
        //3D
        texture3Dshader.use();
        //Model rotation
        glfwGetCursorPos(window, &xpos, &ypos);
        if (ypos - SCR_HEIGHT / 2 > 0)
        {
            rotation = glm::rotate(rotation, glm::radians(rotationAngel), glm::vec3(1.0, 0.0, 0.0));
        }
        else { rotation = glm::rotate(rotation, glm::radians(-rotationAngel), glm::vec3(1.0, 0.0, 0.0)); }
        //send rotation to vertex shader:
        texture3Dshader.setMat4("rotation", rotation);
        //Model rotation

        glBindVertexArray(texture3DVAO);

        ////texture mapping
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, texture1);
        //glActiveTexture(GL_TEXTURE1);
        //glBindTexture(GL_TEXTURE_2D, texture2);
        //glDrawElements(GL_TRIANGLES, out_indices.size(), GL_UNSIGNED_INT, 0);
        //glBindVertexArray(0);
        ////texture mapping
        
        //Environment mapping
        Model = rotation * scaling;

        texture3Dshader.setMat4("model", Model);
        texture3Dshader.setMat4("view", View);
        texture3Dshader.setMat4("projection", Projection);
        texture3Dshader.setVec3("cameraPos", cameraPosition);
        //draw triangle
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawElements(GL_TRIANGLES, out_indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        //Environment mapping

        //cube mapping
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        skyboxshader.use();

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            Pitch += 0.8f;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            Pitch -= 0.8f;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            Yaw -= 0.8f;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            Yaw += 0.8f;
        }
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        cameraTarget = glm::vec3(front.x, front.y, front.z);

        View = glm::lookAt(cameraPosition, cameraTarget, upvector);
        skyboxshader.setMat4("projection", Projection);
        skyboxshader.setMat4("view", View);

        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default
        //cube mapping
        
        //shadow  mapping
        // 1. render depth of scene to texture (from light's perspective)
        // --------------------------------------------------------------
        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        float near_plane = 1.0f, far_plane = 10.5f;
        lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;

        // render scene from light's point of view
   
        float a;
        float t = sin(glfwGetTime() * 2.0f);
        a = t * 0.5f;
        glm::vec3 translation(a, 0.0f, 0.0f);

        simpleDepthShader.use();
        simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, woodTexture);
        renderScene(simpleDepthShader, translation);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // reset viewport
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 2. render scene as normal using the generated depth/shadow map  
        // --------------------------------------------------------------
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shadowShader.use();
        shadowShader.setMat4("projection", Projection);
        shadowShader.setMat4("view", View);
        // set light uniforms
        shadowShader.setVec3("viewPos", cameraPosition);
        shadowShader.setVec3("lightPos", lightPos);
        shadowShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, woodTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        renderScene(shadowShader, translation);
        //shadow  mapping
        

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    /*glDeleteVertexArrays(1, &texture2DVAO);
    glDeleteBuffers(1, &texture2DVBO);
    glDeleteBuffers(1, &texture2DEBO);
    glDeleteProgram(texture2Dshader.ID);*/

    glDeleteVertexArrays(1, &texture3DVAO);
    glDeleteBuffers(1, &texture3DVBO);
    glDeleteBuffers(1, &texture3DEBO);
    glDeleteProgram(texture3Dshader.ID);

    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVBO);
    glDeleteProgram(skyboxshader.ID);

    glDeleteVertexArrays(1, &planeVAO);
    glDeleteBuffers(1, &planeVBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
// renders the 3D scene
// --------------------
void renderScene(const readShader& shader, glm::vec3 translation)
{
    // floor
    glm::mat4 model = glm::mat4(1.0f);
    shader.setMat4("model", model);
    glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
 
    // cubes
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.25f, 0.2f, 0.3f));
    model = glm::translate(model,translation);
    model = glm::scale(model, glm::vec3(0.1f));
    model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    shader.setMat4("model", model);
    renderCube();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.5f, 0.0f, 1.0f));
    model = glm::translate(model, translation);
    model = glm::scale(model, glm::vec3(0.2f));
    shader.setMat4("model", model);
    renderCube();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-0.3f, 0.0f, 0.2f));
    model = glm::translate(model, translation);
    model = glm::scale(model, glm::vec3(0.2));
    shader.setMat4("model", model);
    renderCube();
}


// renderCube() renders a 1x1 3D cube in NDC.
// -------------------------------------------------
unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void renderCube()
{
    // initialize (if necessary)
    if (cubeVAO == 0)
    {
        float vertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
            // bottom face
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
            // top face
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
             1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
             1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
        };
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(cubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    // render Cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

// renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}