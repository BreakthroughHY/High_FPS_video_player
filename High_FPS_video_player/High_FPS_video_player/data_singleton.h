#pragma once

#include <mutex>
#include <libavcodec\packet.h>
#include <libavcodec\avcodec.h>
#include "FFmpeg_safe_queue.h"

class DataSingleton
{
public:
	static DataSingleton& getInstance();
	// ����
	DataSingleton(const DataSingleton&) = delete;
	void operator=(const DataSingleton&) = delete;

	// ����FFmpeg��Ҫ�Ķ���
	bool setFFmpegQueue(int vPNum, int aPNum, int vFNum, int aFNum);
	FFmpegSafeQueue<AVPacket>& getVideoPacketQueue();
	FFmpegSafeQueue<AVPacket>& getAudioPacketQueue();
	FFmpegSafeQueue<AVCodec>& getVideoFrameQueue();
	FFmpegSafeQueue<AVCodec>& getAudioFrameQueue();


private:
	// ������
	FFmpegSafeQueue<AVPacket>* videoPacketQueue = nullptr;
	FFmpegSafeQueue<AVPacket>* audioPacketQueue = nullptr;
	// ֡����
	FFmpegSafeQueue<AVCodec>* videoFrameQueue = nullptr;
	FFmpegSafeQueue<AVCodec>* audioFrameQueue = nullptr;



private:
	// ֱ��˽�в�����
	DataSingleton() {};

	static DataSingleton* instance;
	static std::mutex mutex;
};
