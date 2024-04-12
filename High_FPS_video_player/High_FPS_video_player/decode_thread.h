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

    // 设置属性
    void setParameters();

protected:
    void run() override; // 告诉编译器重写父类虚函数

private:
    // 全局唯一的数据对象 支持多线程访问
    DataSingleton& dataSingleton;
    // 线程类型(记录线程是处理视频还是音频)
    Types types;
    // 包队列
    FFmpegSafeQueue<AVPacket*>* packetQueue = nullptr;
    // 帧队列
    FFmpegSafeQueue<Myframe*>* vframeQueue = nullptr;
    FFmpegSafeQueue<AVFrame*>* aframeQueue = nullptr;
    // 解码器
    AVCodecContext* codecCtx = nullptr;
    // 包指针
    AVPacket* packet = nullptr;
    // 帧
    AVFrame* frame = nullptr;
    Myframe* myFrame = nullptr;
    // 视频转码需要的空间
    int outBufferSize;
    uint8_t* outBuffer = NULL;
    // 视频转码结构体
    struct SwsContext* swsContext = NULL;
};