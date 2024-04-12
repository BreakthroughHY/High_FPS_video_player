#pragma once

#include "thread_base.h"
#include "data_singleton.h"

extern "C"
{
#include <libswscale\swscale.h>
#include "libavutil\imgutils.h"
}

class DecodeThread : public ThreadBase
{
public:
    enum class Types {
        VIDEO,
        AUDIO
    };

    DecodeThread(Types types);

    ~DecodeThread();

    // ��������
    void setParameters();

protected:
    void run() override; // ���߱�������д�����麯��

private:
    // ȫ��Ψһ�����ݶ��� ֧�ֶ��̷߳���
    DataSingleton& dataSingleton;
    // �߳�����(��¼�߳��Ǵ�����Ƶ������Ƶ)
    Types types;
    // ������
    FFmpegSafeQueue<AVPacket*>* packetQueue = nullptr;
    // ֡����
    FFmpegSafeQueue<Myframe*>* vframeQueue = nullptr;
    FFmpegSafeQueue<AVFrame*>* aframeQueue = nullptr;
    // ������
    AVCodecContext* codecCtx = nullptr;
    // ��ָ��
    AVPacket* packet = nullptr;
    // ֡
    AVFrame* frame = nullptr;
    Myframe* myFrame = nullptr;
    // ��Ƶת����Ҫ�Ŀռ�
    int outBufferSize;
    uint8_t* outBuffer = NULL;
    // ��Ƶת��ṹ��
    struct SwsContext* swsContext = NULL;
};