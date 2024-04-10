#include "shader_arr.h"

ShaderArr::ShaderArr()
{
}

ShaderArr::~ShaderArr()
{
    for (auto ele : shaderIdPool)
    {
        delete ele.second;
    }
}

// 根据传入的文件创建一个着色程序
unsigned int ShaderArr::loadShader(const char* vertexPath, const char* fragmentPath)
{
    unsigned int ret = -1;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned int> dis(0, std::numeric_limits<unsigned int>::max());
    ret = dis(gen);
    
    while (shaderIdPool.find(ret) != shaderIdPool.end())
    {
        ret = dis(gen);
    }

    /*QOpenGLShader vertexShader(QOpenGLShader::Vertex);
    QOpenGLShader fragmentShader(QOpenGLShader::Fragment);*/

    /*QString vertexShaderCode = Helper::loadShaderStr(vertexPath);
    QString fragmentShaderCode = Helper::loadShaderStr(fragmentPath);*/

    /*if (!vertexShader.compileSourceCode(vertexShaderCode) || !fragmentShader.compileSourceCode(fragmentShaderCode)) {
        qDebug() << "Failed to compile shaders";
        return -1;
    }*/

    QOpenGLShaderProgram* shaderProgram = new QOpenGLShaderProgram();
    /*shaderProgram->addShader(&vertexShader);
    shaderProgram->addShader(&fragmentShader);*/

    shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/ShaderFile/video_shader.vs");
    shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/ShaderFile/video_shader.fs");

    if (!shaderProgram->link()) {
        qDebug() << "Failed to link shader program";
        delete shaderProgram;
        return -1;
    }

    //// 着色器程序插入map容器
    shaderIdPool.insert(std::make_pair(ret, shaderProgram));
    // 向用户返回着色器程序的id
    return ret;
}

// 使用指定id的着色器程序
void ShaderArr::useShaderId(unsigned int key)
{
    // 使用指定id的着色程序
    shaderIdPool.at(key)->bind();
}

// 获取shader程序id
GLuint ShaderArr::getShaderId(unsigned int key)
{
    return shaderIdPool.at(key)->programId();
}