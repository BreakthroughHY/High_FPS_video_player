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

	// 
	ui.VolumeWid->installEventFilter(this);
	ui.btnCloseVideoList->installEventFilter(this);

	ui.VolumeWid->hide();
	ui.btnCloseVideoList->hide();
}

Show::~Show()
{}

// 同步openGL中的数据
void Show::setOpenGLParameters(double WdividedH)
{
	this->WdividedH = WdividedH;
	this->ui.openGLWidget->setParameters();

	// 设置openGL组件在Show中的位置
	setOpenGLInShowPos();
}

// 绘制虚函数实现
void Show::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.fillRect(event->rect(), QColor(0, 0, 0));
}

// 大小改变虚函数
void Show::resizeEvent(QResizeEvent* event)
{
	// 设置openGL组件在Show中的位置
	setOpenGLInShowPos();
	
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
	// 处理音量滑块窗口事件
	if (obj == ui.VolumeWid)
	{
		// 鼠标进入音量滑块窗口
		if (event->type() == QEvent::Enter)
		{
			// 音量滑块窗口消失的计时器如果开启就关闭它
			if (volumeWidTimer->isActive())
			{
				volumeWidTimer->stop();
			}
			// 向主窗口发信号传输
			sig_enterVolumeWid(true);
		}
		// 鼠标离开音量滑块窗口
		else if (event->type() == QEvent::Leave)
		{
			// 音量滑块窗口消失的计时器如果没有开启就开启它
			if (!volumeWidTimer->isActive())
			{
				volumeWidTimer->start();
			}
			// 向主窗口发信号传输
			sig_enterVolumeWid(false);
		}
	}
	// 处理关闭视频列表按钮事件
	else if (obj == ui.btnCloseVideoList)
	{
		// 鼠标进入关闭视频列表按钮
		if (event->type() == QEvent::Enter)
		{
			// 向主窗口发送是否进入BtnCloseVideoList按钮
			emit sig_enterBtnCloseVideoList(true);
			enterBtnCloseVideoList = true;
			btnCloseVideoListTimer->stop();
		}
		// 鼠标离开关闭视频列表按钮
		else if (event->type() == QEvent::Leave)
		{
			// 向主窗口发送是否离开BtnCloseVideoList按钮
			emit sig_enterBtnCloseVideoList(false);
			enterBtnCloseVideoList = false;
			btnCloseVideoListTimer->start();
		}
	}

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
	// 通过滑块改变音量
	connect(ui.VolumeSlider, &QSlider::valueChanged, this, &Show::do_valueChanged);
}

// 设置openGL组件在Show中的位置
void Show::setOpenGLInShowPos()
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
	if (!enterBtnCloseVideoList)
	{
		ui.btnCloseVideoList->show();
		btnCloseVideoListTimer->start();
	}
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

// 处理立刻隐藏视频列表关闭按钮
void Show::do_hideBtnCloseVideoList()
{
	if (!btnCloseVideoListTimer->isActive())
		btnCloseVideoListTimer->stop();

	ui.btnCloseVideoList->hide();
}

// 通过滑块改变音量
void Show::do_valueChanged(int value)
{
	ui.VolumeLabel->setText(QString::number(value) + "%");
}