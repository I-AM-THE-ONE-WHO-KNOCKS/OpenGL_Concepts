/*
 * Author: Umesh Prabhakar Pimpalakar
 * Date : August 2018
 */

#ifndef SPHERE_H
#define SPHERE_H

//external headers
#include<GL/glew.h>
#include<FreeImage.h>
//internal headers


class Sphere
{
public:

    //image loader
    BYTE* initializeImage(const char* filepath);

    //initialize the textures
    GLuint initializeTexture(const char* filepath);

    //initialize specular texture
    GLuint initializeSpecularTexture(const char* textureFile);

    //getter for different texture ID's
    void getTextureIDs();

    //getter for specular texture ID's
    void getSpecularTextureIDs();

    //generates the vertices, colors, normals, texture coordinates of sphere
    void initSphere(GLint stacks, GLint slices, GLfloat radius, GLfloat squash);

    //draw sphere: this draw simulates the different matrices we have.
    void drawSphereMatrices(int pLoc, int cLoc, int vmLoc, int mmLoc, int mProjm);

    //draw sphere: this draw simulates the different transformation we have
    // i,e translate, rotate and scale
    void drawSphereTransformation(int pLoc, int cLoc, int vmLoc, int mmLoc, int mProjm);

    //draw sphere: this draw simulates per vertex lighting and per fragment
    void drawSphereLighting(int pLoc, int nLoc, int cLoc, int mmLoc, int mProjm);

    //draw sphere: this draw simulates the texture loading
    //draws sun, earth and moon with texture
    void drawSphereTexture(int pLoc, int cLoc, int tLoc, int aTLoc, int vmLoc, int mmLoc, int mProjm);

    //draw sphere: this draw simulates specular texture mapping
    void drawSphereSpecular(int pLoc, int cLoc, int nLoc, int tLoc, int aTloc, int saTLoc, int mmLoc, int mProjm);

    void drawSphere(int pLoc, int nLoc, int cLoc, int tloc);

    float mAngle = 0.0; //used to simulate the roatation
    float mZPos = 0.0; //used to simulate the translate

    int mTexWidth = 0.0; //texture image width
    int mTexHeight = 0.0; //texture image height

    int mSunTextureID; //texture id for sun texture
    int mEarthTextureID; //texture id for earth texture
    int mMoonTextureID; // texture id for moon texture
    int mEarthSpecularTexID; // texture used for specular mapping

    int mSpecularTextureID; // specular texture id
};

#endif //SPHERE_H
