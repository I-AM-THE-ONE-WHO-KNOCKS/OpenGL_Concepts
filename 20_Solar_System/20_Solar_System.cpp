#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Shader.h"
#include <stdio.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "Sphere.h"
#include <FreeImage.h>

Shader* shader;
GLint programID = -1;
Sphere* mSphere;

GLint positionLocation;
GLint colorLocation;
GLint modelMatrixLocation;
GLint projectionMatrixLocation;
GLint viewMatrixLocation;
GLint textureLocation;
GLint activeTextureLocation;

float zPos = 0.0;

int sunTextureId;
int earthTextureId;
int moonTextureId;

int width = 0;
int height = 0;

BYTE* initializeImage(const char* filepath)
{
    BYTE* image;

    //image format
    FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

    //pointer to the image, once loaded
    FIBITMAP *dib = 0;

    //check the file signature and deduce its format
    fif = FreeImage_GetFileType(filepath, 0);

    //if still unknown, try to guess the file format from the file extension
    if (fif == FIF_UNKNOWN)
        fif = FreeImage_GetFIFFromFilename(filepath);

    dib = FreeImage_Load(fif, filepath, 0);
    if (dib == NULL)
    {
        printf("Unable to load image");
        return NULL;
    }

    image = FreeImage_GetBits(dib);
    width = FreeImage_GetWidth(dib);
    height = FreeImage_GetHeight(dib);

    return image;
}

GLuint initializeTexture(const char* filepath)
{
    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    BYTE* data = initializeImage(filepath);
    if(data != NULL)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

        // define filtering i.e minification and magnification
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);

        return id;
    }

    return UINT_MAX;
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);

    shader = new Shader();
    programID = shader->init("shader_solar.vert",
                             "shader_solar.frag");
    printf("program id = %d", programID);

    //load the program object to the GPU
    glUseProgram(programID);

    positionLocation =
            glGetAttribLocation(programID, "a_Position");
    if(positionLocation < 0) {
        printf("Invalid location for a_Position");
    }

    colorLocation =
            glGetAttribLocation(programID, "a_Color");
    if(colorLocation < 0) {
        printf("Invalid location for a_Color");
    }

    textureLocation =
            glGetAttribLocation(programID, "a_Texcoord");
    if(textureLocation < 0) {
        printf("Invalid location for a_Texcoord");
    }

    activeTextureLocation =
            glGetUniformLocation(programID, "activeTexture");
    if(activeTextureLocation < 0) {
        printf("Invalid location for activeTexture");
    }

    modelMatrixLocation =
            glGetUniformLocation(programID, "u_ModelMatrix");

    projectionMatrixLocation =
            glGetUniformLocation(programID, "u_ProjectionMatrix");

    viewMatrixLocation =
            glGetUniformLocation(programID, "u_ViewMatrix");

    glm::mat4 projMatrix = glm::mat4(1.0);
    projMatrix = glm::perspective(45.0, 1.0, 0.1, 100.0);

    glUniformMatrix4fv(projectionMatrixLocation,
                       1,
                       false,
                       &projMatrix[0][0]);

    mSphere = new Sphere();
    mSphere->initSphere(50, 50, 1.0, 1.0);

    sunTextureId = initializeTexture("sun.jpg");
    earthTextureId = initializeTexture("earth.jpg");
    moonTextureId = initializeTexture("moon.png");
}

float angle = 0.0;

void drawScene() {
    angle += 1.0;
    if( angle >= 360.0) angle = 0.0;

    zPos += 1.0;
    if(zPos >= 150.0) zPos = 0.0;

    glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0.0, 0.0, 1.0),  //pos
                                       glm::vec3(0.0, 0.0, 0.0),  //looking at
                                       glm::vec3(0.0, 1.0, 0.0)); //up vector
    glUniformMatrix4fv(viewMatrixLocation, 1, false, &viewMatrix[0][0]);



    glm::mat4 modelMatrix;

    modelMatrix =
            glm::translate(glm::vec3(0, 0.0, -7.0));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(angle),
                              glm::vec3(0.0, 1.0, 0.0));

    glUniformMatrix4fv(modelMatrixLocation,
                       1,
                       false,
                       &modelMatrix[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sunTextureId);
    glUniform1i(activeTextureLocation, 0);

    //draw the sun
    mSphere->drawSphere(positionLocation, -1 , colorLocation, textureLocation);

    modelMatrix = glm::translate(modelMatrix, glm::vec3(4.0, 0.0, 0.0));
    modelMatrix = glm::rotate(modelMatrix,
                              glm::radians(angle+30), glm::vec3(0.0, 1.0, 0.0));

    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5, 0.5, 0.5));
    glUniformMatrix4fv(modelMatrixLocation,
                       1,
                       false,
                       &modelMatrix[0][0]);

    glBindTexture(GL_TEXTURE_2D, earthTextureId);
    //draw the earth
    mSphere->drawSphere(positionLocation, -1 , colorLocation, textureLocation);

    modelMatrix = glm::translate(modelMatrix, glm::vec3(3.0, 0.0, 0.0));
    modelMatrix = glm::rotate(modelMatrix,
                              glm::radians(angle+100), glm::vec3(0.0, 1.0, 0.0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5, 0.5, 0.5));
    //draw the moon
    glUniformMatrix4fv(modelMatrixLocation,
                       1,
                       false,
                       &modelMatrix[0][0]);

    glBindTexture(GL_TEXTURE_2D, moonTextureId);

    mSphere->drawSphere(positionLocation, -1 , colorLocation, textureLocation);
}

void draw() {
    //printf("DRAW\n");
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawScene();

    glutSwapBuffers();
}

void timer(int val) {
    draw();
    glutTimerFunc(50, timer, 0);
}

void resize(int w, int h) {
    printf("resize");

    glViewport(0, 0, w, h);
    glm::mat4 projMatrix = glm::mat4(1.0);
    projMatrix =
          glm::perspective(45.0, 1.0, 0.1, 100.0);
    glUniformMatrix4fv(projectionMatrixLocation, 1,
                       false, &projMatrix[0][0]);
}

int main(int argc, char** argv)
{
    //initialize the glut library
    glutInit(&argc, argv);

    //set the appropriate display mode
    //this configures the frame buffer
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);

    //define the size of the window
    glutInitWindowSize(512, 512);

    glutCreateWindow("Solar System");

//     printf("%s\n", glGetString(GL_VERSION));
//     printf("%s\n", glGetString(GL_VENDOR));
//     printf("%s\n", glGetString(GL_EXTENSIONS));

     //initialize the glew library
    glewInit();

    //start making opengl calls
    init();

    glutDisplayFunc(draw);
    glutReshapeFunc(resize);

    glutTimerFunc(50, timer, 0);
    //start the glut event loop
    glutMainLoop();


    return 0;
}
