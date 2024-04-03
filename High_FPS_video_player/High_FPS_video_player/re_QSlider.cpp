#include "re_QSlider.h"

ReQSlider::ReQSlider(QWidget* parent)
	: QSlider(parent)
{

}

ReQSlider::~ReQSlider()
{}

// 鼠标按下虚函数实现
void ReQSlider::mousePressEvent(QMouseEvent* event)
{
	int a = (int)(event->pos().x() * 1.0 / width() * 100);
	qDebug() << event->pos() << "      " << a;
	setValue(a);
}

// 鼠标移动虚函数实现
void ReQSlider::mouseMoveEvent(QMouseEvent* event)
{
	int a = (int)(event->pos().x() * 1.0 / width() * 100);
	qDebug() << event->pos() << "      " << a;
	setValue(a);
}