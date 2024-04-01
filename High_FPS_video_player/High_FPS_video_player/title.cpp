#include "title.h"

Title::Title(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	// ��ʽ
	setStyle();

	// �����źźͲ�
	connectSignalSlots();
}

Title::~Title()
{}

// ��ȡ������󻯵�״̬
bool Title::getMaximize() { return this->maximize; };

// �����麯��ʵ��
void Title::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.fillRect(event->rect(), QColor(30, 32, 34)); // ���ñ�����ɫΪǳ��ɫ
}

// ��갴���麯��ʵ��
void Title::mousePressEvent(QMouseEvent* event)
{
	//qDebug() << 1;
	if (event->button() == Qt::LeftButton && event->pos().y() > 4) // ����yֵ��0��4�ǿ��ƴ������ŵ�����ִ�д����ƶ�
	{
		m_moving = true;
		// ��¼�������λ��
		m_lastPos = event->pos();
	}
	return QWidget::mousePressEvent(event);
}

// ���̧���麯��ʵ��
void Title::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_moving = false;
		//event->accept();
	}
	// �������ڷ����źţ�����ͨ�������ϱ߿������Сʱ̧����괥�������ڵ�mouseReleaseEvent��
	if (event->button() == Qt::LeftButton)
	{
		// �������ڷ����źŴ������������̧���¼�
		emit sig_borderExtension(event); 
	}

	return QWidget::mouseReleaseEvent(event);
}

// ����ƶ��麯��ʵ��
void Title::mouseMoveEvent(QMouseEvent* event)
{
	if (m_moving)
	{
		QPoint targetPos = event->globalPosition().toPoint() - m_lastPos;
		
		if (maximize) // ����󻯵�������ƶ����ڻ��Զ��������
		{
			maximize = false; // �������
			m_lastPos.setX(m_lastPos.x() * NonMaximizeWidth / this->width());
			emit sig_ResMaxBtnClicked(); // ���������󻯵��ź�
			ui.btnMax->setText(QChar(0xe65d));
		}
		// �ƶ�����
		emit sig_MoveWindow(targetPos);
	}
	return QWidget::mouseMoveEvent(event);
}


// ��ʽ
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
	ui.btnClose->setStyleSheet(btnLeave + "font-size: 23px;");
	ui.btnMax->setStyleSheet(btnLeave + "font-size: 16px;");
	ui.btnMin->setStyleSheet(btnLeave + "font-size: 15px;");


	// ί���¼�
	ui.btnClose->installEventFilter(this);
	ui.btnMax->installEventFilter(this);
	ui.btnMin->installEventFilter(this);

	//ui.btnClose->setFlat(true);

	// �����Զ��������ļ�
	int fontId = QFontDatabase::addApplicationFont(":/iconFont/font/iconfont.ttf");
	QString fontName = QFontDatabase::applicationFontFamilies(fontId).at(0);
	fontIcon.setFamily(fontName);
	//fontIcon.setPointSize(9);
	/*ui.videoName->setFont(fontIcon);
	ui.videoName->setText(QChar(0xe570));*/
	ui.btnMin->setFont(fontIcon);
	ui.btnMin->setText(QChar(0xe956));
	ui.btnMax->setFont(fontIcon);
	ui.btnMax->setText(QChar(0xe65d));
	ui.btnClose->setFont(fontIcon);
	ui.btnClose->setText(QChar(0xe611));
}


bool Title::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == ui.btnClose)
    {
        if (event->type() == QEvent::Enter)
        {
            ui.btnClose->setStyleSheet(btnEnter + "font-size: 23px;");
        }
        else if (event->type() == QEvent::Leave)
        {
            ui.btnClose->setStyleSheet(btnLeave + "font-size: 23px;");
        }
    }
    else if (watched == ui.btnMax)
	{
		if (event->type() == QEvent::Enter)
		{
			ui.btnMax->setStyleSheet(btnEnter + "font-size: 16px;");
		}
		else if (event->type() == QEvent::Leave)
		{
			ui.btnMax->setStyleSheet(btnLeave + "font-size: 16px;");
		}
	}
	else if (watched == ui.btnMin)
	{
		if (event->type() == QEvent::Enter)
		{
			ui.btnMin->setStyleSheet(btnEnter + "font-size: 15px;");
		}
		else if (event->type() == QEvent::Leave)
		{
			ui.btnMin->setStyleSheet(btnLeave + "font-size: 15px;");
		}
	}

    return QWidget::eventFilter(watched, event);
}


// �����źźͲ�
void Title::connectSignalSlots()
{
	connect(ui.btnClose, &QPushButton::clicked, this, &Title::do_closeBtnClicked);
	connect(ui.btnMax, &QPushButton::clicked, this, &Title::do_maxBtnClicked);
	connect(ui.btnMin, &QPushButton::clicked, this, &Title::do_minBtnClicked);
}

// �رհ�ť��Ӧ�Ĳ�
void Title::do_closeBtnClicked()
{
	emit sig_CloseBtnClicked();
}

// ��󻯰�ť��Ӧ�Ĳ�
void Title::do_maxBtnClicked()
{
	if (maximize)
	{
		maximize = false;
		emit sig_ResMaxBtnClicked();
		ui.btnMax->setText(QChar(0xe65d));
	}
	else
	{
		maximize = true;
		NonMaximizeWidth = this->width();
		emit sig_MaxBtnClicked();
		ui.btnMax->setText(QChar(0xe692));
	}
}
// ��С����ť��Ӧ�Ĳ�
void Title::do_minBtnClicked()
{
	emit sig_MinBtnClicked();
}
