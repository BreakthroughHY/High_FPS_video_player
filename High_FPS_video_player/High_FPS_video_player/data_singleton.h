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
	// ����
	DataSingleton(const DataSingleton&) = delete;
	void operator=(const DataSingleton&) = delete;

	// ����FFmpeg��Ҫ�Ķ���
	bool setFFmpegQueue(int vPNum, int aPNum, int vFNum, int aFNum);
	FFmpegSafeQueue<AVPacket*>* getVideoPacketQueue();
	FFmpegSafeQueue<AVPacket*>* getAudioPacketQueue();
	FFmpegSafeQueue<AVFrame*>* getVideoFrameQueue();
	FFmpegSafeQueue<AVFrame*>* getAudioFrameQueue();

	// ����Ƶ��ʽ�������Ľṹ��ָ��
	void setFormatCtx(AVFormatContext* formatCtx);
	AVFormatContext* getFormatCtx();
	// ��Ƶ����
	void setVideoIndex(unsigned videoIndex);
	unsigned getVideoIndex();
	// ��Ƶ����
	void setAudioIndex(unsigned audioIndex);
	unsigned getAudioIndex();
	// ��Ƶ������
	void setVCodecCtx(AVCodecContext* vCodecCtx);
	AVCodecContext* getVCodecCtx();
	// ��Ƶ������
	void setACodecCtx(AVCodecContext* aCodecCtx);
	AVCodecContext* getACodecCtx();

private:
	// ������
	FFmpegSafeQueue<AVPacket*>* videoPacketQueue = nullptr;
	FFmpegSafeQueue<AVPacket*>* audioPacketQueue = nullptr;
	// ֡����
	FFmpegSafeQueue<AVFrame*>* videoFrameQueue = nullptr;
	FFmpegSafeQueue<AVFrame*>* audioFrameQueue = nullptr;

	// ����Ƶ��ʽ�������Ľṹ��ָ��
	AVFormatContext* formatCtx = nullptr;
	// ��Ƶ����Ƶ������
	std::atomic<unsigned> videoIndex, audioIndex;
	// ������
	AVCodecContext* vCodecCtx = nullptr, * aCodecCtx = nullptr;


private:
	// ֱ��˽�в�����
	DataSingleton() {};

	static DataSingleton* instance;
	static std::mutex mutex;
};
