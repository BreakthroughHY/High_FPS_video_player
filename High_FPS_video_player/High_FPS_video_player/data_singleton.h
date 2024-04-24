#pragma once

#include "FFmpeg_safe_queue.h"

extern "C"
{
#include <libavformat\avformat.h>
}

class DataSingleton
{
public:
	static DataSingleton& getInstance();
	// 禁用
	DataSingleton(const DataSingleton&) = delete;
	void operator=(const DataSingleton&) = delete;

	// 设置FFmpeg需要的队列
	bool setFFmpegQueue(int vPNum, int aPNum, int vFNum, int aFNum);
	FFmpegSafeQueue<AVPacket*>* getVideoPacketQueue();
	FFmpegSafeQueue<AVPacket*>* getAudioPacketQueue();
	FFmpegSafeQueue<Myframe*>* getVideoFrameQueue();
	FFmpegSafeQueue<AVFrame*>* getAudioFrameQueue();

	// 音视频格式的上下文结构体指针
	void setFormatCtx(AVFormatContext* formatCtx);
	AVFormatContext* getFormatCtx();
	// 视频索引
	void setVideoIndex(unsigned videoIndex);
	unsigned getVideoIndex();
	// 音频索引
	void setAudioIndex(unsigned audioIndex);
	unsigned getAudioIndex();
	// 视频解码器
	void setVCodecCtx(AVCodecContext* vCodecCtx);
	AVCodecContext* getVCodecCtx();
	// 音频解码器
	void setACodecCtx(AVCodecContext* aCodecCtx);
	AVCodecContext* getACodecCtx();

	// 视频时间基
	void setvTimeBase(AVRational vTimeBase);
	AVRational getvTimeBase();

	// 音频时间基
	void setaTimeBase(AVRational aTimeBase);
	AVRational getaTimeBase();

	// 可以播放的pts区间
	void setPTS(double beforePTS, double currPTS);
	void getPTS(double& beforePTS, double& currPTS);

	// openGL窗口比例
	void setWdividedH(double WdividedH);
	double getWdividedH();

	// 视频帧率
	void setFPSV(double FPSV);
	double getFPSV();

private:
	// 包队列
	FFmpegSafeQueue<AVPacket*>* videoPacketQueue = nullptr;
	FFmpegSafeQueue<AVPacket*>* audioPacketQueue = nullptr;
	// 帧队列
	FFmpegSafeQueue<Myframe*>* videoFrameQueue = nullptr;
	FFmpegSafeQueue<AVFrame*>* audioFrameQueue = nullptr;

	// 音视频格式的上下文结构体指针
	AVFormatContext* formatCtx = nullptr;
	// 视频和音频的索引
	std::atomic<unsigned> videoIndex, audioIndex;
	// 解码器
	AVCodecContext* vCodecCtx = nullptr, * aCodecCtx = nullptr;

	// 音视频时间基
	AVRational vTimeBase;
	AVRational aTimeBase;

	// 可以播放的pts区间
	double beforePTS = 0, currPTS = 0;
	// openGL窗口比例
	double WdividedH = 1.777;
	// 视频帧率
	double FPSV;
private:
	// 直接私有不让用
	DataSingleton() {};
	// 创建单例
	static void init()
	{
		instance = new DataSingleton();
	}

	static DataSingleton* instance;
	static std::once_flag once;
	static std::mutex mutex;
	static std::mutex mutexPTS;
};
