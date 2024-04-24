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
	FFmpegSafeQueue<Myframe*>* getVideoFrameQueue();
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

	// ��Ƶʱ���
	void setvTimeBase(AVRational vTimeBase);
	AVRational getvTimeBase();

	// ��Ƶʱ���
	void setaTimeBase(AVRational aTimeBase);
	AVRational getaTimeBase();

	// ���Բ��ŵ�pts����
	void setPTS(double beforePTS, double currPTS);
	void getPTS(double& beforePTS, double& currPTS);

	// openGL���ڱ���
	void setWdividedH(double WdividedH);
	double getWdividedH();

	// ��Ƶ֡��
	void setFPSV(double FPSV);
	double getFPSV();

private:
	// ������
	FFmpegSafeQueue<AVPacket*>* videoPacketQueue = nullptr;
	FFmpegSafeQueue<AVPacket*>* audioPacketQueue = nullptr;
	// ֡����
	FFmpegSafeQueue<Myframe*>* videoFrameQueue = nullptr;
	FFmpegSafeQueue<AVFrame*>* audioFrameQueue = nullptr;

	// ����Ƶ��ʽ�������Ľṹ��ָ��
	AVFormatContext* formatCtx = nullptr;
	// ��Ƶ����Ƶ������
	std::atomic<unsigned> videoIndex, audioIndex;
	// ������
	AVCodecContext* vCodecCtx = nullptr, * aCodecCtx = nullptr;

	// ����Ƶʱ���
	AVRational vTimeBase;
	AVRational aTimeBase;

	// ���Բ��ŵ�pts����
	double beforePTS = 0, currPTS = 0;
	// openGL���ڱ���
	double WdividedH = 1.777;
	// ��Ƶ֡��
	double FPSV;
private:
	// ֱ��˽�в�����
	DataSingleton() {};
	// ��������
	static void init()
	{
		instance = new DataSingleton();
	}

	static DataSingleton* instance;
	static std::once_flag once;
	static std::mutex mutex;
	static std::mutex mutexPTS;
};
