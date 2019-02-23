#ifndef SHADER_H
#define SHADER_H
#include <GL/glew.h>
#include <stdlib.h>

class Shader {
public:
	Shader();
	~Shader();
	GLuint init(const char* csFile);
	GLuint init(const char *vsFile, const char *fsFile);
	GLuint init(const char *vsFile, const char *fsFile, const char* gs);
	GLuint init(const char *vsFile, const char *fsFile, const char* tcs, const char* tes);

	unsigned int id();

	void bind();

	void unbind();
private:
private:
	unsigned int shader_id;
	unsigned int shader_vp;
	unsigned int shader_fp;
	unsigned int shader_gp;
	unsigned int shader_tcsp;
	unsigned int shader_tesp;
	unsigned int shader_cp;

};
#endif
