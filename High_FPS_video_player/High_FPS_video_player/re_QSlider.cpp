#include "re_QSlider.h"

ReQSlider::ReQSlider(QWidget* parent)
	: QSlider(parent)
{

}

ReQSlider::~ReQSlider()
{}

// ��갴���麯��ʵ��
void ReQSlider::mousePressEvent(QMouseEvent* event)
{
	int a = (int)(event->pos().x() * 1.0 / width() * 100);
	qDebug() << event->pos() << "      " << a;
	setValue(a);
}

// ����ƶ��麯��ʵ��
void ReQSlider::mouseMoveEvent(QMouseEvent* event)
{
	int a = (int)(event->pos().x() * 1.0 / width() * 100);
	qDebug() << event->pos() << "      " << a;
	setValue(a);
}