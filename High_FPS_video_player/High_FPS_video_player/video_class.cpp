#include "video_class.h"

videoClass::videoClass()
	: dataSingleton(DataSingleton::getInstance())
{}

//videoClass::videoClass(const std::string& videoName)
//{
//
//}

// 加载本地视频
bool videoClass::loadVideo(std::string videoPath)
{
	if (formatCtx)
		avformat_close_input(&formatCtx);

	QString utf8Path = QString::fromLocal8Bit(videoPath.c_str()).toUtf8();

	if (avformat_open_input(&formatCtx, utf8Path.toStdString().c_str(), nullptr, nullptr) != 0)
	{
		std::cout << "视频文件打开失败！" << std::endl;
		return false;
	}

	//查找输入流信息
	if (avformat_find_stream_info(formatCtx, NULL) < 0) {
		std::cout << "没有找到流信息	！" << std::endl;
		return false;
	}

	//获取视频流
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
