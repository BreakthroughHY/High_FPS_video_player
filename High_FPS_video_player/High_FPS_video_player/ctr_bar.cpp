#include "ctr_bar.h"

CtrBar::CtrBar(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	// 设置样式
	setStyle();
	// 连接信号和槽
	connectSignalSlots();

	// 音频控件进行事件过滤器
	ui.VolumeBtn->installEventFilter(this);
}

CtrBar::~CtrBar()
{}

// 绘制虚函数实现
void CtrBar::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	//painter.fillRect(event->rect(), QColor(0, 100, 0, 128)); // 设置背景颜色为浅灰色

	// 创建线性渐变对象
	QLinearGradient gradient(0, 0, 0, height());
	gradient.setColorAt(0, QColor(255, 255, 255, 0)); // 起始颜色
	gradient.setColorAt(1, QColor(0, 0, 0, 128)); // 结束颜色

	// 使用渐变颜色填充矩形
	painter.fillRect(event->rect(), gradient);
}

// 窗口位置改变的虚函数
void CtrBar::moveEvent(QMoveEvent* event)
{
	// 向show发信号设置音量滑块位置
	emit sig_SetVolumeSliderPos(ui.VolumeBtn->pos().x());
}

// 大小改变虚函数
void CtrBar::resizeEvent(QResizeEvent* event)
{
	// 向show发信号设置音量滑块位置
	emit sig_SetVolumeSliderPos(ui.VolumeBtn->pos().x());
}

// 事件过滤器虚函数
bool CtrBar::eventFilter(QObject* obj, QEvent* event)
{
	if (obj == ui.VolumeBtn)
	{
		if (event->type() == QEvent::Enter)
		{
			emit sig_SetVolumeSliderShowHide(true);
		}
		else if (event->type() == QEvent::Leave)
		{
			emit sig_SetVolumeSliderShowHide(false);
		}
	}

	return QWidget::eventFilter(obj, event);
}

void CtrBar::setStyle()
{
	// 加载自定义字体文件
	int fontId = QFontDatabase::addApplicationFont(":/iconFont/font/iconfont.ttf");
	QString fontName = QFontDatabase::applicationFontFamilies(fontId).at(0);
	fontIcon.setFamily(fontName);

	ui.PlayOrPauseBtn->setFont(fontIcon);
	ui.PlayOrPauseBtn->setText(QChar(0xe570));
	ui.VolumeBtn->setFont(fontIcon);
	ui.VolumeBtn->setText(QChar(0xe858));
	ui.FullScreenBtn->setFont(fontIcon);
	ui.FullScreenBtn->setText(QChar(0xe659));
}

// 连接信号和槽
void CtrBar::connectSignalSlots()
{
	// 连接播放和暂停按钮对应的槽
	connect(ui.PlayOrPauseBtn, &QPushButton::clicked, this, &CtrBar::do_PlayOrPauseBtnClicked);
	// 连接全屏按钮与相应的槽函数
	connect(ui.FullScreenBtn, &QPushButton::clicked, this, &CtrBar::do_FullScreenBtnClicked);
}

// 播放和暂停按钮对应的槽
void CtrBar::do_PlayOrPauseBtnClicked()
{
	if (playing)
	{
		playing = false;
		ui.PlayOrPauseBtn->setText(QChar(0xe570));
	}
	else
	{
		playing = true;
		ui.PlayOrPauseBtn->setText(QChar(0xe694));
	}
}

// 处理全屏按钮
void CtrBar::do_FullScreenBtnClicked()
{
	if (sig_FullScreen)
	{
		sig_FullScreen = false;
		ui.FullScreenBtn->setText(QChar(0xe659));
		emit sig_fullScreen(sig_FullScreen);
	}
	else
	{
		sig_FullScreen = true;
		ui.FullScreenBtn->setText(QChar(0xe65a));
		emit sig_fullScreen(sig_FullScreen);
	}
}
