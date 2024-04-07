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
	// ����
	DataSingleton(const DataSingleton&) = delete;
	void operator=(const DataSingleton&) = delete;

	// ����FFmpeg��Ҫ�Ķ���
	bool setFFmpegQueue(int vPNum, int aPNum, int vFNum, int aFNum);
	FFmpegSafeQueue<AVPacket*>* getVideoPacketQueue();
	FFmpegSafeQueue<AVPacket*>* getAudioPacketQueue();
	FFmpegSafeQueue<AVCodec*>* getVideoFrameQueue();
	FFmpegSafeQueue<AVCodec*>* getAudioFrameQueue();

	// ����Ƶ��ʽ�������Ľṹ��ָ��
	void setFormatCtx(AVFormatContext* formatCtx);
	AVFormatContext* getFormatCtx();
	// ��Ƶ����
	void setVideoIndex(unsigned videoIndex);
	unsigned getVideoIndex();
	// ��Ƶ����
	void setAudioIndex(unsigned audioIndex);
	unsigned getAudioIndex();

private:
	// ������
	FFmpegSafeQueue<AVPacket*>* videoPacketQueue = nullptr;
	FFmpegSafeQueue<AVPacket*>* audioPacketQueue = nullptr;
	// ֡����
	FFmpegSafeQueue<AVCodec*>* videoFrameQueue = nullptr;
	FFmpegSafeQueue<AVCodec*>* audioFrameQueue = nullptr;

	// ����Ƶ��ʽ�������Ľṹ��ָ��
	AVFormatContext* formatCtx = nullptr;
	// ��Ƶ����Ƶ������
	std::atomic<unsigned> videoIndex, audioIndex;


private:
	// ֱ��˽�в�����
	DataSingleton() {};

	static DataSingleton* instance;
	static std::mutex mutex;
};
