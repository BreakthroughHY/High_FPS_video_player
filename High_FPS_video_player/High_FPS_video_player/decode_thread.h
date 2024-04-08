#pragma once

#include "thread_base.h"
#include "data_singleton.h"

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
    FFmpegSafeQueue<AVFrame*>* frameQueue = nullptr;
    // ������
    AVCodecContext* codecCtx = nullptr;
    // ��ָ��
    AVPacket* packet = nullptr;
    // ֡
    AVFrame* frame = nullptr;
};