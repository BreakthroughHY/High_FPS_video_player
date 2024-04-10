#pragma once

#include <map>
#include <iostream>
#include <random>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include "helper.h"

class ShaderArr
{
public:
	ShaderArr();
	~ShaderArr();

	// ���ݴ�����ļ�����һ����ɫ����
	unsigned int loadShader(const char* vertexPath, const char* fragmentPath);
	// ʹ��ָ��id����ɫ������
	void useShaderId(unsigned int key);

	// ��ȡshader����id
	GLuint getShaderId(unsigned int key);

private:
	std::map<unsigned int, QOpenGLShaderProgram*> shaderIdPool;
};

