/*
 * Author: Umesh Prabhakar Pimpalakar
 * Date : August 2018
 */

//external headers
#include<GL/glew.h>
//internal headers
#include "triangle.h"
#include "string.h"

Triangle::Triangle()
{
}

Triangle::~Triangle()
{
}

void Triangle::setLocation(int pLoc, int cLoc)
{
    mPositionLocation = pLoc;
    mColorLocation = cLoc;
}

void Triangle::initializeVBO(float *ptr, int size){
    GLuint id;

    //generate buffer, how many and id
    glGenBuffers(1, &id);

    //bind the buffer to id
    glBindBuffer(GL_ARRAY_BUFFER, id);

    //allocate memory and
    //upload data to the buffer object
    glBufferData(GL_ARRAY_BUFFER, size, ptr, GL_STATIC_DRAW);

    //unbind the buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    mVBOId = id;
}

void Triangle::initializeVAO()
{
    GLuint id;

    //generate vartex array object and bind
    glGenVertexArrays(1, &id);
    glBindVertexArray(id);
    //enable vertex and color array
    glEnableVertexAttribArray(mPositionLocation);
    glEnableVertexAttribArray(mColorLocation);
    //bind the vertex buffer object to VAO
    glBindBuffer(GL_ARRAY_BUFFER, mVBOId);
    //attach the vertex attribute pointers
    glVertexAttribPointer(mPositionLocation, 2, GL_FLOAT, false, 24, 0);
    glVertexAttribPointer(mColorLocation, 4, GL_FLOAT, false, 24, (GLvoid*)(2*sizeof(float)));
    //unbind the VAO
    glBindVertexArray(0);
    //disable the vertex and color array
    glDisableVertexAttribArray(mPositionLocation);
    glDisableVertexAttribArray(mColorLocation);

    mVAOId = id;
}

void Triangle::keyboardusingsubdata(unsigned char c, int x, int y){
        //update the  triangle buffer object
        glBindBuffer(GL_ARRAY_BUFFER, mVBOId);
        float newPosition[] = {-1.0, -1.0};

        float* gpuMem = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
        gpuMem +=6;
        memcpy(gpuMem,newPosition,8);
}

void Triangle::keyboardusingmapbufferdata(unsigned char c, int x, int y){
        //update the  triangle buffer object
        glBindBuffer(GL_ARRAY_BUFFER, mVBOId);
       float newPosition[] = {-1.0, 1.0};

        //using this you can update the current VBO
        glBufferSubData(GL_ARRAY_BUFFER, 24, 8, newPosition);
}

void Triangle::drawTriangle(int pLoc, int cLoc, Tests t){
    setLocation(pLoc, cLoc);
    //enable vertex and color array
    glEnableVertexAttribArray(mPositionLocation);
    glEnableVertexAttribArray(mColorLocation);
    //Provide the source pointer of the locating color and vertex data;
    glVertexAttribPointer(mPositionLocation, 2, GL_FLOAT, false, 24, mVertices);
    glVertexAttribPointer(mColorLocation, 4, GL_FLOAT, false, 24, mVertices+2);
    //this is just for simulation purpose
    //all these test should be enabled once, in init.
    mTest = t;
    switch (mTest) {
    case BLENDING:
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        break;
    case CULLING:
        //you can also define the cull face by glcullface as front or back, by default it back face.
        //To check the back face culling you need to rotate the triangle. to do the rotation you need to update your shader
        //refer to sphere draw call with tranformation
        glCullFace(GL_FRONT); //setting the face as front and triangle will not get drawn
        glEnable(GL_CULL_FACE);
        break;
    case CLIPPING:
        break;
    case SCISSOR:
        glEnable(GL_SCISSOR_TEST);
        glScissor(100, 100, 300, 300);
        break;
    case STENCIL:
        glClear(GL_STENCIL_BUFFER_BIT);
        //disable writing to the color buffer
        glColorMask(false, false, false, false);

        //define stencil test, i,e when will fragment pass the stencil test
        glStencilFunc(GL_ALWAYS, 1.0, 1.0);// if the stencil buffer value is less that the ref(2nd param) value the this test will pass

        //define the stencil Op, i,e how to update the stencil buffer
        glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
        break;
    default:
        break;
    }
    //draw triangle
    glDrawArrays(GL_TRIANGLES, 0, 3);

    if(mTest == STENCIL){
        //enable writing to color bit
        glColorMask(true, true, true, true);

        //set up another stencil test
        glStencilFunc(GL_EQUAL, 1.0, 1.0);

        //define the stencil Op, i,e how to update the stencil buffer
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

        //call draw of the quad
        drawQuad(pLoc, cLoc);
    }
    //disable the vertex and color array
    glDisableVertexAttribArray(mPositionLocation);
    glDisableVertexAttribArray(mColorLocation);
}

void Triangle::drawQuad(int pLoc, int cLoc)
{
    setLocation(pLoc, cLoc);
    //enable vertex and color array
    glEnableVertexAttribArray(mPositionLocation);
    glEnableVertexAttribArray(mColorLocation);
    //Provide the source pointer of the locating color and vertex data;
    glVertexAttribPointer(mPositionLocation, 2, GL_FLOAT, false, 24, mQuad);
    glVertexAttribPointer(mColorLocation, 4, GL_FLOAT, false, 24, mQuad+2);

    //draw quad
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Triangle::drawVBOTriangle(int pLoc, int cLoc)
{
    //set the attirbute location
    setLocation(pLoc,cLoc);
    //initialize the vbo
    initializeVBO(mVertices, 18*sizeof(float));
    //enable vertex and color array
    glEnableVertexAttribArray(mPositionLocation);
    glEnableVertexAttribArray(mColorLocation);
    //bind the vbo for vertex position
    glBindBuffer(GL_ARRAY_BUFFER, mVBOId);
    glVertexAttribPointer(mPositionLocation, 2, GL_FLOAT, false, 24, 0);
    glVertexAttribPointer(mColorLocation, 4, GL_FLOAT, false, 24, (GLvoid*)(2*sizeof(float)));
    //draw triangle
    glDrawArrays(GL_TRIANGLES, 0, 3);
    //unbind the vbo
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //disable the vertex and color array
    glDisableVertexAttribArray(mPositionLocation);
    glDisableVertexAttribArray(mColorLocation);
}

void Triangle::drawVAOTriangle(int pLoc, int cLoc)
{
    //set the attribute location
    setLocation(pLoc, cLoc);
    //initialize the vbo
    initializeVBO(mVertices, 18*sizeof(float));
    //initialize the VAO
    initializeVAO();
    //enable the vertex attribute arrays
    glEnableVertexAttribArray(mPositionLocation);
    glEnableVertexAttribArray(mColorLocation);
    //bind the VAO
    glBindVertexArray(mVAOId);
    //draw triangle
    glDrawArrays(GL_TRIANGLES, 0, 3);
    //unbind the VAO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //disable the vertex and color array
    glDisableVertexAttribArray(mPositionLocation);
    glDisableVertexAttribArray(mColorLocation);
}

void Triangle::drawTriangleGS(int pLoc, int cLoc)
{
    //enable the attributes
    glEnableVertexAttribArray(pLoc);
    glEnableVertexAttribArray(cLoc);

    //attach data to the attributes
    glVertexAttribPointer(pLoc, 2, GL_FLOAT, false, 24, mVertices);
    glVertexAttribPointer(cLoc, 4, GL_FLOAT, false, 24, mVertices+2);

    //increase the point size to increase the visibility
    glPointSize(10.0);

    //draw
    glDrawArrays(GL_POINTS, 0, 3);

    //disable attributes
    glDisableVertexAttribArray(pLoc);
    glDisableVertexAttribArray(cLoc);
}

void Triangle::drawTriangleTess(int pLoc, int cLoc)
{
    //enable the attributes
    glEnableVertexAttribArray(pLoc);
    glEnableVertexAttribArray(cLoc);

    //attach data to the attributes
    glVertexAttribPointer(pLoc, 2, GL_FLOAT, false, 24, mVertices);
    glVertexAttribPointer(cLoc, 4, GL_FLOAT, false, 24, mVertices+2);

    //increase the point size to increase the visibility
    glPointSize(10.0);

    //draw
    glDrawArrays(GL_PATCHES, 0, 3);

    //disable attributes
    glDisableVertexAttribArray(pLoc);
    glDisableVertexAttribArray(cLoc);
}
