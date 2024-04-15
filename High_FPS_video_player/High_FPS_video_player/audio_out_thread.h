#pragma once

#include "thread_base.h"
#include "data_singleton.h"
#include <SDL.h>

extern "C"
{

#include <libavcodec\avcodec.h>
#include <libavutil/opt.h>
#include <libswresample/swresample.h>
}

class AudioOutThread : public ThreadBase
{
public:
	AudioOutThread();
	~AudioOutThread();

	// ��������
	void setParameters();

	// SDL2 ��Ƶ�ص�����
	static void audio_callback(void* udata, Uint8* stream, int len);
	// ��������
	static void setVolume(int volume_);

protected:
	void run() override; // ���߱�������д�����麯��


private:


private:
	// ȫ��Ψһ�����ݶ��� ֧�ֶ��̷߳���
	DataSingleton& dataSingleton;
	// ������
	AVCodecContext* codecCtx = nullptr;
	// want_spec�е�����Ҫ���õĲ�����have_spec�Ƿ��ص�ʵ�ʲ���
	SDL_AudioSpec want_spec, have_spec;
	// ��Ƶ�ز���������
	SwrContext* au_convert_ctx = nullptr;
	// �����ڴ������ز��������Ƶ����
	int converted_audio_size = -1;
	uint8_t* converted_audio_data = nullptr;
	// ֡����
	FFmpegSafeQueue<AVFrame*>* frameQueue = nullptr;
	// ֡
	AVFrame* frame = nullptr;
	// ��Ƶʱ���
	AVRational aTimeBase;
	// ���Բ��ŵ�pts����
	double beforePTS = 0, currPTS = 0;


	static unsigned int audioLen;
	static unsigned char* audioChunk;
	static unsigned char* audioPos;
	// ����
	static int volume;
};