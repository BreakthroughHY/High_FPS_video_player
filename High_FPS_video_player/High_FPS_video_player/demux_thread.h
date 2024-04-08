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
	// ��������
	void setParameters();

protected:
	void run() override; // ���߱�������д�����麯��

private:

private:
	// ȫ��Ψһ�����ݶ��� ֧�ֶ��̷߳���
	DataSingleton& dataSingleton;
	// ������
	FFmpegSafeQueue<AVPacket*>* videoPacketQueue = nullptr;
	FFmpegSafeQueue<AVPacket*>* audioPacketQueue = nullptr;

	// ����Ƶ��ʽ�������Ľṹ��ָ��
	AVFormatContext* formatCtx = nullptr;
	// ��ָ��
	AVPacket* packet = nullptr;
	// ��Ƶ����Ƶ������
	unsigned videoIndex, audioIndex;

};

