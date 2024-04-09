#include "demux_thread.h"

DemuxThread::DemuxThread()
	: dataSingleton(DataSingleton::getInstance())
{
	videoPacketQueue = dataSingleton.getVideoPacketQueue();
	audioPacketQueue = dataSingleton.getAudioPacketQueue();

	packet = av_packet_alloc();
}

DemuxThread::~DemuxThread()
{
}

void DemuxThread::run()
{
	while (isRunning())
	{
		if (av_read_frame(formatCtx, packet) < 0)
		{
			av_packet_free(&packet); // 被用过的包不能用，会内存泄露
			packet = av_packet_alloc();
			continue;
		}

		if (packet->stream_index == videoIndex)
		{
			videoPacketQueue->push(packet);
			packet = av_packet_alloc();
		}
		else if (packet->stream_index == audioIndex)
		{
			audioPacketQueue->push(packet);
			packet = av_packet_alloc();
		}
		else
		{
			av_packet_free(&packet); // 被用过的包不能用，会内存泄露
			packet = av_packet_alloc();
		}
	}
}

// 设置解复用的参数
void DemuxThread::setParameters()
{
	formatCtx = dataSingleton.getFormatCtx();

	videoIndex = dataSingleton.getVideoIndex();
	audioIndex = dataSingleton.getAudioIndex();
}