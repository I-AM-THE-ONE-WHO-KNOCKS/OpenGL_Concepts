
#ifndef CUBE_H_
#define CUBE_H_

#include <GL/glew.h>


class Cube {
public:
    int posLoc;
    int colorLoc;
    Cube(int programID);
	virtual ~Cube();

	void draw();
	void drawCube();
};

#endif /* CUBE_H_ */
