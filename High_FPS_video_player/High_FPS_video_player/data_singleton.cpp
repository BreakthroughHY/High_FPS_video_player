#include "data_singleton.h"

DataSingleton* DataSingleton::instance = nullptr;
std::mutex DataSingleton::mutex;

// ��ȡDataSingletonΨһ����ķ���
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

// ����FFmpeg��Ҫ�Ķ���  �����ô�Сʱ���ݽ�����գ�
bool DataSingleton::setFFmpegQueue(int vPNum, int aPNum, int vFNum, int aFNum)
{
    if (vPNum > 0 && aPNum > 0 && vFNum > 0 && aFNum > 0) // ���ݷǷ�ʱ��ִ��
    {
        if (videoPacketQueue) // ��Ƶ������
            videoPacketQueue = new FFmpegSafeQueue<AVPacket>(vPNum);
        else
            videoPacketQueue->setCapacity(vPNum);

        if (audioPacketQueue) // ��Ƶ������
            audioPacketQueue = new FFmpegSafeQueue<AVPacket>(aPNum);
        else
            audioPacketQueue->setCapacity(aPNum);

        if (videoFrameQueue) // ��Ƶ֡����
            videoFrameQueue = new FFmpegSafeQueue<AVCodec>(vFNum);
        else
            videoFrameQueue->setCapacity(vFNum);

        if (audioFrameQueue) // ��Ƶ֡����
            audioFrameQueue = new FFmpegSafeQueue<AVCodec>(aFNum);
        else
            audioFrameQueue->setCapacity(aFNum);
    }
    else
    {
        return false;
    }
}

// FFmpeg���е�get��������ֵ������
FFmpegSafeQueue<AVPacket>& DataSingleton::getVideoPacketQueue() { return *videoPacketQueue; }
FFmpegSafeQueue<AVPacket>& DataSingleton::getAudioPacketQueue() { return *audioPacketQueue; }
FFmpegSafeQueue<AVCodec>& DataSingleton::getVideoFrameQueue() { return *videoFrameQueue; }
FFmpegSafeQueue<AVCodec>& DataSingleton::getAudioFrameQueue() { return *audioFrameQueue; }