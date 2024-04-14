#include "decode_thread.h"

DecodeThread::DecodeThread(Types types)
	: dataSingleton(DataSingleton::getInstance())
{
	this->types = types;

	if (types == Types::VIDEO)
	{
		packetQueue = dataSingleton.getVideoPacketQueue();
		vframeQueue = dataSingleton.getVideoFrameQueue();
	}
	else if (types == Types::AUDIO)
	{
		packetQueue = dataSingleton.getAudioPacketQueue();
		aframeQueue = dataSingleton.getAudioFrameQueue();
	}

	frame = av_frame_alloc();
}

DecodeThread::~DecodeThread()
{}

// ��������
void DecodeThread::setParameters()
{
	if (types == Types::VIDEO) // ������Ƶ����
	{
		// ��ȡ��Ƶ������
		codecCtx = dataSingleton.getVCodecCtx();
		// ��ȡ������Ƶÿһ��֡��Ҫ�Ŀռ��С
		outBufferSize = av_image_get_buffer_size(AV_PIX_FMT_RGB24, codecCtx->width, codecCtx->height, 1);
		// ������Ƶת��ṹ��
		if (swsContext) // ��Ϊ��ָ���������д����
		{
			sws_freeContext(swsContext);
			swsContext = NULL;
		}
		swsContext = sws_getCachedContext(swsContext, codecCtx->width, codecCtx->height, codecCtx->pix_fmt, codecCtx->width, codecCtx->height, AV_PIX_FMT_RGB24, SWS_FAST_BILINEAR, NULL, NULL, NULL);
	}
	else if (types == Types::AUDIO) // ������Ƶ����
	{
		codecCtx = dataSingleton.getACodecCtx();
	}
}

void DecodeThread::run()
{
	while (isRunning())
	{
		//packetQueue->waitAndPop(packet);
		if (!packetQueue->tryPop(packet))
			continue;

		avcodec_send_packet(codecCtx, packet);

		av_packet_free(&packet);

		while (isRunning())
		{
			if (myFrame)
			{
				if (!vframeQueue->tryPush(myFrame))
				{
					continue;
				}
				else
				{
					myFrame = nullptr;
					av_frame_free(&frame);
					frame = av_frame_alloc();
				}
			}

			int ret = avcodec_receive_frame(codecCtx, frame);
			if (ret == 0)
			{
				switch (types)
				{
					// ������Ƶ
				case Types::VIDEO:

					myFrame = new Myframe;
					outBuffer = (uint8_t*)av_malloc(outBufferSize);
					av_image_fill_arrays(myFrame->data, myFrame->linesize, outBuffer, AV_PIX_FMT_RGB24, codecCtx->width, codecCtx->height, 1);

					sws_scale(swsContext, frame->data, frame->linesize, 0, frame->height, myFrame->data, myFrame->linesize);

					myFrame->outBuffer = outBuffer;
					myFrame->pts = frame->pts;

					//vframeQueue->push(myFrame);
					if (!vframeQueue->tryPush(myFrame))
					{
						continue;
					}
					else
					{
						myFrame = nullptr;
					}

					av_frame_free(&frame);
					frame = av_frame_alloc();

					break;
					// ������Ƶ
				case Types::AUDIO:
					aframeQueue->push(frame);
					frame = av_frame_alloc();
					break;
				}
			}
			else
			{
				break;
			}
		}
	}
	if (myFrame)
	{
		av_freep(&myFrame->outBuffer);
		delete myFrame;
		myFrame = nullptr;
	}
}