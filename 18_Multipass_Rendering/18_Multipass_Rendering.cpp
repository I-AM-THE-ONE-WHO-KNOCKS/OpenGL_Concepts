#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Shader.h"
#include <stdio.h>
#include <string.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <FreeImage.h>
#include <iostream>
#include "Cube.h"

using namespace std;

float quad[] = {1.0, 1.0,  1.0, 0.0, 0.0, 1.0, 1.0, 1.0,
               -1.0, 1.0,  0.0, 1.0, 0.0, 1.0, 0.0, 1.0,
                1.0, -1.0,  0.0, 0.0, 1.0 ,1.0, 1.0, 0.0,
               -1.0, -1.0, 0.0, 0.0, 1.0 ,1.0, 0.0, 0.0 };

Shader* shader;
GLint programID = -1;
GLint programID1 = -1;

GLint positionLocation;
GLint colorLocation;
GLint textureLocation;
GLint modelMatrixLocation;
GLint projectionMatrixLocation;
GLint viewMatrixLocation;
GLint activeTextureLocation;
GLint enableTextureLocation;

int width;
int height;
GLuint textureID;

int onScreenFBO;
int offScreenFBO;
int offScreenTextureID;

int pixelReadObject;

GLint initializePBO(int w, int h, int pixelsize)
{
    GLuint id;
    glGenBuffers(1, &id);

    glBindBuffer(GL_PIXEL_PACK_BUFFER, id); // this will be packing operation
    glBufferData(GL_PIXEL_PACK_BUFFER, w * h * pixelsize, NULL, GL_STREAM_COPY);


   glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
    return id;
}

void initializeOffScreenFBO()
{
    glBindFramebuffer(GL_FRAMEBUFFER, offScreenFBO);
    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, onScreenFBO);
}

GLuint createOffScreenTexture(int w, int h)
{
    GLuint id;

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    return id;
}

GLuint createOffScreenDepthAttachment(int w, int h)
{
    GLuint id;

    glGenRenderbuffers(1, &id);
    glBindRenderbuffer(GL_RENDERBUFFER, id);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, w, h);
    glGenRenderbuffers(1, 0);

    return id;
}

int initializeFBO(int width, int height)
{
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &onScreenFBO);

    GLuint id;

    glGenFramebuffers(1, &id);

    //bind to the frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, id);

    offScreenTextureID = createOffScreenTexture(width, height);
    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D,
                           offScreenTextureID,
                           0);

    GLuint depthID = createOffScreenDepthAttachment(width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                              GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER,
                              depthID);

    int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(status == GL_FRAMEBUFFER_COMPLETE)
    {
        cout << endl;
        cout << "Successfully created frame buffer object" << endl;
    }
    else {
        cout << endl;        cout << "Failed to create frame buffer object" << endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, onScreenFBO);

    return id;
}

void drawQuad() {
	glEnableVertexAttribArray(positionLocation);
	glEnableVertexAttribArray(colorLocation);
	glEnableVertexAttribArray(textureLocation);

	glVertexAttribPointer(positionLocation, 2, GL_FLOAT, false, 32, quad);
	glVertexAttribPointer(colorLocation, 4, GL_FLOAT, false, 32, quad+2);
	glVertexAttribPointer(textureLocation, 2, GL_FLOAT, false, 32, quad+6);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableVertexAttribArray(positionLocation);
	glDisableVertexAttribArray(colorLocation);
	glDisableVertexAttribArray(textureLocation);

}

BYTE* initializeImage(const char* filepath) {
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
    if (dib == NULL) {
        printf("Unable to load image");
        return NULL;
    }
    image = FreeImage_GetBits(dib);
    width = FreeImage_GetWidth(dib);
    height = FreeImage_GetHeight(dib);

    return image;
}


GLuint initializeTexture(const char* textureFile) {
    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    BYTE* data = initializeImage(textureFile);
    if( data != NULL) {
        glTexImage2D(GL_TEXTURE_2D, 0,
                     GL_RGBA, width, height, 0,
                     GL_BGR, GL_UNSIGNED_BYTE, data);

        if( GL_NO_ERROR == glGetError()) {
            printf("Going fine\n");
        }
        //define filtering i.e minification and magnification
        glTexParameteri(GL_TEXTURE_2D,
                        GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,
                        GL_TEXTURE_MAG_FILTER,
                        GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);

        return id;
    }

    return UINT_MAX;
}

float zPos = 0.0;

void initializeLocations(int programID)
{
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
            glGetAttribLocation(programID, "a_TextureCoordinates");
    modelMatrixLocation =
            glGetUniformLocation(programID, "u_ModelMatrix");

    projectionMatrixLocation =
            glGetUniformLocation(programID, "u_ProjectionMatrix");
    viewMatrixLocation =
            glGetUniformLocation(programID, "u_ViewMatrix");

    activeTextureLocation =
            glGetUniformLocation(programID, "activeTexture");

    enableTextureLocation =
        glGetUniformLocation(programID, "enableTexture");
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);

    shader = new Shader();
    programID = shader->init("shader_multi.vert",
                             "shader_multi.frag");
    printf("program id = %d\n", programID);

    programID1 = shader->init("shader_multi1.vert",
                             "shader_multi1.frag");
    printf("program id = %d\n", programID1);

    //load the program object to the GPU
    glUseProgram(programID);

    initializeLocations(programID);


    textureID = initializeTexture("earth.jpg");

    offScreenFBO = initializeFBO(512, 512);
    initializeOffScreenFBO();


    pixelReadObject = initializePBO(1024, 1024, 4);
}

void drawScene() {
    // FIRST PASS OF THE GRAPHICS PIPELINE
    glBindFramebuffer(GL_FRAMEBUFFER, offScreenFBO);

    glm::mat4 projMatrix = glm::mat4(1.0);
    projMatrix =
          glm::ortho(-1.0, 1.0, -1.0, 1.0, -1.0, 10.0);
    glUniformMatrix4fv(projectionMatrixLocation, 1,
                       false, &projMatrix[0][0]);

     //glViewport(0, 0, 512, 512);

    glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0.0, 0.0, 1), //pos
                                       glm::vec3(0.0, 0.0, 0.0),  //looking at
                                       glm::vec3(0.0, 1.0, 0.0)); //up vector
    glUniformMatrix4fv(viewMatrixLocation, 1, false, &viewMatrix[0][0]);


    //set texture unit with ID 0 as the active texture unit
    glActiveTexture(GL_TEXTURE0);
    //bind the texture object to the texute unit
    glBindTexture(GL_TEXTURE_2D, textureID);

    //tell the shader what is the id of the active texture unit
    glUniform1i(activeTextureLocation, 0);



    glm::mat4 modelMatrix;

    modelMatrix = glm::translate(glm::vec3(0.0, 0.0, -2.0));

    glUniformMatrix4fv(modelMatrixLocation,
                       1,
                       false,
                       &modelMatrix[0][0]);

    glUniform1i(enableTextureLocation, 1);

    drawQuad();

    glFlush();

    // pass two

    glBindFramebuffer(GL_FRAMEBUFFER, onScreenFBO);

    glUseProgram(programID1);
    initializeLocations(programID1);
    glUniformMatrix4fv(viewMatrixLocation, 1, false, &viewMatrix[0][0]);

    glUniformMatrix4fv(modelMatrixLocation,
                       1,
                       false,
                       &modelMatrix[0][0]);
    glUniform1i(enableTextureLocation, 1);
    glUniformMatrix4fv(projectionMatrixLocation, 1,
                       false, &projMatrix[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, offScreenTextureID);
    glUniform1i(activeTextureLocation, 0);

     glViewport(0, 0, 512, 512);

    drawQuad();

    glFlush();


}
void draw() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
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
          glm::ortho(-1.0, 1.0, -1.0, 1.0, -1.0, 10.0);
    glUniformMatrix4fv(projectionMatrixLocation, 1,
                       false, &projMatrix[0][0]);
}

void keyStroke(unsigned char c, int x, int y)
{
    if(c == 's')
    {
        //draw cube in offscreen FBO
        glBindFramebuffer(GL_FRAMEBUFFER, offScreenFBO);

        glUniform1i(enableTextureLocation, 0);
        glViewport(0, 0, 1024, 1024);
        glClearColor(0.5, 0.5, 0.5, 0.5);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 modelMatrix;

        modelMatrix = glm::translate(glm::vec3(-0.5, 0.0, -2.0));

        glUniformMatrix4fv(modelMatrixLocation,
                           1,
                           false,
                           &modelMatrix[0][0]);
        glFinish();

        glBindFramebuffer(GL_FRAMEBUFFER, onScreenFBO);
    }

    if(c == 'f'){
         glBindFramebuffer(GL_FRAMEBUFFER, offScreenFBO);
//         unsigned char pixels[4];
//         unsigned char pixelfull[1024*1024*4];
//         glReadPixels(0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
//         //you can directly attach this pixelfull to tex2d and use
//         //but this will be gpu to cpu and cpu to gpu copy which is not good
//         //Gpu gpu copy is always good choice which can be done using pixel buffer object shown below
//         glReadPixels(0, 0, 1024, 1024, GL_RGBA, GL_UNSIGNED_BYTE, pixelfull);
//          printf("color values %u, %u, %u, %u\n", pixels[0], pixels[1], pixels[2], pixels[3]);


         glBindBuffer(GL_PIXEL_PACK_BUFFER, pixelReadObject);
         glReadPixels(0, 0, 1024, 104, GL_RGBA, GL_UNSIGNED_BYTE, 0);
//         //copy data from texture mapped to FBO into PBO
//         glBindBuffer(GL_PIXEL_PACK_BUFFER, pixelReadObject);
//         glBindTexture(GL_TEXTURE_2D, offScreenTextureID);
//         //copy data from the texture object to the pixel buffer object
//         glBufferSubData(GL_PIXEL_PACK_BUFFER, 0, 1024 * 1024 * 4, 0);

         // map PBO to client space
         void* pbo = glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_WRITE);
         unsigned char color[4];
         memcpy(color, pbo, 4);
         printf("%u, %u, %u, %u\n", color[0], color[1], color[2], color[3]);
         glUnmapBuffer(GL_PIXEL_PACK_BUFFER);



         glBindFramebuffer(GL_FRAMEBUFFER, onScreenFBO);
    }
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

    glutCreateWindow("Multipass rendering");

//     printf("%s\n", glGetString(GL_VERSION));
//     printf("%s\n", glGetString(GL_VENDOR));
//     printf("%s\n", glGetString(GL_EXTENSIONS));

     //initialize the glew library
    glewInit();

    //start making opengl calls
    init();

    glutDisplayFunc(draw);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyStroke);

    glutTimerFunc(50, timer, 0);
    //start the glut event loop
    glutMainLoop();


    return 0;
}
