
#include "Cube.h"


float vertices[] = {
	    // front
	    -0.5f, 0.5f, 0.5f,   0.5f, 0.5f, 0.5f,   0.5f,-0.5f, 0.5f,  -0.5f,-0.5f, 0.5f,

	    // back
	     0.5f, 0.5f,-0.5f,  -0.5f, 0.5f,-0.5f,  -0.5f,-0.5f,-0.5f,   0.5f,-0.5f,-0.5f,

	    // top
	    -0.5f, 0.5f,-0.5f,   0.5f, 0.5f,-0.5f,   0.5f, 0.5f, 0.5f,  -0.5f, 0.5f, 0.5f,

	    // bottom
	    -0.5f,-0.5f, 0.5f,   0.5f,-0.5f, 0.5f,   0.5f,-0.5f,-0.5f,  -0.5f,-0.5f,-0.5f,

	    // right
	     0.5f, 0.5f, 0.5f,   0.5f, 0.5f,-0.5f,   0.5f,-0.5f,-0.5f,   0.5f,-0.5f, 0.5f,

	    // left
	    -0.5f, 0.5f,-0.5f, -0.5f, 0.5f, 0.5f,  -0.5f,-0.5f, 0.5f,  -0.5f,-0.5f,-0.5f,
    };

GLubyte indices[] = {
	    //front
	    0,1,2, 2,3,0,
	    //back
	    4,5,6, 6,7,4,
	    // top
	    8,9,10, 10,11,8,
	    // bottom
	    12,13,14,  14,15,12,
	    //right
	    16,17,18,  18,19,16,
	    //left
	    20,21,22,  22,23,20,
    };
float color[] = {
	1.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 1.0f };

    float normals[] = {
    // front
            0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1,
            // back
            0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1,
            // top
            0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0,
            // bottom
            0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0,
            // right
            1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0,
            // left
            -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, };
Cube::Cube(int programID) {
	// TODO Auto-generated constructor stub
    posLoc = glGetAttribLocation(programID, "a_Position");
    colorLoc = glGetAttribLocation(programID, "a_Color");

}

Cube::~Cube() {
	// TODO Auto-generated destructor stub
}

void Cube::draw() {

		//Enable the vertex, texture and normal state
        glEnableVertexAttribArray(posLoc);
        glEnableVertexAttribArray(colorLoc);

        glVertexAttribPointer(posLoc, 3, GL_FLOAT, false, 0, vertices);
        glVertexAttribPointer(colorLoc, 4, GL_FLOAT, false, 0, color);

		//Draw the vertices as triangles, based on the Index Buffer information
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);

        //Disable the client state before leaving
}

void Cube::drawCube() {
	//	glColor3f(0.0, 0.0, 1.0);
	//	glutSolidCube(1.0);
	glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
									  // Top face (y = 1.0f)
									  // Define vertices in counter-clockwise (CCW) order with normal pointing out
	glColor3f(0.0f, 1.0f, 0.0f);     // Green
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);

	// Bottom face (y = -1.0f)
	glColor3f(1.0f, 0.5f, 0.0f);     // Orange
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);

	// Front face  (z = 1.0f)
	glColor3f(1.0f, 0.0f, 0.0f);     // Red
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	// Back face (z = -1.0f)
	glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);

	// Left face (x = -1.0f)
	glColor3f(0.0f, 0.0f, 1.0f);     // Blue
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	// Right face (x = 1.0f)
	glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glEnd();  // End of drawing color-cube
}

