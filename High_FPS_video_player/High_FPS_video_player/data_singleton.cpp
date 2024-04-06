#include "data_singleton.h"

DataSingleton* DataSingleton::instance = nullptr;
std::mutex DataSingleton::mutex;

// 获取DataSingleton唯一对象的方法
DataSingleton& DataSingleton::getInstance()
{
    if (instance == nullptr)
    {
        std::lock_guard<std::mutex> lock(mutex);
        if (instance == nullptr)
        {
            instance = new DataSingleton();
        }
    }
    return *instance;
}

// 设置FFmpeg需要的队列  （重置大小时内容将被清空）
bool DataSingleton::setFFmpegQueue(int vPNum, int aPNum, int vFNum, int aFNum)
{
    if (vPNum > 0 && aPNum > 0 && vFNum > 0 && aFNum > 0) // 数据非法时不执行
    {
        if (videoPacketQueue) // 视频包队列
            videoPacketQueue = new FFmpegSafeQueue<AVPacket>(vPNum);
        else
            videoPacketQueue->setCapacity(vPNum);

        if (audioPacketQueue) // 音频包队列
            audioPacketQueue = new FFmpegSafeQueue<AVPacket>(aPNum);
        else
            audioPacketQueue->setCapacity(aPNum);

        if (videoFrameQueue) // 视频帧队列
            videoFrameQueue = new FFmpegSafeQueue<AVCodec>(vFNum);
        else
            videoFrameQueue->setCapacity(vFNum);

        if (audioFrameQueue) // 音频帧队列
            audioFrameQueue = new FFmpegSafeQueue<AVCodec>(aFNum);
        else
            audioFrameQueue->setCapacity(aFNum);
    }
    else
    {
        return false;
    }
}

// FFmpeg队列的get函数返回值是引用
FFmpegSafeQueue<AVPacket>& DataSingleton::getVideoPacketQueue() { return *videoPacketQueue; }
FFmpegSafeQueue<AVPacket>& DataSingleton::getAudioPacketQueue() { return *audioPacketQueue; }
FFmpegSafeQueue<AVCodec>& DataSingleton::getVideoFrameQueue() { return *videoFrameQueue; }
FFmpegSafeQueue<AVCodec>& DataSingleton::getAudioFrameQueue() { return *audioFrameQueue; }