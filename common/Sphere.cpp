/*
 * Author: Umesh Prabhakar Pimpalakar
 * Date : August 2018
 */

//external headers
#include <GL/glew.h>
#include <stdlib.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <FreeImage.h>
#include <stdio.h>
#include "iostream"
//internal headers
#include "Sphere.h"

#define M_PI 3.14159265358979323846


GLfloat			*m_VertexData;
GLubyte			*m_ColorData;
GLfloat			*m_NormalData;
GLfloat         *m_TexCoordsData;
GLint			m_Stacks, m_Slices;
GLfloat			m_Scale;						
GLfloat			m_Squash;
GLfloat			m_Angle;
GLfloat			m_Pos[3];

BYTE* Sphere::initializeImage(const char* filepath)
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
    mTexWidth = FreeImage_GetWidth(dib);
    mTexHeight = FreeImage_GetHeight(dib);

    return image;
}

GLuint Sphere::initializeTexture(const char *filepath)
{
    //texture id
    GLuint id;
    //generate and bind textures
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    //load image
    BYTE* data = initializeImage(filepath);
    if(data != NULL)
    {
        //attach image to texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mTexWidth, mTexHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

        // define filtering i.e minification and magnification
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        //unbind texture
        glBindTexture(GL_TEXTURE_2D, 0);

        return id;
    }

    return UINT_MAX;
}

GLuint Sphere::initializeSpecularTexture(const char* textureFile){
    //specular texture id
    GLuint id;
    //generate texture
    glGenTextures(1, &id);
    //bind texture
    glBindTexture(GL_TEXTURE_2D, id);
    //load texture data
    BYTE* data = initializeImage(textureFile);
    if (data != NULL) {
        //attach texture
        glTexImage2D(GL_TEXTURE_2D, 0,
            GL_ALPHA, mTexWidth, mTexHeight, 0,
            GL_ALPHA, GL_UNSIGNED_BYTE, data);

        if (GL_NO_ERROR == glGetError()) {
            printf("Going fine\n");
        }
        //define filtering i.e minification and magnification
        glTexParameteri(GL_TEXTURE_2D,
            GL_TEXTURE_MIN_FILTER,
            GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,
            GL_TEXTURE_MAG_FILTER,
            GL_LINEAR);
        //unbind the texture
        glBindTexture(GL_TEXTURE_2D, 0);

        return id;
    }

    return UINT_MAX;
}

void Sphere::getTextureIDs()
{
    mSunTextureID = initializeTexture("sun.jpg");
    mEarthTextureID = initializeTexture("earth.jpg");
    mMoonTextureID = initializeTexture("moon.png");
    mEarthSpecularTexID = initializeTexture("earth_specular.jpg");
}

void Sphere::getSpecularTextureIDs()
{
    mSpecularTextureID = initializeSpecularTexture("earth_specular_map.jpg");
}

void Sphere::initSphere(GLint stacks, GLint slices, GLfloat radius, GLfloat squash)
{
    unsigned int colorIncrment=0;
    unsigned int blue=10;
    unsigned int red=10;
    int numVertices=0;
    
    m_Scale=radius;
    m_Squash=squash;
    
    colorIncrment=255/stacks;
    
        m_Stacks = stacks;
        m_Slices = slices;
        m_VertexData = NULL;
        m_TexCoordsData = NULL;
        
        //vertices
        
        GLfloat *vPtr = m_VertexData =
        (GLfloat*)malloc(sizeof(GLfloat) * 3 * ((m_Slices*2+2) *
                                                (m_Stacks)));
        
        //color data
        
        GLubyte *cPtr = m_ColorData =
        (GLubyte*)malloc(sizeof(GLubyte) * 4 * ((m_Slices*2+2) *
                                                (m_Stacks)));
        
        //normal pointers for lighting
        
        GLfloat *nPtr = m_NormalData = (GLfloat*)
        malloc(sizeof(GLfloat) * 3 * ((m_Slices*2+2) * (m_Stacks)));
        
        GLfloat *tPtr=NULL;                                          //3
        
            tPtr=m_TexCoordsData =
            (GLfloat *)malloc(sizeof(GLfloat) * 2 * ((m_Slices*2+2) *
                                                     (m_Stacks)));
        
        unsigned int phiIdx, thetaIdx;
        
        //latitude
        
        for(phiIdx=0; phiIdx < m_Stacks; phiIdx++)
        {
            //starts at -1.57 goes up to +1.57 radians
            
            //the first circle
            
            float phi0 = M_PI * ((float)(phiIdx+0) * (1.0/(float)
                                                      (m_Stacks)) - 0.5);
            
            //the next, or second one.
            
            float phi1 = M_PI * ((float)(phiIdx+1) * (1.0/(float)
                                                      (m_Stacks)) - 0.5);
            float cosPhi0 = cos(phi0);
            float sinPhi0 = sin(phi0);
            float cosPhi1 = cos(phi1);
            float sinPhi1 = sin(phi1);
            
            float cosTheta, sinTheta;
            
            //longitude
            
            for(thetaIdx=0; thetaIdx < m_Slices; thetaIdx++)
            {
                //Increment along the longitude circle each "slice."
                
                float theta = -2.0*M_PI * ((float)thetaIdx) *
                (1.0/(float)(m_Slices-1));
                cosTheta = cos(theta);
                sinTheta = sin(theta);
                
                //We're generating a vertical pair of points, such
                //as the first point of stack 0 and the first point
                //of stack 1
                //above it. This is how TRIANGLE_STRIPS work,
                //taking a set of 4 vertices and essentially drawing
                //two triangles
                //at a time. The first is v0-v1-v2 and the next is
                //v2-v1-v3. Etc.
                
                //Get x-y-z for the first vertex of stack.
                
                vPtr[0] = m_Scale*cosPhi0 * cosTheta;
                vPtr[1] = m_Scale*sinPhi0*m_Squash;
                vPtr[2] = m_Scale*(cosPhi0 * sinTheta);
                
                //the same but for the vertex immediately above the
                //previous one
                
                vPtr[3] = m_Scale*cosPhi1 * cosTheta;
                vPtr[4] = m_Scale*sinPhi1*m_Squash;
                vPtr[5] = m_Scale*(cosPhi1 * sinTheta);
                
                //normal pointers for lighting
                
                nPtr[0] = cosPhi0 * cosTheta;
                nPtr[2] = cosPhi0 * sinTheta;
                nPtr[1] = sinPhi0;
                
                nPtr[3] = cosPhi1 * cosTheta;
                nPtr[5] = cosPhi1 * sinTheta;
                nPtr[4] = sinPhi1;
                
                if(tPtr!=NULL)                               //4
                {
                    GLfloat texX = (float)thetaIdx *
                    (1.0f/(float)(m_Slices-1));
                    tPtr[0] = texX;
                    tPtr[1] = (float)(phiIdx+0) *
                    (1.0f/(float)(m_Stacks));
                    tPtr[2] = texX;
                    tPtr[3] = (float)(phiIdx+1) *
                    (1.0f/(float)(m_Stacks));
                }
                
                cPtr[0] = red;
                cPtr[1] = 0;
                cPtr[2] = blue;
                cPtr[4] = red;
                cPtr[5] = 0;
                cPtr[6] = blue;
                cPtr[3] = cPtr[7] = 255;
                
                cPtr += 2*4;
                vPtr += 2*3;
                nPtr += 2*3;
                
                
                if(tPtr!=NULL)                               //5
                    tPtr += 2*2;
            }
            
            blue+=colorIncrment;
            red-=colorIncrment;
            
            // Degenerate triangle to connect stacks and maintain
            //winding order.
            
            vPtr[0] = vPtr[3] = vPtr[-3];
            vPtr[1] = vPtr[4] = vPtr[-2];
            vPtr[2] = vPtr[5] = vPtr[-1];
            
            nPtr[0] = nPtr[3] = nPtr[-3];
            nPtr[1] = nPtr[4] = nPtr[-2];
            nPtr[2] = nPtr[5] = nPtr[-1];
            
            if(tPtr!=NULL)
            {
                tPtr[0] = tPtr[2] = tPtr[-2];         //6
                tPtr[1] = tPtr[3] = tPtr[-1];
            }
            
        }
        
        numVertices=(vPtr-m_VertexData)/6;
}

void Sphere::drawSphereMatrices(int pLoc, int cLoc, int vmLoc, int mmLoc, int mProjm)
{
    //define view matrix
    glm::mat4 viewMatrix = glm::mat4(1.0);
    viewMatrix = glm::lookAt(glm::vec3(0.0,0.0,1.0),//pos
                             glm::vec3(0.0,0.0,0.0),//looking
                             glm::vec3(0.0, 1.0, 0.0));//up vector
    glUniformMatrix4fv(vmLoc, 1, false, &viewMatrix[0][0]);

    //define model matrix
    glm::mat4 modelMatrix = glm::mat4();
    glUniformMatrix4fv(mmLoc, 1, false, &modelMatrix[0][0]);

    glm::mat4 projMatrix  = glm::mat4(1.0);
    projMatrix = glm::perspective(glm::radians(45.0), 1.0, 0.1, 100.0);//perspective with perspective
    glUniformMatrix4fv(mProjm, 1, false, &projMatrix[0][0]);

    //enable vertex attributes
    glEnableVertexAttribArray(pLoc);
    glEnableVertexAttribArray(cLoc);
    //attach data to vertex attributes
    glVertexAttribPointer(pLoc, 3, GL_FLOAT, false, 0, m_VertexData);
    glVertexAttribPointer(cLoc, 4, GL_UNSIGNED_BYTE, true, 0, m_ColorData);

    //draw sphere
    glDrawArrays(GL_TRIANGLE_STRIP, 0, (m_Slices+1)*2*(m_Stacks-1)+2);

    //disable vertex attributes
    glDisableVertexAttribArray(pLoc);
    glDisableVertexAttribArray(cLoc);

}

void Sphere::drawSphereTransformation(int pLoc, int cLoc, int vmLoc, int mmLoc, int mProjm)
{
    mAngle += 1.0;
    if(mAngle >= 360.0) mAngle = 0.0;
    float rad = glm::radians(mAngle);

    mZPos = -1.0;

    //enable vertex attributes
    glEnableVertexAttribArray(pLoc);
    glEnableVertexAttribArray(cLoc);
    //attach data to vertex attributes
    glVertexAttribPointer(pLoc, 3, GL_FLOAT, false, 0, m_VertexData);
    glVertexAttribPointer(cLoc, 4, GL_UNSIGNED_BYTE, true, 0, m_ColorData);

    //define view matrix
    glm::mat4 viewMatrix = glm::mat4(1.0);
    viewMatrix = glm::lookAt(glm::vec3(0.0,0.0,1.0),//pos
                             glm::vec3(0.0,0.0,0.0),//looking
                             glm::vec3(0.0, 1.0, 0.0));//up vector
    glUniformMatrix4fv(vmLoc, 1, false, &viewMatrix[0][0]);

    glm::mat4 projMatrix  = glm::mat4(1.0);
    projMatrix = glm::perspective(glm::radians(45.0), 1.0, 0.1, 100.0);//perspective with perspective
    glUniformMatrix4fv(mProjm, 1, false, &projMatrix[0][0]);

    //define model matrix
    glm::mat4 modelMatrix = glm::mat4();
    //apply translation
    modelMatrix = glm::translate(glm::vec3(0.0, 0.0, mZPos));
    //apply rotation
    modelMatrix = glm::rotate(modelMatrix, glm::radians(mAngle), glm::vec3(0.0,1.0,0.0));
    //apply scale
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.7, 0.7, 0.7));
    glUniformMatrix4fv(mmLoc, 1, false, &modelMatrix[0][0]);

    //draw sphere
    glDrawArrays(GL_LINES, 0, (m_Slices+1)*2*(m_Stacks-1)+2);

    //disable vertex attributes
    glDisableVertexAttribArray(pLoc);
    glDisableVertexAttribArray(cLoc);
}

void Sphere::drawSphereLighting(int pLoc, int nLoc, int cLoc, int mmLoc, int mProjm)
{
    //define model matrix
    glm::mat4 modelMatrix = glm::mat4(1.0);
    modelMatrix = glm::translate(glm::vec3(0.0, 0.0, -1));
    glUniformMatrix4fv(mmLoc, 1, false, &modelMatrix[0][0]);


    glm::mat4 projMatrix  = glm::mat4(1.0);
    projMatrix = glm::perspective(glm::radians(45.0), 1.0, 0.1, 100.0);//perspective with perspective
    glUniformMatrix4fv(mProjm, 1, false, &projMatrix[0][0]);


    //enable vertex attributes
    glEnableVertexAttribArray(pLoc);
    glEnableVertexAttribArray(cLoc);
    glEnableVertexAttribArray(nLoc);
    //attach data to vertex attributes
    glVertexAttribPointer(pLoc, 3, GL_FLOAT, false, 0, m_VertexData);
    glVertexAttribPointer(cLoc, 4, GL_UNSIGNED_BYTE, true, 0, m_ColorData);
    glVertexAttribPointer(nLoc, 3, GL_FLOAT, false, 0, m_NormalData);

    //draw sphere
    glDrawArrays(GL_TRIANGLE_STRIP, 0, (m_Slices+1)*2*(m_Stacks-1)+2);

    //disable vertex attributes
    glDisableVertexAttribArray(pLoc);
    glDisableVertexAttribArray(cLoc);
    glDisableVertexAttribArray(nLoc);
}

void Sphere::drawSphereTexture(int pLoc, int cLoc, int tLoc, int aTLoc, int vmLoc, int mmLoc, int mProjm)
{
    mAngle += 1.0;
    if(mAngle >= 360.0) mAngle = 0.0;

    mZPos = -1.0;

    //define view matrix
    glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0.0, 0.0, 1.0),  //pos
                                       glm::vec3(0.0, 0.0, 0.0),  //looking at
                                       glm::vec3(0.0, 1.0, 0.0)); //up vector
    glUniformMatrix4fv(vmLoc, 1, false, &viewMatrix[0][0]);

    //define model matrix
    glm::mat4 modelMatrix = glm::mat4(1.0);
    modelMatrix = glm::translate(glm::vec3(0.0, 0.0, mZPos));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(mAngle),
                              glm::vec3(0.0, 1.0, 0.0));
    glUniformMatrix4fv(mmLoc, 1, false, &modelMatrix[0][0]);

    glm::mat4 projMatrix  = glm::mat4(1.0);
    projMatrix = glm::perspective(glm::radians(45.0), 1.0, 0.1, 100.0);//perspective with perspective
    glUniformMatrix4fv(mProjm, 1, false, &projMatrix[0][0]);


    //enable vertex attributes
    glEnableVertexAttribArray(pLoc);
    glEnableVertexAttribArray(cLoc);
    glEnableVertexAttribArray(tLoc);
    //attach data to vertex attributes
    glVertexAttribPointer(pLoc, 3, GL_FLOAT, false, 0, m_VertexData);
    glVertexAttribPointer(cLoc, 4, GL_UNSIGNED_BYTE, true, 0, m_ColorData);
    glVertexAttribPointer(tLoc, 2, GL_FLOAT, false, 0, m_TexCoordsData);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mSunTextureID);
    glUniform1i(aTLoc, 0);

    //draw Sun
    glDrawArrays(GL_TRIANGLE_STRIP, 0, (m_Slices+1)*2*(m_Stacks-1)+2);

    modelMatrix = glm::translate(modelMatrix, glm::vec3(4.0, 0.0, 0.0));
    modelMatrix = glm::rotate(modelMatrix,
                              glm::radians(mAngle+30), glm::vec3(0.0, 1.0, 0.0));

    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5, 0.5, 0.5));
    glUniformMatrix4fv(mmLoc, 1, false, &modelMatrix[0][0]);

    glBindTexture(GL_TEXTURE_2D, mEarthTextureID);

    //draw earth
    glDrawArrays(GL_TRIANGLE_STRIP, 0, (m_Slices+1)*2*(m_Stacks-1)+2);

    modelMatrix = glm::translate(modelMatrix, glm::vec3(3.0, 0.0, 0.0));
    modelMatrix = glm::rotate(modelMatrix,
                              glm::radians(mAngle+100), glm::vec3(0.0, 1.0, 0.0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5, 0.5, 0.5));
    //draw the moon
    glUniformMatrix4fv(mmLoc, 1, false, &modelMatrix[0][0]);

    glBindTexture(GL_TEXTURE_2D, mMoonTextureID);

    //draw moon
    glDrawArrays(GL_TRIANGLE_STRIP, 0, (m_Slices+1)*2*(m_Stacks-1)+2);

    //disable vertex attributes
    glDisableVertexAttribArray(pLoc);
    glDisableVertexAttribArray(cLoc);
    glDisableVertexAttribArray(tLoc);
}

void Sphere::drawSphereSpecular(int pLoc, int cLoc, int nLoc, int tLoc, int aTloc, int saTLoc, int mmLoc, int mProjm)
{
    mAngle += 1.0;
    if(mAngle >= 360.0) mAngle = 0.0;

    mZPos = -1.0;

    glm::mat4 modelmatrix = glm::mat4();
    modelmatrix = glm::translate(glm::vec3(0.0f, 0.0f, mZPos));
    modelmatrix = glm::rotate(modelmatrix, glm::radians(mAngle), glm::vec3(0.0, 1.0, 0.0));
    glUniformMatrix4fv(mmLoc, 1, false, &modelmatrix[0][0]);

    glm::mat4 projMatrix  = glm::mat4(1.0);
    projMatrix = glm::perspective(glm::radians(45.0), 1.0, 0.1, 100.0);//perspective with perspective
    glUniformMatrix4fv(mProjm, 1, false, &projMatrix[0][0]);

    //enable vertex attributes
    glEnableVertexAttribArray(pLoc);
    glEnableVertexAttribArray(cLoc);
    glEnableVertexAttribArray(tLoc);
    glEnableVertexAttribArray(nLoc);
    //attach data to vertex attributes
    glVertexAttribPointer(pLoc, 3, GL_FLOAT, false, 0, m_VertexData);
    glVertexAttribPointer(cLoc, 4, GL_UNSIGNED_BYTE, true, 0, m_ColorData);
    glVertexAttribPointer(nLoc, 3, GL_FLOAT, false, 0, m_NormalData);
    glVertexAttribPointer(tLoc, 2, GL_FLOAT, false, 0, m_TexCoordsData);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mEarthSpecularTexID);
    glUniform1i(aTloc, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mSpecularTextureID);
    glUniform1i(saTLoc, 1);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, (m_Slices+1)*2*(m_Stacks-1)+2);

    //disable vertex attributes
    glDisableVertexAttribArray(pLoc);
    glDisableVertexAttribArray(cLoc);
    glDisableVertexAttribArray(nLoc);
    glDisableVertexAttribArray(tLoc);
}

void Sphere::drawSphere(int pLoc, int nLoc, int cLoc, int tloc)
{
    //glEnableClientState(GL_VERTEX_ARRAY);
    //glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableVertexAttribArray(pLoc);
   // glEnableVertexAttribArray(nLoc);
    glEnableVertexAttribArray(cLoc);
    glEnableVertexAttribArray(tloc);

    glVertexAttribPointer(pLoc, 3, GL_FLOAT, false, 0, m_VertexData);
    glVertexAttribPointer(cLoc, 4, GL_UNSIGNED_BYTE, true, 0, m_ColorData);
    glVertexAttribPointer(tloc, 2, GL_FLOAT, false, 0, m_TexCoordsData);
   // glVertexAttribPointer(nLoc, 3, GL_FLOAT, false, 0, m_NormalData);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, (m_Slices+1)*2*(m_Stacks-1)+2);
    glDisableVertexAttribArray(pLoc);
   // glDisableVertexAttribArray(nLoc);
    glDisableVertexAttribArray(cLoc);
    glDisableVertexAttribArray(tloc);
    //glDisableClientState(GL_VERTEX_ARRAY);
    //glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}
