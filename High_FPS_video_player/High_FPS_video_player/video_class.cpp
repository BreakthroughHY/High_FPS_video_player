#include "video_class.h"

VideoClass::VideoClass()
	: dataSingleton(DataSingleton::getInstance())
{
	vCodecCtx = avcodec_alloc_context3(NULL);
	aCodecCtx = avcodec_alloc_context3(NULL);
}

//videoClass::videoClass(const std::string& videoName)
//{
//
//}

VideoClass::~VideoClass()
{
	avcodec_free_context(&vCodecCtx);
	avcodec_free_context(&aCodecCtx);
}

// 加载本地视频
bool VideoClass::loadVideo(QString videoPath)
{
	if (formatCtx)
	{
		avformat_close_input(&formatCtx);
		formatCtx = nullptr;
	}

	formatCtx = avformat_alloc_context();

	//QString utf8Path = QString::fromLocal8Bit(videoPath.c_str()).toUtf8();

	if (avformat_open_input(&formatCtx, videoPath.toStdString().c_str(), nullptr, nullptr) != 0)
	//if (avformat_open_input(&formatCtx, utf8Path.toStdString().c_str(), nullptr, nullptr) != 0)
	{
		std::cout << "视频文件打开失败！" << std::endl;
		return false;
	}

	//查找输入流信息
	if (avformat_find_stream_info(formatCtx, NULL) < 0) {
		std::cout << "没有找到流信息	！" << std::endl;
		return false;
	}

	videoIndex = audioIndex = -1;
	//获取视频流
	for (unsigned i = 0; i < formatCtx->nb_streams; ++i)
	{
		if (formatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
			videoIndex = i;
		else if (formatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
			audioIndex = i;
	}
	if (videoIndex < 0 || audioIndex < 0)
	{
		std::cout << "流索引！" << std::endl;
		return false;
	}


	if (vCodecCtx)
	{
		avcodec_free_context(&vCodecCtx);
		vCodecCtx = avcodec_alloc_context3(NULL);
	}

	avcodec_parameters_to_context(vCodecCtx, formatCtx->streams[videoIndex]->codecpar);
	const AVCodec* pCodecV = avcodec_find_decoder(vCodecCtx->codec_id);
	if (pCodecV == NULL)
	{
		std::cout << "没有发现视频解码器" << std::endl;
		return false;
	}
	avcodec_open2(vCodecCtx, pCodecV, NULL);


	if (aCodecCtx)
	{
		avcodec_free_context(&aCodecCtx);
		aCodecCtx = avcodec_alloc_context3(NULL);
	}
	avcodec_parameters_to_context(aCodecCtx, formatCtx->streams[audioIndex]->codecpar);
	const AVCodec* pCodecA = avcodec_find_decoder(aCodecCtx->codec_id);
	if (pCodecA == NULL)
	{
		std::cout << "没有发现音频解码器" << std::endl;
		return false;
	}
	avcodec_open2(aCodecCtx, pCodecA, NULL);


	// 数据同步到数据单例对象中
	dataSingleton.setFormatCtx(formatCtx);
	dataSingleton.setVideoIndex(videoIndex);
	dataSingleton.setAudioIndex(audioIndex);
	dataSingleton.setVCodecCtx(vCodecCtx);
	dataSingleton.setACodecCtx(aCodecCtx);

	dataSingleton.setvTimeBase(formatCtx->streams[videoIndex]->time_base);
	dataSingleton.setaTimeBase(formatCtx->streams[audioIndex]->time_base);

	dataSingleton.setWdividedH(vCodecCtx->width * 1.0 / vCodecCtx->height);

	AVRational frameRate = formatCtx->streams[videoIndex]->r_frame_rate;
	dataSingleton.setFPSV(av_q2d(frameRate));

	AVRational timeBaseA = formatCtx->streams[audioIndex]->time_base;
	totalVideoDuration = (int)formatCtx->streams[audioIndex]->duration * av_q2d(timeBaseA);


	//int64_t duration = formatCtx->duration;

	// 获取视频流的时间基
	//AVRational timeBase = formatCtx->streams[videoIndex]->time_base;
	//
	//// 转换为实际时长（秒）
	//double durationInSeconds = (double)formatCtx->streams[videoIndex]->duration * av_q2d(timeBase);

	//std::cout << durationInSeconds << std::endl;
	//std::cout << "num:" << timeBase.num << " den:" << timeBase.den << std::endl;

	//AVRational timeBaseA = formatCtx->streams[audioIndex]->time_base;

	////// 转换为实际时长（秒）
	//double durationInSecondsA = (double)formatCtx->streams[audioIndex]->duration * av_q2d(timeBaseA);

	//std::cout << durationInSecondsA << std::endl;
	//std::cout << "num:" << timeBaseA.num << " den:" << timeBaseA.den << std::endl;



	//std::cout << timeBase.num << std::endl;
	//std::cout << "   " << vCodecCtx->time_base.num << std::endl;

	return true;
}

// 获取当前视频的总时长
int VideoClass::getTotalVideoDuration()
{
	return this->totalVideoDuration;
}
