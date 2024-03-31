#include "title.h"

Title::Title(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	// 样式
	setStyle();

	// 连接信号和槽
	connectSignalSlots();
}

Title::~Title()
{}

// 绘制虚函数实现
void Title::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.fillRect(event->rect(), QColor(30, 32, 34)); // 设置背景颜色为浅灰色
}

// 鼠标按下虚函数实现
void Title::mousePressEvent(QMouseEvent* event)
{
	//qDebug() << 1;
	if (event->button() == Qt::LeftButton && event->pos().y() > 4) // 顶部y值在0到4是控制窗口缩放的区域不执行窗口移动
	{
		m_moving = true;
		// 记录鼠标点击的位置
		m_lastPos = event->pos();
	}
	return QWidget::mousePressEvent(event);
}

// 鼠标抬起虚函数实现
void Title::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_moving = false;
		//event->accept();
	}
	// 向主窗口发射信号（处理通过窗口上边框调整大小时抬起鼠标触发主窗口的mouseReleaseEvent）
	if (event->button() == Qt::LeftButton)
	{
		// 向主窗口发射信号触发主窗口鼠标抬起事件
		emit sig_borderExtension(event); 
	}

	return QWidget::mouseReleaseEvent(event);
}

// 鼠标移动虚函数实现
void Title::mouseMoveEvent(QMouseEvent* event)
{
	if (m_moving)
	{
		QPoint targetPos = event->globalPosition().toPoint() - m_lastPos;
		
		if (maximize) // 在最大化的情况下移动窗口会自动结束最大化
		{
			maximize = false; // 结束最大化
			m_lastPos.setX(m_lastPos.x() * NonMaximizeWidth / this->width());
			emit sig_ResMaxBtnClicked(); // 发射结束最大化的信号
		}
		// 移动操作
		emit sig_MoveWindow(targetPos);
	}
	return QWidget::mouseMoveEvent(event);
}


// 样式
void Title::setStyle()
{
	ui.logo->setStyleSheet("background-color: rgb(30, 32, 34);color: rgb(78, 194, 145); ");
	ui.logo->setEnabled(false);
	ui.logo->setFlat(true);
	QFont font = ui.logo->font();
	font.setBold(true);
	ui.logo->setFont(font);

	ui.videoName->setStyleSheet("background-color: rgb(30, 32, 34);\
													  color: rgb(255, 255, 255);");
	ui.btnClose->setStyleSheet(btnLeave);
	ui.btnMax->setStyleSheet(btnLeave);
	ui.btnMin->setStyleSheet(btnLeave);


	// 委托事件
	ui.btnClose->installEventFilter(this);
	ui.btnMax->installEventFilter(this);
	ui.btnMin->installEventFilter(this);

	ui.btnClose->setFlat(true);
}


bool Title::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == ui.btnClose)
    {
        if (event->type() == QEvent::Enter)
        {
            ui.btnClose->setStyleSheet(btnEnter);
        }
        else if (event->type() == QEvent::Leave)
        {
            ui.btnClose->setStyleSheet(btnLeave);
        }
    }
    else if (watched == ui.btnMax)
	{
		if (event->type() == QEvent::Enter)
		{
			ui.btnMax->setStyleSheet(btnEnter);
		}
		else if (event->type() == QEvent::Leave)
		{
			ui.btnMax->setStyleSheet(btnLeave);
		}
	}
	else if (watched == ui.btnMin)
	{
		if (event->type() == QEvent::Enter)
		{
			ui.btnMin->setStyleSheet(btnEnter);
		}
		else if (event->type() == QEvent::Leave)
		{
			ui.btnMin->setStyleSheet(btnLeave);
		}
	}

    return QWidget::eventFilter(watched, event);
}


// 连接信号和槽
void Title::connectSignalSlots()
{
	connect(ui.btnClose, &QPushButton::clicked, this, &Title::do_closeBtnClicked);
	connect(ui.btnMax, &QPushButton::clicked, this, &Title::do_maxBtnClicked);
	connect(ui.btnMin, &QPushButton::clicked, this, &Title::do_minBtnClicked);
}

// 关闭按钮对应的槽
void Title::do_closeBtnClicked()
{
	emit sig_CloseBtnClicked();
}

// 最大化按钮对应的槽
void Title::do_maxBtnClicked()
{
	if (maximize)
	{
		maximize = false;
		emit sig_ResMaxBtnClicked();
	}
	else
	{
		maximize = true;
		NonMaximizeWidth = this->width();
		emit sig_MaxBtnClicked();
	}
}
// 最小化按钮对应的槽
void Title::do_minBtnClicked()
{
	emit sig_MinBtnClicked();
}
