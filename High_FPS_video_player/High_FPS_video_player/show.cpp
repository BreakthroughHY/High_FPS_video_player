#include "show.h"

Show::Show(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	QSlider* slider = new QSlider(Qt::Vertical, this);
	slider->setRange(0, 100); // 设置范围
	slider->setStyleSheet("QSlider::groove:horizontal {border: 1px solid #4A708B;background: #C0C0C0;height: 3px;border - radius: 2px;padding - left:-1px;padding - right:-1px;}");
	slider->setGeometry(50, 150, 30, 100);

	slider->show();
}

Show::~Show()
{}

// 绘制虚函数实现
void Show::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.fillRect(event->rect(), QColor(0, 0, 0));
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

	// 向主窗口发信号设置CtrlBarWid位置
	emit sig_setCtrlBarWidPos();
}

// 鼠标移动虚函数实现
void Show::mouseMoveEvent(QMouseEvent* event)
{
	qDebug() << event->pos();
}