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

	// 设置属性
	void setParameters();

	// SDL2 音频回调函数
	static void audio_callback(void* udata, Uint8* stream, int len);
	// 设置音量
	static void setVolume(int volume_);

protected:
	void run() override; // 告诉编译器重写父类虚函数


private:


private:
	// 全局唯一的数据对象 支持多线程访问
	DataSingleton& dataSingleton;
	// 解码器
	AVCodecContext* codecCtx = nullptr;
	// want_spec中的是想要设置的参数，have_spec是返回的实际参数
	SDL_AudioSpec want_spec, have_spec;
	// 音频重采样上下文
	SwrContext* au_convert_ctx = nullptr;
	// 分配内存用于重采样后的音频数据
	int converted_audio_size = -1;
	uint8_t* converted_audio_data = nullptr;
	// 帧队列
	FFmpegSafeQueue<AVFrame*>* frameQueue = nullptr;
	// 帧
	AVFrame* frame = nullptr;
	// 音频时间基
	AVRational aTimeBase;
	// 可以播放的pts区间
	double beforePTS = 0, currPTS = 0;


	static unsigned int audioLen;
	static unsigned char* audioChunk;
	static unsigned char* audioPos;
	// 音量
	static int volume;
};