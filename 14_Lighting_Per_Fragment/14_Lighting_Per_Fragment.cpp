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
GLint mNormalLocation;

void init(){
    glClearColor(0.0,0.0,0.0,1.0);
    glClearDepth(1.0);
    mShader = new Shader();
    //load shaders and create program
    mProgramID = mShader->init("shader_lighting_perfragment.vert", "shader_lighting_perfragment.frag");
    mShader->bind();
    mModelMatrixLocation = glGetUniformLocation(mProgramID,"u_ModelMatrix");
    if(mModelMatrixLocation < 0){
       printf("invalid location for u_ModelMatrix\n");
    }
    mProjectionMatrixLocation = glGetUniformLocation(mProgramID, "u_ProjectionMatrix");
    if(mProjectionMatrixLocation < 0){
       printf("invalid location for u_projectionMatrix\n");
    }
    mNormalLocation = glGetAttribLocation(mProgramID, "a_Normal");
    if(mNormalLocation < 0){
       printf("invalid location for a_Normal\n");
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
}


void draw(){
    std::cout<<"draw\n";
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    mSphere->drawSphereLighting(mPositionLocation,mNormalLocation, mColorLocation, mModelMatrixLocation, mProjectionMatrixLocation);
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

    glutCreateWindow("Per fragment lighting");

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

