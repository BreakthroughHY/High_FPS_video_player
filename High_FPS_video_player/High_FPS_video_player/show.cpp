#include "show.h"

Show::Show(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

Show::~Show()
{}

// 绘制虚函数实现
void Show::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.fillRect(event->rect(), QColor(255, 132, 0)); // 设置背景颜色为浅灰色
}

// 大小改变虚函数
void Show::resizeEvent(QResizeEvent* event)
{
	// 设置openGL窗口位置
	double w = (double)width();
	double h = (double)height();
	if (w / h <= WdividedH)
	{
		int openGLH = w / WdividedH;
		ui.openGLWidget->setGeometry(0, (h - openGLH) / 2, w, openGLH);
	}
	else
	{
		int openGLW = h * WdividedH;
		ui.openGLWidget->setGeometry((w - openGLW) / 2, 0, openGLW, h);
	}
}