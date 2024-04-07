#pragma once

#include <mutex>
#include <atomic>
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
	FFmpegSafeQueue<AVCodec*>* getVideoFrameQueue();
	FFmpegSafeQueue<AVCodec*>* getAudioFrameQueue();

	// 音视频格式的上下文结构体指针
	void setFormatCtx(AVFormatContext* formatCtx);
	AVFormatContext* getFormatCtx();
	// 视频索引
	void setVideoIndex(unsigned videoIndex);
	unsigned getVideoIndex();
	// 音频索引
	void setAudioIndex(unsigned audioIndex);
	unsigned getAudioIndex();

private:
	// 包队列
	FFmpegSafeQueue<AVPacket*>* videoPacketQueue = nullptr;
	FFmpegSafeQueue<AVPacket*>* audioPacketQueue = nullptr;
	// 帧队列
	FFmpegSafeQueue<AVCodec*>* videoFrameQueue = nullptr;
	FFmpegSafeQueue<AVCodec*>* audioFrameQueue = nullptr;

	// 音视频格式的上下文结构体指针
	AVFormatContext* formatCtx = nullptr;
	// 视频和音频的索引
	std::atomic<unsigned> videoIndex, audioIndex;


private:
	// 直接私有不让用
	DataSingleton() {};

	static DataSingleton* instance;
	static std::mutex mutex;
};
