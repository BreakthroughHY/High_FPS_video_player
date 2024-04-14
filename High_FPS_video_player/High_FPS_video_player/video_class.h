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

	// ���ر�����Ƶ
	bool loadVideo(QString videoPath);
	// ��ȡ��ǰ��Ƶ����ʱ��
	int getTotalVideoDuration();

private:
	// ȫ��Ψһ�����ݶ��� ֧�ֶ��̷߳���
	DataSingleton& dataSingleton;
	// ����Ƶ��ʽ�������Ľṹ��ָ��
	AVFormatContext* formatCtx = nullptr;

	// ��Ƶ����Ƶ������
	unsigned videoIndex, audioIndex;
	// ������
	AVCodecContext* vCodecCtx = nullptr, * aCodecCtx = nullptr;

	// ��ǰ������Ƶ����ʱ��
	int totalVideoDuration = -1;
};