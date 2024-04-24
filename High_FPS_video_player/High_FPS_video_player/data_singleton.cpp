#include "data_singleton.h"

DataSingleton* DataSingleton::instance = nullptr;
std::once_flag DataSingleton::once;
std::mutex DataSingleton::mutex;
std::mutex DataSingleton::mutexPTS;

// 获取DataSingleton唯一对象的方法
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

// 设置FFmpeg需要的队列  （重置大小时内容将被清空）
bool DataSingleton::setFFmpegQueue(int vPNum, int aPNum, int vFNum, int aFNum)
{
    if (vPNum > 0 && aPNum > 0 && vFNum > 0 && aFNum > 0) // 数据非法时不执行
    {
        if (!videoPacketQueue) // 视频包队列
            videoPacketQueue = new FFmpegSafeQueue<AVPacket*>(aPNum);
        else
            videoPacketQueue->setCapacity(vPNum);

        if (!audioPacketQueue) // 音频包队列
            audioPacketQueue = new FFmpegSafeQueue<AVPacket*>(aPNum);
        else
            audioPacketQueue->setCapacity(aPNum);

        if (!videoFrameQueue) // 视频帧队列
            videoFrameQueue = new FFmpegSafeQueue<Myframe*>(vFNum);
        else
            videoFrameQueue->setCapacity(vFNum);

        if (!audioFrameQueue) // 音频帧队列
            audioFrameQueue = new FFmpegSafeQueue<AVFrame*>(aFNum);
        else
            audioFrameQueue->setCapacity(aFNum);
    }
    else
    {
        return false;
    }
}

// FFmpeg队列的get函数返回值是引用
FFmpegSafeQueue<AVPacket*>* DataSingleton::getVideoPacketQueue() { return videoPacketQueue; }
FFmpegSafeQueue<AVPacket*>* DataSingleton::getAudioPacketQueue() { return audioPacketQueue; }
FFmpegSafeQueue<Myframe*>* DataSingleton::getVideoFrameQueue() { return videoFrameQueue; }
FFmpegSafeQueue<AVFrame*>* DataSingleton::getAudioFrameQueue() { return audioFrameQueue; }


// 音视频格式的上下文结构体指针
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

// 视频索引
void DataSingleton::setVideoIndex(unsigned videoIndex)
{
    this->videoIndex = videoIndex;
}
unsigned DataSingleton::getVideoIndex()
{
    return this->videoIndex;
}

// 音频索引
void DataSingleton::setAudioIndex(unsigned audioIndex)
{
    this->audioIndex = audioIndex;
}
unsigned DataSingleton::getAudioIndex()
{
    return this->audioIndex;
}

// 视频解码器
void DataSingleton::setVCodecCtx(AVCodecContext* vCodecCtx)
{
    this->vCodecCtx = vCodecCtx;
}
AVCodecContext* DataSingleton::getVCodecCtx()
{
    return this->vCodecCtx;
}

// 音频解码器
void DataSingleton::setACodecCtx(AVCodecContext* aCodecCtx)
{
    this->aCodecCtx = aCodecCtx;
}
AVCodecContext* DataSingleton::getACodecCtx()
{
    return this->aCodecCtx;
}

// 视频时间基
void DataSingleton::setvTimeBase(AVRational vTimeBase)
{
    this->vTimeBase.num = vTimeBase.num;
    this->vTimeBase.den = vTimeBase.den;
}
AVRational DataSingleton::getvTimeBase()
{
    return this->vTimeBase;
}

// 音频时间基
void DataSingleton::setaTimeBase(AVRational aTimeBase)
{
    this->aTimeBase.num = aTimeBase.num;
    this->aTimeBase.den = aTimeBase.den;
}
AVRational DataSingleton::getaTimeBase()
{
    return this->aTimeBase;
}

// 可以播放的pts区间
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

// openGL窗口比例
void DataSingleton::setWdividedH(double WdividedH)
{
    this->WdividedH = WdividedH;
}
double DataSingleton::getWdividedH()
{
    return this->WdividedH;
}

// 视频帧率
void DataSingleton::setFPSV(double FPSV)
{
    this->FPSV = FPSV;
}
double DataSingleton::getFPSV()
{
    return this->FPSV;
}