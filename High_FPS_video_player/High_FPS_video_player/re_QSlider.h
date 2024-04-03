#pragma once

#include <QSlider>
#include <QMouseEvent>

class ReQSlider : public QSlider
{
public:
	ReQSlider(QWidget* parent = nullptr);
	~ReQSlider();

protected:
	// ��갴���麯��ʵ��
	void mousePressEvent(QMouseEvent* event) override;
	// ����ƶ��麯��ʵ��
	void mouseMoveEvent(QMouseEvent* event) override;
};