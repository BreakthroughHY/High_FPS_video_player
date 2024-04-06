#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

using namespace std;


template <typename T>
class FFmpegSafeQueue
{
public:
	FFmpegSafeQueue();

	FFmpegSafeQueue(int capacity);


	// ���
	void push(const T& item);
	// ���Գ���
	bool tryPop(T& item);
	// �ȴ������Գ���
	void waitAndPop(T& item);
	// ����������С
	void setCapacity(int capacity);

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