#include "thread_base.h"


void ThreadBase::start()
{
	if (!m_running)
	{
		m_running = true;
		m_thread = std::thread(&ThreadBase::run, this);
	}
}

void ThreadBase::stop()
{
	if (m_running)
	{
		m_running = false;
		if (m_thread.joinable())
		{
			m_thread.join();
		}
	}
}

bool ThreadBase::isRunning() const // ��ȡ�̵߳������������ȷ�������޸�״̬����
{
	return this->m_running;
}