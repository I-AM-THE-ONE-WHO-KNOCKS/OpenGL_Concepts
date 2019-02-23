#include<GL/glew.h>
#include<GL/freeglut.h>
#include <iostream>
#include <stdio.h>
#include "Shader.h"
#include "triangle.h"

Shader* mShader;
Triangle* mTriangle;
GLint mProgramID;

GLint mPositionLocation;
GLint mColorLocation;

void init(){
    glClearColor(0.0,0.0,0.0,1.0);

    mShader = new Shader();
    //load shaders and create program
    mProgramID = mShader->init("shader-Tess.vert", "shader-Tess.frag",
                              "shader-Tess-ctrl.tes", "shader-Tess-eval.tes");
    if(mProgramID < 0){
       printf("invalid Program ID\n");
    }
    mShader->bind();

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

    mTriangle = new Triangle();
}


void draw(){
    std::cout<<"draw\n";
    glClear(GL_COLOR_BUFFER_BIT);
    //set the polygon draw mode to lines, done in this case in order to see the tesselation output
    //by default this is gl_fill so we get geometries with filled
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    mTriangle->drawTriangleTess(mPositionLocation, mColorLocation);
    glutSwapBuffers();

    //set the polygon mode back to default
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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

    glutCreateWindow("Tessellation Shader");

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

