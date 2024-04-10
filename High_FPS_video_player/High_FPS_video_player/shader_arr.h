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

	// 根据传入的文件创建一个着色程序
	unsigned int loadShader(const char* vertexPath, const char* fragmentPath);
	// 使用指定id的着色器程序
	void useShaderId(unsigned int key);

	// 获取shader程序id
	GLuint getShaderId(unsigned int key);

private:
	std::map<unsigned int, QOpenGLShaderProgram*> shaderIdPool;
};

