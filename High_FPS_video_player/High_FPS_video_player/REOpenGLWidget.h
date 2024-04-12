#pragma once

//#include <iostream>
//using namespace std;
#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QtOpenGL/QOpenGLFunctions_3_3_Core>
#include <QTimer>
#include <QOpenGLTexture>
#include "data_singleton.h"

extern "C"
{
#include <libswscale\swscale.h>
#include "libavutil\imgutils.h"
}

class ShaderArr;

class REOpenGLWidget : public QOpenGLWidget, QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:

    explicit REOpenGLWidget(QWidget* parent = nullptr);

    ~REOpenGLWidget();

    // 设置属性
    void setParameters();

    // 开始渲染
    void start();

protected:
    // 虚函数需要重写
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    // 连接信号和槽
    void connectSignalSlots();

private slots:
    // 处理窗口移动的槽函数
    void flush();

private:
    // 全局唯一的数据对象 支持多线程访问
    DataSingleton& dataSingleton;
    // 帧队列
    FFmpegSafeQueue<Myframe*>* frameQueue = nullptr;
    // 解码器
    AVCodecContext* codecCtx = nullptr;
    // 帧
    Myframe* myFrame = nullptr;
    // 播放视频帧的计时器
    QTimer* frameTimer;
    // 可以播放的pts区间
    double beforePTS = 0, currPTS = 0;


    // openGL
    // 着色器数组对象指针
    ShaderArr* shaderArr;
    // 视频着色器key
    unsigned int videoShaderKey = -1;

    unsigned int VBO, VAO;

    float vertices[30] = {
            1.0, 1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            -1.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };

    struct SwsContext* swsContext = NULL;
    AVFrame* frameN = nullptr;
    uint8_t* outBuffer = NULL;

    unsigned int texture0;

    // 视频帧纹理videoFrame
    QOpenGLTexture* videoFrameTexture;
    // 视频时间基
    AVRational vTimeBase;


    QTimer* countTimer;
    int count = 0;
};


