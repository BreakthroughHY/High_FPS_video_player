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
	int waiting = false; // �Ƿ�����Ҫ��δ������İ�
	
	while (isRunning())
	{
		if (waiting)
		{
			if (waiting > 0)
			{
				if (!videoPacketQueue->tryPush(packet))
					continue;
				else
				{
					waiting = false;
					packet = av_packet_alloc();
				}
			}
			else
			{
				if (!audioPacketQueue->tryPush(packet))
					continue;
				else
				{
					waiting = false;
					packet = av_packet_alloc();
				}
			}
		}

		if (av_read_frame(formatCtx, packet) < 0)
		{
			av_packet_free(&packet); // ���ù��İ������ã����ڴ�й¶
			packet = av_packet_alloc();
			continue;
		}

		if (packet->stream_index == videoIndex)
		{
			//videoPacketQueue->push(packet);
			if (!videoPacketQueue->tryPush(packet))
			{
				waiting = 1;
				continue;
			}
			packet = av_packet_alloc();
		}
		else if (packet->stream_index == audioIndex)
		{
			//audioPacketQueue->push(packet);
			if (!audioPacketQueue->tryPush(packet))
			{
				waiting = -1;
				continue;
			}
			packet = av_packet_alloc();
		}
		else
		{
			av_packet_free(&packet); // ���ù��İ������ã����ڴ�й¶
			packet = av_packet_alloc();
		}
	}
	if (waiting)
	{
		waiting = false;
		av_packet_free(&packet);
		packet = av_packet_alloc();
	}
}

// ���ý⸴�õĲ���
void DemuxThread::setParameters()
{
	formatCtx = dataSingleton.getFormatCtx();

	videoIndex = dataSingleton.getVideoIndex();
	audioIndex = dataSingleton.getAudioIndex();
}