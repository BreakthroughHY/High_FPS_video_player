#pragma once

#include <mutex>
#include <libavcodec\packet.h>
#include <libavcodec\avcodec.h>
#include "FFmpeg_safe_queue.h"

class DataSingleton
{
public:
	static DataSingleton& getInstance();
	// 禁用
	DataSingleton(const DataSingleton&) = delete;
	void operator=(const DataSingleton&) = delete;

	// 设置FFmpeg需要的队列
	bool setFFmpegQueue(int vPNum, int aPNum, int vFNum, int aFNum);
	FFmpegSafeQueue<AVPacket>& getVideoPacketQueue();
	FFmpegSafeQueue<AVPacket>& getAudioPacketQueue();
	FFmpegSafeQueue<AVCodec>& getVideoFrameQueue();
	FFmpegSafeQueue<AVCodec>& getAudioFrameQueue();


private:
	// 包队列
	FFmpegSafeQueue<AVPacket>* videoPacketQueue = nullptr;
	FFmpegSafeQueue<AVPacket>* audioPacketQueue = nullptr;
	// 帧队列
	FFmpegSafeQueue<AVCodec>* videoFrameQueue = nullptr;
	FFmpegSafeQueue<AVCodec>* audioFrameQueue = nullptr;



private:
	// 直接私有不让用
	DataSingleton() {};

	static DataSingleton* instance;
	static std::mutex mutex;
};
