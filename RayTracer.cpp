#include <fstream>
#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdio.h>
#include <cmath>
#include<malloc.h>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
// settings
const unsigned int W = 1024;
const unsigned int H = 768;

//Ambient coefficient
const double ka = 0.4;
//Diffuse coefficient
const double kd = 0.5;
//specular coefficient
const double ks = 5;
//light intensity
const double I = 3.0;
//Ambient light intensity
const double Ia = 0.8;
//glaze Coefficiency
const int N = 10;
//Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"layout (location = 2) in vec2 aTexCoord;\n"
"out vec3 ourColor;\n"
"out vec2 TexCoord;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(aPos, 1.0);\n"
"ourColor = aColor;\n"
"TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
"}\0";
//Fragment shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"in vec2 TexCoord;\n"
"uniform sampler2D texture1;\n"
"void main()\n"
"{\n"
"   FragColor = texture(texture1, TexCoord);\n"
"}\n\0";

double diffuseCoefficient = 0;
double specularCoefficient = 0;

class Color {
public:
    int r, g, b;
    Color() { r = g = b = 0; }//Defalut color is black
    Color(int red, int green, int blue) { r = red, g = green, b = blue; }
};

class Dot {
public:
    double x, y, z;
    Dot() { x = y = z = 0; }
    Dot(double a, double b, double c) { x = a, y = b, z = c; }
    Dot operator +(const Dot& v) { return Dot(x + v.x, y + v.y, z + v.z); } //Vector add
    Dot operator -(const Dot& v) { return Dot(x - v.x, y - v.y, z - v.z); } //Vector minus
    Dot operator * (double d) const { return Dot(x * d, y * d, z * d); }    //Dot or vector multiply a constant
    Dot operator / (double d) const { return Dot(x / d, y / d, z / d); }    //Dot or vector divide by a constant
    Dot normalize() {
        double model = sqrt(x * x + y * y + z * z);
        return Dot(x / model, y / model, z / model);
    }
    Dot reverse() { return Dot(x = -x, y = -y, z = -z); }
};

typedef Dot Vector;

double dotProduct(Dot v, Dot a) { return (v.x * a.x + v.y * a.y + v.z * a.z); }

class Ray {
public:
    Dot p;   //origin
    Dot d;   //direction
    Ray(Dot p1, Dot d1) { p = p1, d = d1; }
};

class Sphere {
public:
    Color color;
    Dot center;       //center
    double radius;       //radius                        
    Sphere(Dot c1, double r) { center = c1, radius = r; }
    Dot getNormal(Dot pi) { return Dot(center - pi) / radius; }
    bool intersect(Ray r, double& t) {
        Dot p = r.p;
        Dot d = r.d;
        Vector cp = p - center;
        double b = 2 * dotProduct(cp, d);
        double c = dotProduct(cp, cp) - radius * radius;
        double discriminant = b * b - 4 * c;//🔺=B*B-4AC, A=a*a=1(a is unit vector)

       // cout << "discriminant=" << discriminant << endl;

        if (discriminant < 0) {

            return false;
        }
        else {
            discriminant = sqrt(discriminant);
            double t0 = -b - discriminant;
            double t1 = -b + discriminant;

            t = (t0 < t1) ? t0 : t1;
            return true;
        }
    }
};

class Triangle {
public:
    Color color;
    Dot a, b, c;
    Triangle(Dot i, Dot j, Dot k) { a = i, b = j, c = k; }
    Vector getTriangleNormal(Vector AB, Vector AC) {
        double x0 = AB.x; double y0 = AB.y; double z0 = AB.z;
        double x1 = AC.x; double y1 = AC.y; double z1 = AC.z;
        return Vector(y0 * z1 - y1 * z0, x1 * z0 - x0 * z1, x0 * y1 - x1 * y0).normalize();
    }
    bool intersect(Ray ray, Triangle t1, double& t) {
        bool notParallel, within;
        double x, y, z, ta;
        ta = 0;
        notParallel = within = false;
        Vector normal = t1.getTriangleNormal((t1.b - t1.a), (t1.c - t1.a));
        ta = dotProduct(ray.d, normal);

        //cout <<"ta="<< ta << endl;

        if (ta != 0) {
            notParallel = true;
            t = dotProduct((t1.a - ray.p), normal) / ta;
        }
        //cout << "notParallel=" << notParallel << endl;
        if (notParallel) {
            Dot intersectionPoint = ray.p + ray.d * t;
            x = dotProduct(normal, t1.getTriangleNormal((t1.b - t1.a), (intersectionPoint - t1.a)));
            y = dotProduct(normal, t1.getTriangleNormal((t1.c - t1.b), (intersectionPoint - t1.b)));
            z = dotProduct(normal, t1.getTriangleNormal((t1.a - t1.c), (intersectionPoint - t1.c)));
            if (x > 0 && y > 0 && z > 0) {
                within = true;
            }
        }
        //cout << "within=" << within << endl;
        return within;

    }
};
class Tetrahedron {
public:
    Dot a, b, c, d;
    Triangle tri[4] = { Triangle(a,b,c),Triangle(a,c,d),Triangle(a,d,b),Triangle(b,d,c) };
    Tetrahedron(Dot i, Dot j, Dot k, Dot l) {
        a = i, b = j, c = k, d = l;
        tri[0] = Triangle(i, j, k),
            tri[1] = Triangle(i, k, l),
            tri[2] = Triangle(i, l, j),
            tri[3] = Triangle(j, l, k);
    }

};


void  BubbleSort1(double* T) {
    int i, j;
    double temp;
    for (j = 0; j < 3 - j; j++)
    {
        for (i = 0; i < 3; i++)
            if (T[i] > T[i + 1])
            {
                temp = T[i];
                T[i] = T[i + 1];
                T[i + 1] = temp;
            }
    }
}
void  BubbleSort2(double* T) {
    int i, j;
    double temp;
    for (j = 0; j < 1 - j; j++)
    {
        for (i = 0; i < 1; i++)
            if (T[i] > T[i + 1])
            {
                temp = T[i];
                T[i] = T[i + 1];
                T[i + 1] = temp;
            }
    }
}
//lookat direction
Vector lookat(0,0,-1);

//light direction
Vector lightDirection(0, 1, -1);
Vector lookatReverse = lookat.reverse().normalize();

double getDiffuseCoefficient(Vector normal) {
    double diffuseCoefficient = I * kd * max(double(0), dotProduct(lightDirection, normal));
    return diffuseCoefficient;
}
double getSpecularCoefficient(Vector normal) {
    lightDirection.normalize();
    Vector lightReverse = lightDirection.reverse().normalize();
    Vector Vh = (lightReverse + lookatReverse).normalize();
    double specularCoefficient = I * ks * pow(max(double(0), dotProduct(Vh, normal)),N);
    return specularCoefficient;
}


int main()
{

    Color white(100, 100, 100);
    Color red(100, 0, 0);
    Color green(0, 220, 0);
    Color yellow(100, 100, 0);
    Color blue(0, 0, 100);

    Tetrahedron tetrahedron1(Dot(100, 100, 0), Dot(200, 200, -100), Dot(100, 400, -100), Dot(450, 300, -50));//

    Sphere sphere0(Dot(200, 650, 100), -100);
    Sphere sphere1(Dot(500, 800, 400), -200);
    ;

    sphere0.color = red;
    sphere1.color = green;

    tetrahedron1.tri[0].color = green;
    tetrahedron1.tri[1].color = blue;
    tetrahedron1.tri[2].color = yellow;
    tetrahedron1.tri[3].color = red;

    Sphere s[2] = { sphere0,sphere1 };



    // glfw: initialize and configure GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(W, H, "RayTracer", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // // GLEW: load all OpenGL function pointers
    glewInit();

    // build and compile the shaders
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    // load and create a texture 
    // -------------------------
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    //pixel order
    // Create the image (RGB Array) to be displayed
    unsigned char* image = (unsigned char*) malloc(sizeof(unsigned char[H * W * 8]));

    double T1[4] = { 0 };
    double T2[4] = { 0 };
    double Ts[3] = { 0 };
    double Ts1[3] = { 0 };
    int triangleNumber, sphereNumber;
    triangleNumber = sphereNumber = -1;

   //cout << "if you want to switch to perspective mode, press 'P'!" << endl;
   // char key;
   // cin >> key;
    //Ray Tracer, pixel order
    for (int i = 0; i < H; i++)
    {
        for (int j = 0; j < W; j++)
        {
            int idx = (i * W + j) * 3;

            //normalize light direction
            lightDirection.normalize();

            Dot lightReverse = lightDirection.reverse().normalize();
            //send a ray through each pixel
      
            Ray ray(Dot(i, j, 0), lookat.normalize()); //parallel camera
               
            //if (key == 'p' || key == 'P') {ray = Ray(Dot(H/2, W/2, -200), Dot(i-H/2, j-W/2, 200).normalize());}// Perspective camera

            //Ray-Triangle Intersection:
            for (int a = 0; a < 4; a++) {
                T2[a] = 9999;
                if (tetrahedron1.tri[a].intersect(ray, tetrahedron1.tri[a], T1[a])) {
                    if (T1[a] != 0) {
                        T2[a] = T1[a];
                    }
                }
            }

            BubbleSort1(T2);
            for (int i = 0; i < 4; i++) {
                if (T2[0] != 9999) {
                    if (T2[0] == T1[i]) {
                        triangleNumber = i;            //first triangle to be intersected 
                    }
                }
                //  cout << "triangleNumber:" << triangleNumber <<"triangleT:"<<T2[0] << endl;
            }

            //Ray-Sphere Intersection:
            for (int i = 0; i < 2; i++) {
                Ts[i] = 9999;
                if (s[i].intersect(ray, Ts1[i])) {
                    if (Ts1[i] != 0) {
                        Ts[i] = Ts1[i];
                    }
                }
                // cout << T1[i] << endl;
            }

            //Discriminate the first one to be intersected
            BubbleSort2(Ts);
            for (int i = 0; i < 2; i++) {
                if (Ts[0] != 9999) {
                    if (Ts[0] == Ts1[i]) {
                        sphereNumber = i;            //first triangle to be intersected 
                    }
                }
                //cout << "triangleNumber:" << triangleNumber << "triangleT:" << T2[0] << endl;
            }
            if (Ts[0] == 9999) {        //No sphere is intersected
                if (T2[0] != 9999) {      //if any triangle is intersected
                //render tetrahedron
                    //Blinn-Phong Model
                    Triangle it = tetrahedron1.tri[triangleNumber];
                    Vector normal = it.getTriangleNormal((it.b - it.a), (it.c - it.a));

                    normal.normalize();
                    diffuseCoefficient = getDiffuseCoefficient(normal);

                    double tz = T1[triangleNumber];
                    Dot intersectionPoint = Dot(i + lookat.x * tz, j + lookat.y * tz, lookat.z * tz);
               
                    specularCoefficient = getSpecularCoefficient(normal);  

                    //white light
                    int r = (int)((tetrahedron1.tri[triangleNumber].color.r) * (ka *Ia + diffuseCoefficient + specularCoefficient));
                    int g = (int)((tetrahedron1.tri[triangleNumber].color.g) * (ka *Ia + diffuseCoefficient + specularCoefficient));
                    int b = (int)((tetrahedron1.tri[triangleNumber].color.b) * (ka *Ia + diffuseCoefficient + specularCoefficient));



                    image[idx] = (unsigned char)r; //R
                    image[idx + 1] = (unsigned char)g;        //G
                    image[idx + 2] = (unsigned char)b;       //B                                            
                }
                else if (T2[0] == 9999) {                 //Nothing is intersected
                    //Blinn-Phong Model
                    Vector normal = Vector(1, 0, 0);
                    diffuseCoefficient = getDiffuseCoefficient(normal);

                    double tz = 0;
                    if (lookat.x != 0) {
                        tz = -i / lookat.x;
                        Dot intersectionPoint = Dot(i + lookat.x * tz, j + lookat.y * tz, lookat.z * tz);         
                    }
                  
                    specularCoefficient = getSpecularCoefficient(normal);
                    //white light
                    int r = 5*(int)(ka * Ia + diffuseCoefficient + specularCoefficient);
                    int g = 5*(int)(ka * Ia + diffuseCoefficient + specularCoefficient);
                    int b = 5*(int)(ka * Ia + diffuseCoefficient + specularCoefficient);



                    image[idx] = (unsigned char)r;   //Background color is white
                    image[idx + 1] = (unsigned char)g;
                    image[idx + 2] = (unsigned char)b;
                }
            }
            else if (Ts[0] != 9999) {                              //Sphere is intersected
                if (T2[0] != 9999 && Ts[0] < T2[0] || T2[0] == 9999) {         //Sphere is first intersected
                    //Blinn-Phong Model
                    Vector normal = lookatReverse * Ts1[sphereNumber] - s[sphereNumber].center;
                    /*
                    if (key == 'p' || key == 'P') {
                        normal = (Dot(H / 2, W / 2, -200) - Dot(i, j, 0)) * Ts1[sphereNumber] - s[sphereNumber].center;
                    }
                    */
                    normal.normalize();
                    
                    double tz = T1[triangleNumber];
                    Dot intersectionPoint = Dot(i + lookat.x * tz, j + lookat.y * tz, lookat.z * tz);

                    diffuseCoefficient = getDiffuseCoefficient(normal);

                    specularCoefficient = getSpecularCoefficient(normal);
                 

                    }
                    //render sphere
                    int r = (int)((s[sphereNumber].color.r) * (ka *Ia + diffuseCoefficient + specularCoefficient));
                    int g = (int)((s[sphereNumber].color.g) * (ka *Ia + diffuseCoefficient + specularCoefficient));
                    int b = (int)((s[sphereNumber].color.b) * (ka *Ia + diffuseCoefficient + specularCoefficient));

                    image[idx] = (unsigned char)r;
                    image[idx + 1] = (unsigned char)g;
                    image[idx + 2] = (unsigned char)b;
                }
                else if (T2[0] != 9999 && T2[0] < Ts[0]) 
                {        //Tetrahedron is first intersected
                   //render tetrahedron
                   //Blinn-Phong Model
                    Triangle it = tetrahedron1.tri[triangleNumber];
                    Vector normal = it.getTriangleNormal((it.b - it.a), (it.c - it.a));
                    normal.normalize();
                    diffuseCoefficient = getDiffuseCoefficient(normal);

                    specularCoefficient = getSpecularCoefficient(normal);

                    double tz = T1[sphereNumber];
                    Dot intersectionPoint = Dot(i + lookat.x * tz, j + lookat.y * tz, lookat.z * tz);     
              
                    //white light
                    int r = (int)((tetrahedron1.tri[triangleNumber].color.r) * (ka * Ia + diffuseCoefficient + specularCoefficient));
                    int g = (int)((tetrahedron1.tri[triangleNumber].color.g) * (ka * Ia + diffuseCoefficient + specularCoefficient));
                    int b = (int)((tetrahedron1.tri[triangleNumber].color.b) * (ka * Ia + diffuseCoefficient + specularCoefficient));

                    image[idx] = (unsigned char)r; //R
                    image[idx + 1] = (unsigned char)g;        //G
                    image[idx + 2] = (unsigned char)b;       //B   
                }
        }
    }
 

    unsigned char* data = &image[0];
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, W, H, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        cout << "Failed to load texture" << endl;
    }



    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // bind Texture
        glBindTexture(GL_TEXTURE_2D, texture);

        // render container
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    free(image);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    //free(image);
    system("pause");
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
