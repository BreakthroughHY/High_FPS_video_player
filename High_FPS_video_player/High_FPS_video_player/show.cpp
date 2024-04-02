#include "show.h"

Show::Show(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	QSlider* slider = new QSlider(Qt::Vertical, this);
	slider->setRange(0, 100); // ���÷�Χ
	slider->setStyleSheet("QSlider::groove:horizontal {border: 1px solid #4A708B;background: #C0C0C0;height: 3px;border - radius: 2px;padding - left:-1px;padding - right:-1px;}");
	slider->setGeometry(50, 150, 30, 100);

	slider->show();
}

Show::~Show()
{}

// �����麯��ʵ��
void Show::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.fillRect(event->rect(), QColor(0, 0, 0));
}

// ��С�ı��麯��
void Show::resizeEvent(QResizeEvent* event)
{
	// ����openGL����λ��
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

	// �������ڷ��ź�����CtrlBarWidλ��
	emit sig_setCtrlBarWidPos();
}

// ����ƶ��麯��ʵ��
void Show::mouseMoveEvent(QMouseEvent* event)
{
	qDebug() << event->pos();
}