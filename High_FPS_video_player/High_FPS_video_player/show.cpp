#include "show.h"

Show::Show(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.VolumeSlider, &QSlider::valueChanged, [](int v) {
		qDebug() << v;
		});
	// 样式
	setStyle();
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
	
	// 设置隐藏视频列表按钮位置
	ui.btnCloseVideoList->move(width() - ui.btnCloseVideoList->width(), (height() - ui.btnCloseVideoList->height()) / 2);

	// 向主窗口发信号设置CtrlBarWid位置
	emit sig_setCtrlBarWidPos();
	// 向ctr_bar发射信号通知进行音量滑块位置的设置
	emit sig_SetVolumeSliderPos();
}

// 鼠标移动虚函数实现
void Show::mouseMoveEvent(QMouseEvent* event)
{
	//qDebug() << event->pos();
}

// 样式
void Show::setStyle()
{
	// 加载自定义字体文件
	int fontId = QFontDatabase::addApplicationFont(":/iconFont/font/iconfont.ttf");
	QString fontName = QFontDatabase::applicationFontFamilies(fontId).at(0);
	fontIcon.setFamily(fontName);

	ui.btnCloseVideoList->setFont(fontIcon);
	ui.btnCloseVideoList->setText(QChar(0xe608));

}

// 设置音量滑块位置的槽函数
void Show::do_SetVolumeSliderPos(int x)
{
	ui.VolumeWid->move(x - 15, height() - 225);
}