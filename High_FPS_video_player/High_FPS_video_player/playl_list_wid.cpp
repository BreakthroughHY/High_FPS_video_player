#include "playl_list_wid.h"

PlaylListWid::PlaylListWid(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

PlaylListWid::~PlaylListWid()
{}

// �����麯��ʵ��
void PlaylListWid::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.fillRect(event->rect(), QColor(23, 24, 27)); // ���ñ�����ɫΪǳ��ɫ
}
