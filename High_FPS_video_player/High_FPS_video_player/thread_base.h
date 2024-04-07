#pragma once

#include <thread>
#include <atomic>

class ThreadBase
{
public:
	ThreadBase() : m_running(false) {}
	virtual ~ThreadBase() {} // ȷ������������Լ�������

	void start();

	void stop();

protected:
	virtual void run() = 0; // ���麯��  ���������д

	bool isRunning() const; // ��ȡ�̵߳������������ȷ�������޸�״̬����

private:
	std::thread m_thread; // �߳�

	std::atomic<bool> m_running; // �߳��Ƿ�����    ԭ�Ӳ���ȷ���̰߳�ȫ
};

