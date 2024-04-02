#include "ctr_bar.h"

CtrBar::CtrBar(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	// 设置样式
	setStyle();
	// 连接信号和槽
	connectSignalSlots();
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

void CtrBar::setStyle()
{
	// 加载自定义字体文件
	int fontId = QFontDatabase::addApplicationFont(":/iconFont/font/iconfont.ttf");
	QString fontName = QFontDatabase::applicationFontFamilies(fontId).at(0);
	fontIcon.setFamily(fontName);

	QString tempStr = "background-color: rgba(0, 0, 0, 0);\
																			color: rgb(255, 255, 255);\
																			border: none;\
																			selection-background-color: transparent;\
																			selection-color: rgb(255, 255, 255);";

	/*ui.VideoPlayTimeTimeEdit->setStyleSheet(tempStr);

	ui.VideoTotalTimeTimeEdit->setStyleSheet(tempStr);

	ui.label->setStyleSheet("background-color: transparent;\
																			color: rgb(255, 255, 255);\
																			border: none;\
																			selection-background-color: transparent;\
																			selection-color: rgb(255, 255, 255);");

	tempStr = "background-color: transparent;\
																			color: rgb(255, 255, 255);\
																			border: none;";


	ui.PlayOrPauseBtn->setStyleSheet(tempStr);

	ui.VolumeBtn->setStyleSheet(tempStr);

	ui.FullScreenBtn->setStyleSheet(tempStr);*/

	ui.PlayOrPauseBtn->setFont(fontIcon);
	ui.PlayOrPauseBtn->setText(QChar(0xe570));
	ui.VolumeBtn->setFont(fontIcon);
	ui.VolumeBtn->setText(QChar(0xe858));
	ui.FullScreenBtn->setFont(fontIcon);
	ui.FullScreenBtn->setText(QChar(0xe659));

	/*ui.PlayOrPauseBtn->setStyleSheet("QPushButton { color: black; selection-background-color: transparent;border: none;}"
		"QPushButton:hover { color: red; }");*/

	//ui.PlayOrPauseBtn->setStyleSheet(Helper::loadQssStr(":/qss/qss/ctr_bar.css"));
}

// 连接信号和槽
void CtrBar::connectSignalSlots()
{
	// 连接播放和暂停按钮对应的槽
	connect(ui.PlayOrPauseBtn, &QPushButton::clicked, this, &CtrBar::do_PlayOrPauseBtnClicked);
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
