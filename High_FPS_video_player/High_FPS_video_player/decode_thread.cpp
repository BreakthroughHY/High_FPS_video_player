#include "decode_thread.h"

DecodeThread::DecodeThread(Types types)
	: dataSingleton(DataSingleton::getInstance())
{
	this->types = types;

	if (types == Types::VIDEO)
	{
		packetQueue = dataSingleton.getVideoPacketQueue();
		frameQueue = dataSingleton.getVideoFrameQueue();
	}
	else if (types == Types::AUDIO)
	{
		packetQueue = dataSingleton.getAudioPacketQueue();
		frameQueue = dataSingleton.getAudioFrameQueue();
	}

	frame = av_frame_alloc();
}

DecodeThread::~DecodeThread()
{}

// ÉèÖÃÊôÐÔ
void DecodeThread::setParameters()
{
	if (types == Types::VIDEO)
	{
		codecCtx = dataSingleton.getVCodecCtx();
	}
	else if (types == Types::AUDIO)
	{
		codecCtx = dataSingleton.getACodecCtx();
	}
}

void DecodeThread::run()
{
	while (isRunning())
	{
		packetQueue->waitAndPop(packet);

		avcodec_send_packet(codecCtx, packet);

		av_packet_free(&packet);

		while (true)
		{
			int ret = avcodec_receive_frame(codecCtx, frame);
			if (ret == 0)
			{
				frameQueue->push(frame);
				frame = av_frame_alloc();
			}
			else
			{
				break;
			}
		}
	}
}