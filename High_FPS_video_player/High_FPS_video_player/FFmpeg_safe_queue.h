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

	// 尝试入队
	bool tryPush(const T& item);
	// 入队
	void push(const T& item);
	// 尝试出队
	bool tryPop(T& item);
	// 等待到可以出队
	void waitAndPop(T& item);
	// 设置容量大小
	bool setCapacity(int capacity);
	// 清空队列的方法带锁确保线程安全
	void clearMutex();

private:
	// 禁止外部访问只能通过public中的函数间接调用，所以函数中不用使用lock
	// 队列是否为满
	bool full();
	// 清空队列的方法
	void clear();

private:
	queue<T> m_queue;
	// 互斥锁 (mutable用于保证m_mutex在const修饰的函数中也可以修改)
	mutable mutex m_mutex;
	// 解决消费者和生产者问题
	condition_variable m_cond_pop;
	condition_variable m_cond_push;

	// 容量
	int capacity = 100;
};