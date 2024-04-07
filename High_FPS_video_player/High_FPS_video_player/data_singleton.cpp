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
        if (!videoPacketQueue) // ��Ƶ������
            videoPacketQueue = new FFmpegSafeQueue<AVPacket*>(aPNum);
        else
            videoPacketQueue->setCapacity(vPNum);

        if (!audioPacketQueue) // ��Ƶ������
            audioPacketQueue = new FFmpegSafeQueue<AVPacket*>(aPNum);
        else
            audioPacketQueue->setCapacity(aPNum);

        if (!videoFrameQueue) // ��Ƶ֡����
            videoFrameQueue = new FFmpegSafeQueue<AVCodec*>(vFNum);
        else
            videoFrameQueue->setCapacity(vFNum);

        if (!audioFrameQueue) // ��Ƶ֡����
            audioFrameQueue = new FFmpegSafeQueue<AVCodec*>(aFNum);
        else
            audioFrameQueue->setCapacity(aFNum);
    }
    else
    {
        return false;
    }
}

// FFmpeg���е�get��������ֵ������
FFmpegSafeQueue<AVPacket*>* DataSingleton::getVideoPacketQueue() { return videoPacketQueue; }
FFmpegSafeQueue<AVPacket*>* DataSingleton::getAudioPacketQueue() { return audioPacketQueue; }
FFmpegSafeQueue<AVCodec*>* DataSingleton::getVideoFrameQueue() { return videoFrameQueue; }
FFmpegSafeQueue<AVCodec*>* DataSingleton::getAudioFrameQueue() { return audioFrameQueue; }


// ����Ƶ��ʽ�������Ľṹ��ָ��
void DataSingleton::setFormatCtx(AVFormatContext* formatCtx)
{
    std::lock_guard<std::mutex> lock(mutex);
    this->formatCtx = formatCtx;
}
AVFormatContext* DataSingleton::getFormatCtx()
{ 
    std::lock_guard<std::mutex> lock(mutex); 
    return this->formatCtx; 
}

// ��Ƶ����
void DataSingleton::setVideoIndex(unsigned videoIndex)
{
    this->videoIndex = videoIndex;
}
unsigned DataSingleton::getVideoIndex()
{
    return this->videoIndex;
}

// ��Ƶ����
void DataSingleton::setAudioIndex(unsigned audioIndex)
{
    this->audioIndex = audioIndex;
}
unsigned DataSingleton::getAudioIndex()
{
    return this->audioIndex;
}
