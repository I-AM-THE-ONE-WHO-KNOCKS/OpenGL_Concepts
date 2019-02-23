#include<GL/glew.h>
#include<GL/freeglut.h>
#include <iostream>
#include <stdio.h>
#include "Shader.h"
#include "Sphere.h"

Shader* mShader;
Sphere* mSphere;
GLint mProgramID;

GLint mPositionLocation;
GLint mColorLocation;
GLint mModelMatrixLocation;
GLint mProjectionMatrixLocation;
GLint mViewMatrixLocation;
GLint mActiveTextureLocation;
GLint mActiveSpecularTextureLocation;
GLint mNormalLocation;
GLint mTextureLocation;

void init(){
    glClearColor(1.0,1.0,1.0,1.0);
    glClearDepth(1.0);
    mShader = new Shader();
    //load shaders and create program
    mProgramID = mShader->init("shader_specular.vert", "shader_specular.frag");
    mShader->bind();
    mModelMatrixLocation = glGetUniformLocation(mProgramID,"u_ModelMatrix");
    if(mModelMatrixLocation < 0){
       printf("invalid location for u_ModelMatrix\n");
    }
    mProjectionMatrixLocation = glGetUniformLocation(mProgramID, "u_ProjectionMatrix");
    if(mProjectionMatrixLocation < 0){
       printf("invalid location for u_projectionMatrix\n");
    }
    mActiveTextureLocation =
            glGetUniformLocation(mProgramID, "activeTexture");
    if(mActiveTextureLocation < 0) {
        printf("Invalid location for activeTexture");
    }
    mActiveSpecularTextureLocation =
            glGetUniformLocation(mProgramID, "specularactiveTexture");
    if(mActiveSpecularTextureLocation < 0) {
        printf("Invalid location for specularactiveTexture");
    }
    mNormalLocation = glGetAttribLocation(mProgramID, "a_Normal");
    if(mNormalLocation < 0){
       printf("invalid location for a_Normal\n");
    }
    mTextureLocation =
            glGetAttribLocation(mProgramID, "a_TextureCoordinates");
    if(mTextureLocation < 0) {
        printf("Invalid location for a_TextureCoordinates");
    }
    //get vertex postion location
    mPositionLocation = glGetAttribLocation(mProgramID, "a_Position");
    if(mPositionLocation < 0){
       printf("invalid location for a_Position\n");
    }
    //get vertex color location
    mColorLocation = glGetAttribLocation(mProgramID, "a_Color");
    if(mColorLocation < 0){
       printf("invalid location for a_Color\n");
    }

    mSphere = new Sphere();
    mSphere->initSphere(50, 50, 0.2, 1.0);
    mSphere->getSpecularTextureIDs();
    mSphere->getTextureIDs();
}


void draw(){
    std::cout<<"draw\n";
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    mSphere->drawSphereSpecular(mPositionLocation, mColorLocation, mNormalLocation, mTextureLocation,
                                mActiveTextureLocation, mActiveSpecularTextureLocation, mModelMatrixLocation, mProjectionMatrixLocation);
    glutSwapBuffers();
    glDisable(GL_DEPTH_TEST);
}

void timer(int val){
    draw();
    glutTimerFunc(val+1000,timer,0);
}


int main(int argc, char** argv)
{
    //initialize the glut library
    glutInit(&argc, argv);

    //set the appropriate display mode
    //this configures the frame buffer
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

    //define the size of the window
    glutInitWindowSize(600, 600);

    glutCreateWindow("Specular Map");

    std::cout<<glGetString(GL_VERSION)<<"\n";
    std::cout<<glGetString(GL_VENDOR)<<"\n";
    std::cout<<glGetString(GL_EXTENSIONS)<<"\n";


    //initialize the glew library
    glewInit();

    init();

    //when display gets refreshed
    glutDisplayFunc(draw);

    //called in loop as fast as possible
    //glutIdleFunc(draw);

    //timer function
    glutTimerFunc(1000,timer,0);

    //start the glut event loop
    glutMainLoop();

    return 0;
}

