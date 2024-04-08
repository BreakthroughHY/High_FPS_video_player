#include "video_class.h"

videoClass::videoClass()
	: dataSingleton(DataSingleton::getInstance())
{
	vCodecCtx = avcodec_alloc_context3(NULL);
	aCodecCtx = avcodec_alloc_context3(NULL);
}

//videoClass::videoClass(const std::string& videoName)
//{
//
//}

videoClass::~videoClass()
{
	avcodec_free_context(&vCodecCtx);
	avcodec_free_context(&aCodecCtx);
}

// ���ر�����Ƶ
bool videoClass::loadVideo(std::string videoPath)
{
	if (formatCtx)
		avformat_close_input(&formatCtx);

	QString utf8Path = QString::fromLocal8Bit(videoPath.c_str()).toUtf8();

	if (avformat_open_input(&formatCtx, utf8Path.toStdString().c_str(), nullptr, nullptr) != 0)
	{
		std::cout << "��Ƶ�ļ���ʧ�ܣ�" << std::endl;
		return false;
	}

	//������������Ϣ
	if (avformat_find_stream_info(formatCtx, NULL) < 0) {
		std::cout << "û���ҵ�����Ϣ	��" << std::endl;
		return false;
	}

	videoIndex = audioIndex = -1;
	//��ȡ��Ƶ��
	for (unsigned i = 0; i < formatCtx->nb_streams; ++i)
	{
		if (formatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
			videoIndex = i;
		else if (formatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
			audioIndex = i;
	}
	if (videoIndex < 0 || audioIndex < 0)
	{
		std::cout << "��������" << std::endl;
		return false;
	}


	avcodec_parameters_to_context(vCodecCtx, formatCtx->streams[videoIndex]->codecpar);
	const AVCodec* pCodecV = avcodec_find_decoder(vCodecCtx->codec_id);
	if (pCodecV == NULL)
	{
		std::cout << "û�з�����Ƶ������" << std::endl;
		return false;
	}

	avcodec_open2(vCodecCtx, pCodecV, NULL);

	avcodec_parameters_to_context(aCodecCtx, formatCtx->streams[audioIndex]->codecpar);
	const AVCodec* pCodecA = avcodec_find_decoder(aCodecCtx->codec_id);
	if (pCodecA == NULL)
	{
		std::cout << "û�з�����Ƶ������" << std::endl;
		return false;
	}
	avcodec_open2(aCodecCtx, pCodecA, NULL);


	// ����ͬ�������ݵ���������
	dataSingleton.setFormatCtx(formatCtx);
	dataSingleton.setVideoIndex(videoIndex);
	dataSingleton.setAudioIndex(audioIndex);
	dataSingleton.setVCodecCtx(vCodecCtx);
	dataSingleton.setACodecCtx(aCodecCtx);


	return true;
}
