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
			av_packet_free(&packet); // ���ù��İ������ã����ڴ�й¶
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
			av_packet_free(&packet); // ���ù��İ������ã����ڴ�й¶
			packet = av_packet_alloc();
		}
	}
}

// ���ý⸴�õĲ���
void DemuxThread::setParameters()
{
	formatCtx = dataSingleton.getFormatCtx();

	videoIndex = dataSingleton.getVideoIndex();
	audioIndex = dataSingleton.getAudioIndex();
}