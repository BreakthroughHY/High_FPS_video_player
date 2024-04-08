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
			return;

		if (packet->stream_index == videoIndex)
		{
			videoPacketQueue->push(packet);
			packet = av_packet_alloc();

			AVFrame* frame = av_frame_alloc();
			avcodec_send_packet(dataSingleton.getVCodecCtx(), packet);
			std::cout << avcodec_receive_frame(dataSingleton.getVCodecCtx(), frame) << std::endl;
		}
		else if (packet->stream_index == audioIndex)
		{
			audioPacketQueue->push(packet);
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