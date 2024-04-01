#include "ctr_bar.h"

CtrBar::CtrBar(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	// 设置样式
	setStyle();
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
	gradient.setColorAt(1, QColor(85, 85, 85, 128)); // 结束颜色

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

	ui.VideoPlayTimeTimeEdit->setStyleSheet(tempStr);

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

	ui.FullScreenBtn->setStyleSheet(tempStr);

	ui.PlayOrPauseBtn->setFont(fontIcon);
	ui.PlayOrPauseBtn->setText(QChar(0xe570));
	ui.VolumeBtn->setFont(fontIcon);
	ui.VolumeBtn->setText(QChar(0xe858));
	ui.FullScreenBtn->setFont(fontIcon);
	ui.FullScreenBtn->setText(QChar(0xe659));

	ui.PlayOrPauseBtn->setStyleSheet("QPushButton { color: black; selection-background-color: transparent;border: none;}"
		"QPushButton:hover { color: red; }");
}
