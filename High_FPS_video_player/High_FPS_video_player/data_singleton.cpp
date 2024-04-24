#include "data_singleton.h"

DataSingleton* DataSingleton::instance = nullptr;
std::once_flag DataSingleton::once;
std::mutex DataSingleton::mutex;
std::mutex DataSingleton::mutexPTS;

// ��ȡDataSingletonΨһ����ķ���
DataSingleton& DataSingleton::getInstance()
{
    /*if (instance == nullptr)
    {
        std::lock_guard<std::mutex> lock(mutex);
        if (instance == nullptr)
        {
            instance = new DataSingleton();
        }
    }*/
    std::call_once(once, &DataSingleton::init);

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
            videoFrameQueue = new FFmpegSafeQueue<Myframe*>(vFNum);
        else
            videoFrameQueue->setCapacity(vFNum);

        if (!audioFrameQueue) // ��Ƶ֡����
            audioFrameQueue = new FFmpegSafeQueue<AVFrame*>(aFNum);
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
FFmpegSafeQueue<Myframe*>* DataSingleton::getVideoFrameQueue() { return videoFrameQueue; }
FFmpegSafeQueue<AVFrame*>* DataSingleton::getAudioFrameQueue() { return audioFrameQueue; }


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

// ��Ƶ������
void DataSingleton::setVCodecCtx(AVCodecContext* vCodecCtx)
{
    this->vCodecCtx = vCodecCtx;
}
AVCodecContext* DataSingleton::getVCodecCtx()
{
    return this->vCodecCtx;
}

// ��Ƶ������
void DataSingleton::setACodecCtx(AVCodecContext* aCodecCtx)
{
    this->aCodecCtx = aCodecCtx;
}
AVCodecContext* DataSingleton::getACodecCtx()
{
    return this->aCodecCtx;
}

// ��Ƶʱ���
void DataSingleton::setvTimeBase(AVRational vTimeBase)
{
    this->vTimeBase.num = vTimeBase.num;
    this->vTimeBase.den = vTimeBase.den;
}
AVRational DataSingleton::getvTimeBase()
{
    return this->vTimeBase;
}

// ��Ƶʱ���
void DataSingleton::setaTimeBase(AVRational aTimeBase)
{
    this->aTimeBase.num = aTimeBase.num;
    this->aTimeBase.den = aTimeBase.den;
}
AVRational DataSingleton::getaTimeBase()
{
    return this->aTimeBase;
}

// ���Բ��ŵ�pts����
void DataSingleton::setPTS(double beforePTS, double currPTS)
{
    std::lock_guard<std::mutex> lock(mutexPTS);
    this->beforePTS = beforePTS;
    this->currPTS = currPTS;
}
void DataSingleton::getPTS(double& beforePTS, double& currPTS)
{
    std::lock_guard<std::mutex> lock(mutexPTS);
    beforePTS = this->beforePTS;
    currPTS = this->currPTS;
}

// openGL���ڱ���
void DataSingleton::setWdividedH(double WdividedH)
{
    this->WdividedH = WdividedH;
}
double DataSingleton::getWdividedH()
{
    return this->WdividedH;
}

// ��Ƶ֡��
void DataSingleton::setFPSV(double FPSV)
{
    this->FPSV = FPSV;
}
double DataSingleton::getFPSV()
{
    return this->FPSV;
}