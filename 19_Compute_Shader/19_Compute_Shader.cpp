#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <FreeImage.h>
#include <cstring>
#include "Shader.h"

float quad[] = {1.0, 1.0,  1.0, 0.0, 0.0,  1.0, 1.0, 1.0,
               -1.0, 1.0,  0.0, 1.0, 0.0,  1.0, 0.0, 1.0,
                1.0,-1.0,  0.0, 0.0, 1.0,  1.0, 1.0, 0.0,
               -1.0,-1.0,  0.0, 0.0, 1.0,  1.0, 0.0, 0.0 };
int width;
int height;
float angle = 0.0f;
float rad = 0.0f;
float zPos = 0.0;

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
GLuint textureID;
GLuint enableTextureLocation;
GLuint pixelReadObject;
GLint computeID;
int onScreenFBO;
int offScreenFBO;
int offScreenTextureId;
int offScreenDepthId;
int outputImageID;

GLuint initializeOutputImage(int w, int h)
{
    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    return id;
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
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);

        return id;
    }
    return UINT_MAX;
}

GLuint initializePBO(int w, int h, int pixelsize)
{
    GLuint id;
    glGenBuffers(1, &id);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, id);
    glBufferData(GL_PIXEL_PACK_BUFFER, w * h * pixelsize, NULL, GL_STREAM_COPY);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
    return id;
}

void initializeOffscreenFBO()
{
    glBindFramebuffer(GL_FRAMEBUFFER, offScreenFBO);
    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, onScreenFBO);
    glClearColor(1.0, 1.0, 1.0, 1.0);
}

GLuint createOffScreenTexture(int width, int height)
{
    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    return id;
}

GLuint createOffScreenDepthAttachemnt(int width, int height)
{
    GLuint id;
    glGenRenderbuffers(1, &id);
    glBindRenderbuffer(GL_RENDERBUFFER, id);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    return id;
}

int initializeFBO(int width, int height)
{
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &onScreenFBO);
    GLuint id;
    glGenFramebuffers(1, &id);
    glBindFramebuffer(GL_FRAMEBUFFER, id);
    /*
     * CREATE COLOR ATTACHMENT FOR FBO
     * */
    offScreenTextureId = createOffScreenTexture(width, height);
    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D,
                           offScreenTextureId,
                           0);
    /*
     * CREATE DEPTH ATTACHMENT FOR FBO
     * */
    offScreenDepthId = createOffScreenDepthAttachemnt(width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                              GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER,
                              offScreenDepthId);

    int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(status == GL_FRAMEBUFFER_COMPLETE)
    {
        printf("Successfully created frame buffer object \n");
    }
    else
    {
        printf("failed to create FBO\n");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, onScreenFBO);//bind to onscreen frame buffer explicitly.
    return id;
}

void initializeLocations(GLuint programID)
{
    textureID  = initializeTexture("earth.jpg");

    positionLocation         = glGetAttribLocation(programID, "a_Position");
    colorLocation            = glGetAttribLocation(programID, "a_Color");
    textureLocation          = glGetAttribLocation(programID, "a_TextureCoordinates");

    modelMatrixLocation      = glGetUniformLocation(programID, "u_ModelMatrix");
    projectionMatrixLocation = glGetUniformLocation(programID, "u_ProjectionMatrix");
    viewMatrixLocation       = glGetUniformLocation(programID, "u_ViewMatrix");
    activeTextureLocation    = glGetUniformLocation(programID, "u_Texture");
    enableTextureLocation    = glGetUniformLocation(programID, "enableTexture");
    pixelReadObject = initializePBO(1024, 1024, 4);

    glm::mat4 projMatrix = glm::mat4(1.0);
    projMatrix = glm::ortho(-1.0, 1.0, -1.0, 1.0, -1.0, 10.0);
    glUniformMatrix4fv(projectionMatrixLocation, 1,false, &projMatrix[0][0]);
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

void runCompute()
{
    glUseProgram(computeID);
    glBindImageTexture(0, outputImageID, 0, GL_FALSE, 0, GL_WRITE_ONLY,
                       GL_RGBA8);
    glDispatchCompute(8, 16, 1); // Global Work GRoup size.
    glUseProgram(programID1);
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);

    shader = new Shader();
    programID = shader->init(
                "shader_multiPass_1.vert",
                "shader_multiPass_1.frag");

    programID1 = shader->init(
                "shader_multiPass_2.vert",
                "shader_multiPass_2.frag");

    computeID = shader->init("compute_shader.com");
    if(computeID < 0)
    {
        printf("compute id is invalid %d\n", computeID);
    }

    glUseProgram(programID);
    initializeLocations(programID);
    offScreenFBO = initializeFBO(512, 512);
    initializeOffscreenFBO();
    outputImageID = initializeOutputImage(256, 256);
    runCompute();
}

void drawScene() {
    glBindFramebuffer(GL_FRAMEBUFFER, onScreenFBO);
    glUseProgram(programID1);
    initializeLocations(programID1);

    glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0.0, 0.0, 1.0), //pos
                                       glm::vec3(0.0, 0.0, 0.0),  //looking at
                                       glm::vec3(0.0, 1.0, 0.0)); //up vector
    glUniformMatrix4fv(viewMatrixLocation, 1, false, &viewMatrix[0][0]);

    glm::mat4 modelMatrix = glm::mat4();
    modelMatrix = glm::translate(glm::vec3(0.0, 0.0, 0.0));
    glUniformMatrix4fv(modelMatrixLocation, 1, false, &modelMatrix[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, outputImageID);
    glUniform1i(activeTextureLocation, 0);

    drawQuad();
    glFlush();
}

void draw() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawScene();
    glutSwapBuffers();
}

void timer(int val) {
    draw();
    val++;
    glutTimerFunc(50, timer, 0);
}

void resize(int w, int h)
{
    glViewport(0, 0, w, h);
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

    glutCreateWindow("Compute Shader");

    printf("%s\n", glGetString(GL_VERSION));
    printf("%s\n", glGetString(GL_VENDOR));
    printf("%s\n", glGetString(GL_EXTENSIONS));

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
