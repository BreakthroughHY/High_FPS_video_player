#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <iostream>

using namespace std;

extern "C"
{
#include <libavcodec\packet.h>
#include <libavcodec\avcodec.h>
}

typedef struct Myframe {
	uint8_t* data[4];
	int linesize[4];
	int64_t pts;
	uint8_t* outBuffer = NULL;
}Myframe;

template <typename T>
class FFmpegSafeQueue
{
public:
	FFmpegSafeQueue();

	FFmpegSafeQueue(int capacity);

	// �������
	bool tryPush(const T& item);
	// ���
	void push(const T& item);
	// ���Գ���
	bool tryPop(T& item);
	// �ȴ������Գ���
	void waitAndPop(T& item);
	// ����������С
	bool setCapacity(int capacity);
	// ��ն��еķ�������ȷ���̰߳�ȫ
	void clearMutex();

private:
	// ��ֹ�ⲿ����ֻ��ͨ��public�еĺ�����ӵ��ã����Ժ����в���ʹ��lock
	// �����Ƿ�Ϊ��
	bool full();
	// ��ն��еķ���
	void clear();

private:
	queue<T> m_queue;
	// ������ (mutable���ڱ�֤m_mutex��const���εĺ�����Ҳ�����޸�)
	mutable mutex m_mutex;
	// ��������ߺ�����������
	condition_variable m_cond_pop;
	condition_variable m_cond_push;

	// ����
	int capacity = 100;
};