#include "demux_thread.h"

DemuxThread::DemuxThread()
	: dataSingleton(DataSingleton::getInstance())
{
	videoPacketQueue = dataSingleton.getVideoPacketQueue();
	audioPacketQueue = dataSingleton.getAudioPacketQueue();
}

DemuxThread::~DemuxThread()
{
}

void DemuxThread::run()
{
	while (isRunning())
	{
		packet = av_packet_alloc();

		if (av_read_frame(formatCtx, packet) < 0)
			return;

		std::cout << "sss" << std::endl;

		if (packet->stream_index == videoIndex)
		{
			videoPacketQueue->push(packet);

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