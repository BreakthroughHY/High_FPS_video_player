#include "FFmpeg_safe_queue.h"


template <typename T>
FFmpegSafeQueue<T>::FFmpegSafeQueue() {}

template <typename T>
FFmpegSafeQueue<T>::FFmpegSafeQueue(int capacity)
{
	this->capacity = capacity;
}

// �������
template <typename T>
bool FFmpegSafeQueue<T>::tryPush(const T& item)
{
	unique_lock<mutex> lock(m_mutex);

	if (full()) // ����Ϊ����ֱ��pushʧ��
		return false;

	// �����µ�����
	m_queue.push(item);

	// ����ȥ����һ�������̣߳�����еĻ�
	m_cond_pop.notify_one();

	return true;
}

// ���
template <typename T>
void FFmpegSafeQueue<T>::push(const T& item)
{
	unique_lock<mutex> lock(m_mutex);

	// ����Ϊ�������������߳�
	m_cond_push.wait(lock, [this]() { return !full(); });
	
	// �����µ�����
	m_queue.push(item);
	//std::cout << m_queue.size() << std::endl;
	// ����ȥ����һ�������̣߳�����еĻ�
	m_cond_pop.notify_one();

}


// ���Գ���
template <typename T>
bool FFmpegSafeQueue<T>::tryPop(T& item)
{
	unique_lock<mutex> lock(m_mutex);

	// ����ȡ�������������Ϊ��ֱ�ӷ���falseȡ��ʧ��
	if (m_queue.empty())
		return false;

	// ȡ������
	item = m_queue.front();
	// ɾ����ȡ��������
	m_queue.pop();

	// ����ȥ����һ�������̣߳�����еĻ�
	m_cond_push.notify_one();

	return true;
}

// �ȴ������Գ���
template <typename T>
void FFmpegSafeQueue<T>::waitAndPop(T& item)
{
	unique_lock<mutex> lock(m_mutex);

	// �������Ϊ�վ�������������
	m_cond_pop.wait(lock, [this]() { return !m_queue.empty(); });

	// ȡ������
	item = m_queue.front();
	// ɾ����ȡ��������
	m_queue.pop();

	// ����ȥ����һ�������̣߳�����еĻ�
	m_cond_push.notify_one();
}

// ����������С
template <typename T>
bool FFmpegSafeQueue<T>::setCapacity(int capacity)
{
	if (capacity <= 0) // ����Ĵ�С�Ƿ�ֱ���˳�  ��������
		return false;
	unique_lock<mutex> lock(m_mutex);

	clear(); // �������

	this->capacity = capacity; // �����µ�����

	// ����ȥ����һ�������̣߳�����еĻ�
	m_cond_push.notify_one();  // ���ô�С��������ݣ���������̴߳�������������Ϊ�������̶߳��ᱻ����

	return true;
}

// ��ն��еķ�������ȷ���̰߳�ȫ
template <typename T>
void FFmpegSafeQueue<T>::clearMutex()
{
	unique_lock<mutex> lock(m_mutex);
	this->clear(); // ����˽�в���������պ���
}

// �����Ƿ�Ϊ��
template <typename T>
bool FFmpegSafeQueue<T>::full()
{
	if (capacity <= m_queue.size()) return true;

	return false;
}

template <> // T = AVPacket*���ػ�����
void FFmpegSafeQueue<AVPacket*>::clear()
{
	AVPacket* packet = nullptr;
	// ��ն���
	while (!m_queue.empty())
	{
		packet = m_queue.front();
		m_queue.pop();
		av_packet_free(&packet);
	}
	// ����ȥ����һ�������̣߳�����еĻ�
	m_cond_push.notify_one();
}

template <> // T = AVFrame*���ػ�����
void FFmpegSafeQueue<AVFrame*>::clear()
{
	AVFrame* frame = nullptr;
	while (!m_queue.empty())
	{
		frame = m_queue.front();
		m_queue.pop();
		av_frame_free(&frame);
	}
	// ����ȥ����һ�������̣߳�����еĻ�
	m_cond_push.notify_one();
}

template <> // T = Myframe*���ػ�����
void FFmpegSafeQueue<Myframe*>::clear()
{
	Myframe* myFrame = nullptr;
	while (!m_queue.empty())
	{
		myFrame = m_queue.front();
		m_queue.pop();
		av_freep(&myFrame->outBuffer);
		delete myFrame;
	}
	// ����ȥ����һ�������̣߳�����еĻ�
	m_cond_push.notify_one();
}

// ��ʽʵ�������Ա�ȷ�����������������ģ��ʵ��
template class FFmpegSafeQueue<AVPacket*>;
template class FFmpegSafeQueue<AVFrame*>;
template class FFmpegSafeQueue<Myframe*>;
