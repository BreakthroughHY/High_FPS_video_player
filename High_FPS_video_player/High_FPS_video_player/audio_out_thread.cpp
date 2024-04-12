#include "audio_out_thread.h"

unsigned int AudioOutThread::audioLen = 0;
unsigned char* AudioOutThread::audioChunk = NULL;
unsigned char* AudioOutThread::audioPos = NULL;
int AudioOutThread::volume = 64;

AudioOutThread::AudioOutThread()
    : dataSingleton(DataSingleton::getInstance())
{
    // ��ʼ�� SDL2 ��Ƶ��ϵͳ
    SDL_Init(SDL_INIT_AUDIO);

    frameQueue = dataSingleton.getAudioFrameQueue();
}
AudioOutThread::~AudioOutThread()
{
    SDL_Quit();
}

// ��������
void AudioOutThread::setParameters()
{
    codecCtx = dataSingleton.getACodecCtx();
    // ����ʱ���
    aTimeBase = dataSingleton.getaTimeBase();
    // ����PTS
    beforePTS = currPTS = 0;

    // ����Ƶ�豸��������Ƶ����
    want_spec.freq = codecCtx->sample_rate;
    want_spec.format = AUDIO_S16SYS;
    want_spec.channels = codecCtx->channels;
    want_spec.silence = 0;
    want_spec.samples = 1024;
    want_spec.callback = audio_callback;
    want_spec.userdata = this;

    SDL_OpenAudio(&want_spec, NULL);

    if (au_convert_ctx)
    {
        swr_free(&au_convert_ctx);
        au_convert_ctx = nullptr;
    }
    uint64_t out_chn_layout = av_get_default_channel_layout(codecCtx->channels); //AV_CH_LAYOUT_STEREO;  //ͨ������ ���˫����
    au_convert_ctx = swr_alloc_set_opts(nullptr,
        // �����Ƶ����
        out_chn_layout, AV_SAMPLE_FMT_S16, codecCtx->sample_rate,
        // ������Ƶ����
        codecCtx->channel_layout, codecCtx->sample_fmt, codecCtx->sample_rate,
        0, nullptr);
    if (!au_convert_ctx || swr_init(au_convert_ctx) < 0) {
        std::cerr << "��Ƶ�ز��������Ĵ���" << std::endl;
    }

    converted_audio_size = av_samples_get_buffer_size(nullptr, codecCtx->channels, codecCtx->frame_size, AV_SAMPLE_FMT_S16, 1);

    converted_audio_data = (uint8_t*)av_malloc(converted_audio_size);
    if (!converted_audio_data) {
        std::cerr << "Failed to allocate memory for converted audio data" << std::endl;
        // ������
    }
}

void AudioOutThread::run()
{
    SDL_PauseAudio(0);
    while (isRunning())
    {
        frameQueue->waitAndPop(frame);

        //std::cerr << frame->pts * av_q2d(aTimeBase) << std::endl;
        beforePTS = currPTS;
        currPTS = frame->pts * av_q2d(aTimeBase);

        dataSingleton.setPTS(beforePTS, currPTS);

        // ������Ƶ�ز�������  
        if (swr_convert(au_convert_ctx, &converted_audio_data, codecCtx->frame_size, (const uint8_t**)frame->data, frame->nb_samples) < 0)
            std::cerr << "Failed to convert audio data" << std::endl;
        av_frame_free(&frame);

        while (audioLen > 0)
            SDL_Delay(1);

        audioChunk = (unsigned char*)converted_audio_data;
        audioPos = audioChunk; 
        audioLen = converted_audio_size;
    }
    SDL_PauseAudio(1);
}

// SDL2 ��Ƶ�ص�����
void AudioOutThread::audio_callback(void* udata, Uint8* stream, int len)
{
    SDL_memset(stream, 0, len);

    if (audioLen == 0)
        return;

    len = (len > audioLen ? audioLen : len);

    //SDL_MixAudio(stream, audioPos, len, SDL_MIX_MAXVOLUME);
    SDL_MixAudio(stream, audioPos, len, volume);

    audioPos += len;
    audioLen -= len;
}