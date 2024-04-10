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

// ���ݴ�����ļ�����һ����ɫ����
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

    //// ��ɫ���������map����
    shaderIdPool.insert(std::make_pair(ret, shaderProgram));
    // ���û�������ɫ�������id
    return ret;
}

// ʹ��ָ��id����ɫ������
void ShaderArr::useShaderId(unsigned int key)
{
    // ʹ��ָ��id����ɫ����
    shaderIdPool.at(key)->bind();
}

// ��ȡshader����id
GLuint ShaderArr::getShaderId(unsigned int key)
{
    return shaderIdPool.at(key)->programId();
}