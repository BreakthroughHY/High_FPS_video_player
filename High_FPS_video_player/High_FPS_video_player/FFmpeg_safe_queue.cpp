#include "FFmpeg_safe_queue.h"

template <typename T>
FFmpegSafeQueue<T>::FFmpegSafeQueue() {}

template <typename T>
FFmpegSafeQueue<T>::FFmpegSafeQueue(int capacity)
{
	this->capacity = capacity;
}

// 入队
template <typename T>
void FFmpegSafeQueue<T>::push(const T& item)
{
	lock_guard<mutex> lock(m_mutex);

	// 队列为满就阻塞生产线程
	m_cond_push.wait(lock, [this]() { return !full(); });

	// 插入新的数据
	m_queue.push(item);
	// 尝试去激活一个消费线程，如果有的话
	m_cond_pop.notify_one();
}


// 尝试出队
template <typename T>
bool FFmpegSafeQueue<T>::tryPop(T& item)
{
	lock_guard<mutex> lock(m_mutex);

	// 尝试取出数据如果队列为空直接返回false取出失败
	if (m_queue.empty()) return false;

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
	lock_guard<mutex> lock(m_mutex);

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
void FFmpegSafeQueue<T>::setCapacity(int capacity)
{
	clear(); // 清空容器

	this->capacity = capacity; // 设置新的容量
}

// 队列是否为满
template <typename T>
bool FFmpegSafeQueue<T>::full()
{
	if (capacity <= m_queue.size()) return true;

	return false;
}

// 清空队列的方法 （没加锁为了线程安全只能被加了锁的函数调用）
template <typename T>
void FFmpegSafeQueue<T>::clear()
{
	// 清空队列
	while (!m_queue.empty())
		m_queue.pop();

	// 尝试去激活一个生产线程，如果有的话
	m_cond_push.notify_one();
}