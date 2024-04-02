#pragma once

//#include <iostream>
//using namespace std;
#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QtOpenGL/QOpenGLFunctions_3_3_Core>

class REOpenGLWidget : public QOpenGLWidget, QOpenGLFunctions_3_3_Core
{
public:

    explicit REOpenGLWidget(QWidget* parent = nullptr);

    ~REOpenGLWidget();

protected:
    // 虚函数需要重写
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    unsigned int VBO, VAO;

    unsigned int shaderProgram;

    float vertices[12] = {
    0.5f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f
    };

    const char* vertexShaderSource = "#version 330 core\n"
        "layout(location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

    const char* fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n\0";
};

