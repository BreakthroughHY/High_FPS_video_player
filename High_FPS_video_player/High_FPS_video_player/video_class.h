#pragma once

#include <iostream>
#include <QString>
#include "data_singleton.h"

//extern "C"
//{
//#include <libavformat\avformat.h>
//}


class VideoClass
{
public:
	VideoClass();
	//videoClass(const std::string& videoName);
	~VideoClass();

	// 加载本地视频
	bool loadVideo(QString videoPath);
	// 获取当前视频的总时长
	int getTotalVideoDuration();

private:
	// 全局唯一的数据对象 支持多线程访问
	DataSingleton& dataSingleton;
	// 音视频格式的上下文结构体指针
	AVFormatContext* formatCtx = nullptr;

	// 视频和音频的索引
	unsigned videoIndex, audioIndex;
	// 解码器
	AVCodecContext* vCodecCtx = nullptr, * aCodecCtx = nullptr;

	// 当前播放视频的总时长
	int totalVideoDuration = -1;
};