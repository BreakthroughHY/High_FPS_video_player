#pragma once

#include <thread>
#include <atomic>

class ThreadBase
{
public:
	ThreadBase() : m_running(false) {}
	virtual ~ThreadBase() {} // 确保派生类调用自己的析构

	void start();

	void stop();

protected:
	virtual void run() = 0; // 纯虚函数  子类必须重写

	bool isRunning() const; // 获取线程的运行情况，并确保不会修改状态变量

private:
	std::thread m_thread; // 线程

	std::atomic<bool> m_running; // 线程是否运行    原子操作确保线程安全
};

