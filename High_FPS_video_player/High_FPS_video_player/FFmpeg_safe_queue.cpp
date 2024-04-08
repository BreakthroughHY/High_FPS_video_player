#include "FFmpeg_safe_queue.h"


template <typename T>
FFmpegSafeQueue<T>::FFmpegSafeQueue() {}

template <typename T>
FFmpegSafeQueue<T>::FFmpegSafeQueue(int capacity)
{
	this->capacity = capacity;
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
	if (m_queue.empty()) return false;

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

// �����Ƿ�Ϊ��
template <typename T>
bool FFmpegSafeQueue<T>::full()
{
	if (capacity <= m_queue.size()) return true;

	return false;
}

// ��ն��еķ��� ��û����Ϊ���̰߳�ȫֻ�ܱ��������ĺ������ã�
template <typename T>
void FFmpegSafeQueue<T>::clear()
{
	// ��ն���
	while (!m_queue.empty())
		m_queue.pop();

	// ����ȥ����һ�������̣߳�����еĻ�
	m_cond_push.notify_one();
}

// ��ʽʵ�������Ա�ȷ�����������������ģ��ʵ��
template class FFmpegSafeQueue<AVPacket*>;
template class FFmpegSafeQueue<AVFrame*>;
