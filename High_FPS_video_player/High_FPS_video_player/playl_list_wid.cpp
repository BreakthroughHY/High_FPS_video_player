#include "playl_list_wid.h"

PlaylListWid::PlaylListWid(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	// ��ʽ
	setStyle();

	// �����źźͲ�
	connectSignalSlots();

	ui.NetworkVideoListWid->hide();
	ui.LocalVideoListWid->clear();
}

PlaylListWid::~PlaylListWid()
{}

// �����麯��ʵ��
void PlaylListWid::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.fillRect(event->rect(), QColor(23, 24, 27)); // ���ñ�����ɫΪǳ��ɫ
}

// ��ʽ
void PlaylListWid::setStyle()
{
	ui.btnLocal->setStyleSheet(selectStyle);
	// ����ˮƽ����
	ui.LocalVideoListWid->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

// �����źźͲ�
void PlaylListWid::connectSignalSlots()
{
	connect(ui.btnLocal, &QPushButton::clicked, this, &PlaylListWid::do_btnLocal);
	connect(ui.btnNetwork, &QPushButton::clicked, this, &PlaylListWid::do_btnNetwork);
	connect(ui.btnAdd, &QPushButton::clicked, this, &PlaylListWid::do_btnAddClicked);
}

// ��ǰ��ʾ����Ƶ�б���л�
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

// ��ǰ��ʾ����Ƶ�б���л�
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

// ���������Ƶ��ť�Ĳۺ���
void PlaylListWid::do_btnAddClicked()
{
	if (isLoca) // ��ӱ����ļ�
	{
		QString curPath = QCoreApplication::applicationDirPath();

		QStringList fileNames = QFileDialog::getOpenFileNames(this, "ѡ�����ļ�", curPath,
			"�ı��ļ�(*.mp4);;ͼƬ(*.jpg *.png *.gif);;�����ļ�(*.*)");

		QListWidgetItem* item;

		for (auto ele : fileNames)
		{
			QFileInfo fileInfo(ele);
			item =  new QListWidgetItem(fileInfo.fileName());
			item->setData(Qt::UserRole, QVariant(ele));  // �洢��Ƶ·��

			qDebug() << item->data(Qt::UserRole).toString();

			ui.LocalVideoListWid->addItem(item);
		}

	}
}

