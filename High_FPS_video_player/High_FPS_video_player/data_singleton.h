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
	FFmpegSafeQueue<AVFrame*>* getVideoFrameQueue();
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

private:
	// 包队列
	FFmpegSafeQueue<AVPacket*>* videoPacketQueue = nullptr;
	FFmpegSafeQueue<AVPacket*>* audioPacketQueue = nullptr;
	// 帧队列
	FFmpegSafeQueue<AVFrame*>* videoFrameQueue = nullptr;
	FFmpegSafeQueue<AVFrame*>* audioFrameQueue = nullptr;

	// 音视频格式的上下文结构体指针
	AVFormatContext* formatCtx = nullptr;
	// 视频和音频的索引
	std::atomic<unsigned> videoIndex, audioIndex;
	// 解码器
	AVCodecContext* vCodecCtx = nullptr, * aCodecCtx = nullptr;


private:
	// 直接私有不让用
	DataSingleton() {};

	static DataSingleton* instance;
	static std::mutex mutex;
};
