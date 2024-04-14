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

    // ��������
    void setParameters();

    // ��ʼ��Ⱦ
    void startOrStop(bool palyState);

protected:
    // �麯����Ҫ��д
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    // �����źźͲ�
    void connectSignalSlots();

signals:
    // ͨ����ת��ctr_bar���͵�ǰ��Ƶ���ŵ�λ��
    void sig_VideoPlayTimeTimeEdit(int second);

private slots:
    // �������ƶ��Ĳۺ���
    void flush();

private:
    // ȫ��Ψһ�����ݶ��� ֧�ֶ��̷߳���
    DataSingleton& dataSingleton;
    // ֡����
    FFmpegSafeQueue<Myframe*>* frameQueue = nullptr;
    // ������
    AVCodecContext* codecCtx = nullptr;
    // ֡
    Myframe* myFrame = nullptr;
    // ������Ƶ֡�ļ�ʱ��
    QTimer* frameTimer;
    // ���Բ��ŵ�pts����
    double beforePTS = 0, currPTS = 0;
    // ��ǰ�Ĳ���״̬
    bool palyState = false;


    // openGL
    // ��ɫ���������ָ��
    ShaderArr* shaderArr;
    // ��Ƶ��ɫ��key
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

    // ��Ƶ֡����videoFrame
    QOpenGLTexture* videoFrameTexture;
    // ��Ƶʱ���
    AVRational vTimeBase;


    QTimer* countTimer;
    int count = 0;
};


