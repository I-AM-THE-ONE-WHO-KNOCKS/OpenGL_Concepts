/*
 * Author: Umesh Prabhakar Pimpalakar
 * Date : August 2018
 */

#ifndef TRIANGLE_H
#define TRIANGLE_H

//external headers
#include<GL/glew.h>
//internal headers

class Triangle
{
    //create vertex array
    float mVertices[18] = {0.5,0.0,  1.0, 0.0, 0.0, 0.2,
                           0.0,0.5,  0.0, 1.0, 0.0, 0.2,
                          -0.5,0.0,  0.0, 0.0, 1.0, 0.2};
    //quad used for stencil test
    float mQuad[24] = {0.5, 0.5,  1.0, 0.0, 0.0, 1.0,
                    -0.5, 0.5,  0.0, 1.0, 0.0, 1.0,
                    0.5, -0.5,  0.0, 0.0, 1.0 ,1.0,
                   -0.5, -0.5,  0.0, 0.0, 1.0 ,1.0};

    //vertex location ID of shader program
    GLint mPositionLocation;
    //color location ID of shader program
    GLint mColorLocation;
    //vbo id for vertex attributes
    GLuint mVBOId;
    //vao id for vertex attributes
    GLuint mVAOId;


public:
    //contsructor
    Triangle();
    //distructor
    ~Triangle();
    //enum for enable the different tests
    enum Tests
    {
        BLENDING = 11, //to enable blending test
        CULLING, // to enabel culling test
        CLIPPING, // to enable clipping test
        SCISSOR, // to enable scissor test
        STENCIL,
        DEFAULT
    }mTest;

    //set attributes location
    void setLocation(int pLoc, int cLoc);
    //initialize vertex buffer object
    void initializeVBO(float* ptr, int size);
    //initialize VAO
    void initializeVAO();
    //keyboard function used when keyboerd action happens
    //in this case it is used for simulation how buffer data can be used to update the
    //existing VBO dynamically using subbuffer data
    void keyboardusingsubdata(unsigned char c, int x, int y);
    //in this case it is used for simulation how buffer data can be used to update the
    //existing VBO dynamically using map
    void keyboardusingmapbufferdata(unsigned char c, int x, int y);
    //draw function to draw triangle on the screen using the indibidual arrays as display lists.
    void drawTriangle(int pLoc, int cLoc, Tests t);
    //draw quad, used only for stencil test
     void drawQuad(int pLoc, int cLoc);
    //draw function to draw triangle on the screen using the VBO as display lists
    void drawVBOTriangle(int pLoc, int cLoc);
    //draw function to draw triangle on the screen using the VAO as display lists
    void drawVAOTriangle(int pLoc, int cLoc);
    //draw function to draw triangle on screen simulating the geometry shader
    //In this draw call the geometry shader takes one triangle as a inpute and
    //converts the same into three triangles and draws as points
    void drawTriangleGS(int pLoc, int cLoc);
    //draw function to draw triangle on screen simulating the tesselation shader
    //In this draw call single triangle is sent to pipeline and tesselation shader
    //and divides this geometry as per inpute to the tesselation
    void drawTriangleTess(int pLoc, int cLoc);
};

#endif // TRIANGLE_H
