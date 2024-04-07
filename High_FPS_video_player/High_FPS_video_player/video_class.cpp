#include "video_class.h"

videoClass::videoClass()
	: dataSingleton(DataSingleton::getInstance())
{}

//videoClass::videoClass(const std::string& videoName)
//{
//
//}

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

	//��ȡ��Ƶ��
	for (unsigned i = 0; i < formatCtx->nb_streams; ++i)
		if (formatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
			videoIndex = i;
		else if (formatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
			audioIndex = i;


	dataSingleton.setFormatCtx(formatCtx);
	dataSingleton.setVideoIndex(videoIndex);
	dataSingleton.setAudioIndex(audioIndex);

	return true;
}
