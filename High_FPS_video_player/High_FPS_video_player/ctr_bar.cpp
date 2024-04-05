#include "ctr_bar.h"

CtrBar::CtrBar(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	// ������ʽ
	setStyle();
	// �����źźͲ�
	connectSignalSlots();

	// ��Ƶ�ؼ������¼�������
	ui.VolumeBtn->installEventFilter(this);
}

CtrBar::~CtrBar()
{}

// �����麯��ʵ��
void CtrBar::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	//painter.fillRect(event->rect(), QColor(0, 100, 0, 128)); // ���ñ�����ɫΪǳ��ɫ

	// �������Խ������
	QLinearGradient gradient(0, 0, 0, height());
	gradient.setColorAt(0, QColor(255, 255, 255, 0)); // ��ʼ��ɫ
	gradient.setColorAt(1, QColor(0, 0, 0, 128)); // ������ɫ

	// ʹ�ý�����ɫ������
	painter.fillRect(event->rect(), gradient);
}

// ����λ�øı���麯��
void CtrBar::moveEvent(QMoveEvent* event)
{
	// ��show���ź�������������λ��
	emit sig_SetVolumeSliderPos(ui.VolumeBtn->pos().x());
}

// ��С�ı��麯��
void CtrBar::resizeEvent(QResizeEvent* event)
{
	// ��show���ź�������������λ��
	emit sig_SetVolumeSliderPos(ui.VolumeBtn->pos().x());
}

// �¼��������麯��
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
	// �����Զ��������ļ�
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

// �����źźͲ�
void CtrBar::connectSignalSlots()
{
	// ���Ӳ��ź���ͣ��ť��Ӧ�Ĳ�
	connect(ui.PlayOrPauseBtn, &QPushButton::clicked, this, &CtrBar::do_PlayOrPauseBtnClicked);
	// ����ȫ����ť����Ӧ�Ĳۺ���
	connect(ui.FullScreenBtn, &QPushButton::clicked, this, &CtrBar::do_FullScreenBtnClicked);
}

// ���ź���ͣ��ť��Ӧ�Ĳ�
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

// ����ȫ����ť
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
