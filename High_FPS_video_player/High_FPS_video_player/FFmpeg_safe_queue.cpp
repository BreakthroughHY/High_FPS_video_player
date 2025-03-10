#include "FFmpeg_safe_queue.h"


template <typename T>
FFmpegSafeQueue<T>::FFmpegSafeQueue() {}

template <typename T>
FFmpegSafeQueue<T>::FFmpegSafeQueue(int capacity)
{
	this->capacity = capacity;
}

// 尝试入队
template <typename T>
bool FFmpegSafeQueue<T>::tryPush(const T& item)
{
	unique_lock<mutex> lock(m_mutex);

	if (full()) // 队列为满就直接push失败
		return false;

	// 插入新的数据
	m_queue.push(item);

	// 尝试去激活一个消费线程，如果有的话
	m_cond_pop.notify_one();

	return true;
}

// 入队
template <typename T>
void FFmpegSafeQueue<T>::push(const T& item)
{
	unique_lock<mutex> lock(m_mutex);

	// 队列为满就阻塞生产线程
	m_cond_push.wait(lock, [this]() { return !full(); });
	
	// 插入新的数据
	m_queue.push(item);
	//std::cout << m_queue.size() << std::endl;
	// 尝试去激活一个消费线程，如果有的话
	m_cond_pop.notify_one();

}


// 尝试出队
template <typename T>
bool FFmpegSafeQueue<T>::tryPop(T& item)
{
	unique_lock<mutex> lock(m_mutex);

	// 尝试取出数据如果队列为空直接返回false取出失败
	if (m_queue.empty())
		return false;

	// 取出数据
	item = m_queue.front();
	// 删除被取出的数据
	m_queue.pop();

	// 尝试去激活一个生产线程，如果有的话
	m_cond_push.notify_one();

	return true;
}

// 等待到可以出队
template <typename T>
void FFmpegSafeQueue<T>::waitAndPop(T& item)
{
	unique_lock<mutex> lock(m_mutex);

	// 如果队列为空就阻塞到有数据
	m_cond_pop.wait(lock, [this]() { return !m_queue.empty(); });

	// 取出数据
	item = m_queue.front();
	// 删除被取出的数据
	m_queue.pop();

	// 尝试去激活一个生产线程，如果有的话
	m_cond_push.notify_one();
}

// 设置容量大小
template <typename T>
bool FFmpegSafeQueue<T>::setCapacity(int capacity)
{
	if (capacity <= 0) // 传入的大小非法直接退出  锁不申请
		return false;
	unique_lock<mutex> lock(m_mutex);

	clear(); // 清空容器

	this->capacity = capacity; // 设置新的容量

	// 尝试去激活一个生产线程，如果有的话
	m_cond_push.notify_one();  // 重置大小会清空内容，如果生产线程处在阻塞，队列为空两个线程都会被阻塞

	return true;
}

// 清空队列的方法带锁确保线程安全
template <typename T>
void FFmpegSafeQueue<T>::clearMutex()
{
	unique_lock<mutex> lock(m_mutex);
	this->clear(); // 调用私有不带锁的清空函数
}

// 队列是否为满
template <typename T>
bool FFmpegSafeQueue<T>::full()
{
	if (capacity <= m_queue.size()) return true;

	return false;
}

template <> // T = AVPacket*的特化函数
void FFmpegSafeQueue<AVPacket*>::clear()
{
	AVPacket* packet = nullptr;
	// 清空队列
	while (!m_queue.empty())
	{
		packet = m_queue.front();
		m_queue.pop();
		av_packet_free(&packet);
	}
	// 尝试去激活一个生产线程，如果有的话
	m_cond_push.notify_one();
}

template <> // T = AVFrame*的特化函数
void FFmpegSafeQueue<AVFrame*>::clear()
{
	AVFrame* frame = nullptr;
	while (!m_queue.empty())
	{
		frame = m_queue.front();
		m_queue.pop();
		av_frame_free(&frame);
	}
	// 尝试去激活一个生产线程，如果有的话
	m_cond_push.notify_one();
}

template <> // T = Myframe*的特化函数
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
	// 尝试去激活一个生产线程，如果有的话
	m_cond_push.notify_one();
}

// 显式实例化，以便确保编译器生成所需的模板实例
template class FFmpegSafeQueue<AVPacket*>;
template class FFmpegSafeQueue<AVFrame*>;
template class FFmpegSafeQueue<Myframe*>;
