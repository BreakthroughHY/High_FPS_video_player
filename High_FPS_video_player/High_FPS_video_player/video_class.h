#pragma once

#include <iostream>
#include <QString>
#include "data_singleton.h"

//extern "C"
//{
//#include <libavformat\avformat.h>
//}


class videoClass
{
public:
	videoClass();
	//videoClass(const std::string& videoName);
	~videoClass();

	// ���ر�����Ƶ
	bool loadVideo(std::string videoPath);

private:
	// ȫ��Ψһ�����ݶ��� ֧�ֶ��̷߳���
	DataSingleton& dataSingleton;
	// ����Ƶ��ʽ�������Ľṹ��ָ��
	AVFormatContext* formatCtx = nullptr;

	// ��Ƶ����Ƶ������
	unsigned videoIndex, audioIndex;
	// ������
	AVCodecContext* vCodecCtx = nullptr, * aCodecCtx = nullptr;
};