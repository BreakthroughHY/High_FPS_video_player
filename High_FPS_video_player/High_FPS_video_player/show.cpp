#include "show.h"

Show::Show(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.VolumeSlider, &QSlider::valueChanged, [](int v) {
		qDebug() << v;
		});
	// ��ʽ
	setStyle();
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
	
	// ����������Ƶ�б�ťλ��
	ui.btnCloseVideoList->move(width() - ui.btnCloseVideoList->width(), (height() - ui.btnCloseVideoList->height()) / 2);

	// �������ڷ��ź�����CtrlBarWidλ��
	emit sig_setCtrlBarWidPos();
	// ��ctr_bar�����ź�֪ͨ������������λ�õ�����
	emit sig_SetVolumeSliderPos();
}

// ����ƶ��麯��ʵ��
void Show::mouseMoveEvent(QMouseEvent* event)
{
	//qDebug() << event->pos();
}

// ��ʽ
void Show::setStyle()
{
	// �����Զ��������ļ�
	int fontId = QFontDatabase::addApplicationFont(":/iconFont/font/iconfont.ttf");
	QString fontName = QFontDatabase::applicationFontFamilies(fontId).at(0);
	fontIcon.setFamily(fontName);

	ui.btnCloseVideoList->setFont(fontIcon);
	ui.btnCloseVideoList->setText(QChar(0xe608));

}

// ������������λ�õĲۺ���
void Show::do_SetVolumeSliderPos(int x)
{
	ui.VolumeWid->move(x - 15, height() - 225);
}