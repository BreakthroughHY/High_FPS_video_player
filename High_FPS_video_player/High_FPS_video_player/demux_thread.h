#pragma once

#include "thread_base.h"
#include "data_singleton.h"
#include "FFmpeg_safe_queue.h"

//extern "C"
//{
//#include <libavformat\avformat.h>
//#include <libavcodec\avcodec.h>
//}

class DemuxThread : public ThreadBase
{
public:
	DemuxThread();
	~DemuxThread();
	// 设置属性
	void setParameters();

protected:
	void run() override; // 告诉编译器重写父类虚函数

private:

private:
	// 全局唯一的数据对象 支持多线程访问
	DataSingleton& dataSingleton;
	// 包队列
	FFmpegSafeQueue<AVPacket*>* videoPacketQueue = nullptr;
	FFmpegSafeQueue<AVPacket*>* audioPacketQueue = nullptr;

	// 音视频格式的上下文结构体指针
	AVFormatContext* formatCtx = nullptr;
	// 包指针
	AVPacket* packet = nullptr;
	// 视频和音频的索引
	unsigned videoIndex, audioIndex;

};

