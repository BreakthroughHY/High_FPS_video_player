#pragma once

#include <QSlider>
#include <QMouseEvent>

class ReQSlider : public QSlider
{
public:
	ReQSlider(QWidget* parent = nullptr);
	~ReQSlider();

protected:
	// 鼠标按下虚函数实现
	void mousePressEvent(QMouseEvent* event) override;
	// 鼠标移动虚函数实现
	void mouseMoveEvent(QMouseEvent* event) override;
};