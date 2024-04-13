#include "show.h"

Show::Show(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	// ��ʽ
	setStyle();

	// ��ʱ������ָ��  (Ҫ�����Ӳۺ���֮ǰ������ʱ������)
	volumeWidTimer = new QTimer(this);
	btnCloseVideoListTimer = new QTimer(this);
	// ��ʱ����
	volumeWidTimer->setTimerType(Qt::PreciseTimer);
	btnCloseVideoListTimer->setTimerType(Qt::PreciseTimer);
	// ��ʱ���
	volumeWidTimer->setInterval(500);
	btnCloseVideoListTimer->setInterval(1000);
	// ���õ��μ�ʱ
	volumeWidTimer->setSingleShot(true);
	btnCloseVideoListTimer->setSingleShot(true);

	// �����źźͲ�
	connectSignalSlots();

	// 
	ui.VolumeWid->installEventFilter(this);
	ui.btnCloseVideoList->installEventFilter(this);

	ui.VolumeWid->hide();
	ui.btnCloseVideoList->hide();
}

Show::~Show()
{}

// ͬ��openGL�е�����
void Show::setOpenGLParameters(double WdividedH)
{
	this->WdividedH = WdividedH;
	this->ui.openGLWidget->setParameters();

	// ����openGL�����Show�е�λ��
	setOpenGLInShowPos();
}

// �����麯��ʵ��
void Show::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.fillRect(event->rect(), QColor(0, 0, 0));
}

// ��С�ı��麯��
void Show::resizeEvent(QResizeEvent* event)
{
	// ����openGL�����Show�е�λ��
	setOpenGLInShowPos();
	
	// ����������Ƶ�б�ťλ��
	ui.btnCloseVideoList->move(width() - ui.btnCloseVideoList->width(), (height() - ui.btnCloseVideoList->height()) / 2);

	// �������ڷ��ź�����CtrlBarWidλ��
	emit sig_setCtrlBarWidPos();
}

// ����ƶ��麯��ʵ��
void Show::mouseMoveEvent(QMouseEvent* event)
{
}

// �¼��������麯��
bool Show::eventFilter(QObject* obj, QEvent* event)
{
	// �����������鴰���¼�
	if (obj == ui.VolumeWid)
	{
		// �������������鴰��
		if (event->type() == QEvent::Enter)
		{
			// �������鴰����ʧ�ļ�ʱ����������͹ر���
			if (volumeWidTimer->isActive())
			{
				volumeWidTimer->stop();
			}
			// �������ڷ��źŴ���
			sig_enterVolumeWid(true);
		}
		// ����뿪�������鴰��
		else if (event->type() == QEvent::Leave)
		{
			// �������鴰����ʧ�ļ�ʱ�����û�п����Ϳ�����
			if (!volumeWidTimer->isActive())
			{
				volumeWidTimer->start();
			}
			// �������ڷ��źŴ���
			sig_enterVolumeWid(false);
		}
	}
	// ����ر���Ƶ�б�ť�¼�
	else if (obj == ui.btnCloseVideoList)
	{
		// ������ر���Ƶ�б�ť
		if (event->type() == QEvent::Enter)
		{
			// �������ڷ����Ƿ����BtnCloseVideoList��ť
			emit sig_enterBtnCloseVideoList(true);
			enterBtnCloseVideoList = true;
			btnCloseVideoListTimer->stop();
		}
		// ����뿪�ر���Ƶ�б�ť
		else if (event->type() == QEvent::Leave)
		{
			// �������ڷ����Ƿ��뿪BtnCloseVideoList��ť
			emit sig_enterBtnCloseVideoList(false);
			enterBtnCloseVideoList = false;
			btnCloseVideoListTimer->start();
		}
	}

	return QWidget::eventFilter(obj, event);
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

// �����źźͲ�
void Show::connectSignalSlots()
{
	// ����volumeWidTimer��Ӧ�ļ�ʱ�������زۺ���
	connect(this->volumeWidTimer, &QTimer::timeout, this, &Show::do_TimeoutHideVolumeWid);
	// ����btnCloseVideoListTimer��Ӧ�ļ�ʱ�������زۺ���
	connect(this->btnCloseVideoListTimer, &QTimer::timeout, this, &Show::do_TimeoutHideBtnVideoList);
	// ����btnCloseVideoList��ť��Ӧ�Ĳۺ���
	connect(ui.btnCloseVideoList, &QPushButton::clicked, this, &Show::do_closeBtnVideoListClicked);
	// ͨ������ı�����
	connect(ui.VolumeSlider, &QSlider::valueChanged, this, &Show::do_valueChanged);
}

// ����openGL�����Show�е�λ��
void Show::setOpenGLInShowPos()
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
}

// ������������λ�õĲۺ���
void Show::do_SetVolumeSliderPos(int x)
{
	ui.VolumeWid->move(x - 15, height() - 225);
}

// ����btnCloseVideoList
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

// ��ʾbtnCloseVideoList
void Show::do_TimeoutHideVolumeWid()
{
	ui.VolumeWid->hide();
}

// ����btnCloseVideoList
void Show::do_TimeoutShowBtnVideoList()
{
	if (!enterBtnCloseVideoList)
	{
		ui.btnCloseVideoList->show();
		btnCloseVideoListTimer->start();
	}
}

// ��ʾbtnCloseVideoList
void Show::do_TimeoutHideBtnVideoList()
{
	ui.btnCloseVideoList->hide();
}

// �ر���Ƶ�б�Ĳ�
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

// ��������������Ƶ�б�رհ�ť
void Show::do_hideBtnCloseVideoList()
{
	if (!btnCloseVideoListTimer->isActive())
		btnCloseVideoListTimer->stop();

	ui.btnCloseVideoList->hide();
}

// ͨ������ı�����
void Show::do_valueChanged(int value)
{
	ui.VolumeLabel->setText(QString::number(value) + "%");
}