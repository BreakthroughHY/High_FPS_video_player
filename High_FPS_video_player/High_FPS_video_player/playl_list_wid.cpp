#include "playl_list_wid.h"

PlaylListWid::PlaylListWid(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	// 样式
	setStyle();

	// 连接信号和槽
	connectSignalSlots();

	ui.NetworkVideoListWid->hide();
	ui.LocalVideoListWid->clear();
}

PlaylListWid::~PlaylListWid()
{}

// 绘制虚函数实现
void PlaylListWid::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.fillRect(event->rect(), QColor(23, 24, 27)); // 设置背景颜色为浅灰色
}

// 样式
void PlaylListWid::setStyle()
{
	ui.btnLocal->setStyleSheet(selectStyle);
	// 禁用水平滑杆
	ui.LocalVideoListWid->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

// 连接信号和槽
void PlaylListWid::connectSignalSlots()
{
	connect(ui.btnLocal, &QPushButton::clicked, this, &PlaylListWid::do_btnLocal);
	connect(ui.btnNetwork, &QPushButton::clicked, this, &PlaylListWid::do_btnNetwork);
	connect(ui.btnAdd, &QPushButton::clicked, this, &PlaylListWid::do_btnAddClicked);
}

// 当前显示的视频列表的切换
void PlaylListWid::do_btnLocal()
{
	if (!isLoca)
	{
		isLoca = true;
		ui.btnLocal->setStyleSheet(selectStyle);
		ui.btnNetwork->setStyleSheet("");
		ui.LocalVideoListWid->show();
		ui.NetworkVideoListWid->hide();
	}
}

// 当前显示的视频列表的切换
void PlaylListWid::do_btnNetwork()
{
	if (isLoca)
	{
		isLoca = false;
		ui.btnLocal->setStyleSheet("");
		ui.btnNetwork->setStyleSheet(selectStyle);
		ui.LocalVideoListWid->hide();
		ui.NetworkVideoListWid->show();
	}
}

// 处理添加视频按钮的槽函数
void PlaylListWid::do_btnAddClicked()
{
	if (isLoca) // 添加本地文件
	{
		QString curPath = QCoreApplication::applicationDirPath();

		QStringList fileNames = QFileDialog::getOpenFileNames(this, "选择多个文件", curPath,
			"文本文件(*.mp4);;图片(*.jpg *.png *.gif);;所有文件(*.*)");

		QListWidgetItem* item;

		for (auto ele : fileNames)
		{
			QFileInfo fileInfo(ele);
			item =  new QListWidgetItem(fileInfo.fileName());
			item->setData(Qt::UserRole, QVariant(ele));  // 存储视频路径

			qDebug() << item->data(Qt::UserRole).toString();

			ui.LocalVideoListWid->addItem(item);
		}

	}
}

