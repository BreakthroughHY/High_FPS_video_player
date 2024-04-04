#include "show.h"

Show::Show(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	// 样式
	setStyle();

	// 定时器对象指针  (要在连接槽函数之前创建计时器对象)
	volumeWidTimer = new QTimer(this);
	btnCloseVideoListTimer = new QTimer(this);
	// 定时精度
	volumeWidTimer->setTimerType(Qt::PreciseTimer);
	btnCloseVideoListTimer->setTimerType(Qt::PreciseTimer);
	// 定时间隔
	volumeWidTimer->setInterval(500);
	btnCloseVideoListTimer->setInterval(1000);
	// 设置单次计时
	volumeWidTimer->setSingleShot(true);
	btnCloseVideoListTimer->setSingleShot(true);

	// 连接信号和槽
	connectSignalSlots();

	ui.VolumeWid->hide();
	ui.btnCloseVideoList->hide();
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
}

// 鼠标移动虚函数实现
void Show::mouseMoveEvent(QMouseEvent* event)
{
}

// 事件过滤器虚函数
bool Show::eventFilter(QObject* obj, QEvent* event)
{
	return QWidget::eventFilter(obj, event);
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

// 连接信号和槽
void Show::connectSignalSlots()
{
	// 连接volumeWidTimer对应的计时器和隐藏槽函数
	connect(this->volumeWidTimer, &QTimer::timeout, this, &Show::do_TimeoutHideVolumeWid);
	// 连接btnCloseVideoListTimer对应的计时器和隐藏槽函数
	connect(this->btnCloseVideoListTimer, &QTimer::timeout, this, &Show::do_TimeoutHideBtnVideoList);
	// 连接btnCloseVideoList按钮对应的槽函数
	connect(ui.btnCloseVideoList, &QPushButton::clicked, this, &Show::do_closeBtnVideoListClicked);
}

// 设置音量滑块位置的槽函数
void Show::do_SetVolumeSliderPos(int x)
{
	ui.VolumeWid->move(x - 15, height() - 225);
}

// 隐藏btnCloseVideoList
void Show::do_TimeoutShowVolumeWid(bool flag)
{
	if (flag)
	{
		ui.VolumeWid->show();
		volumeWidTimer->stop();
	}
	else
	{
		volumeWidTimer->start();
	}
}

// 显示btnCloseVideoList
void Show::do_TimeoutHideVolumeWid()
{
	ui.VolumeWid->hide();
}

// 隐藏btnCloseVideoList
void Show::do_TimeoutShowBtnVideoList()
{
	ui.btnCloseVideoList->show();
	btnCloseVideoListTimer->start();
}

// 显示btnCloseVideoList
void Show::do_TimeoutHideBtnVideoList()
{
	ui.btnCloseVideoList->hide();
}

// 关闭视频列表的槽
void Show::do_closeBtnVideoListClicked()
{
	if (videoListCollapse)
	{
		videoListCollapse = false;
		emit sig_videoListShowHide(false);
	}
	else
	{
		videoListCollapse = true;
		emit sig_videoListShowHide(true);
	}
}

