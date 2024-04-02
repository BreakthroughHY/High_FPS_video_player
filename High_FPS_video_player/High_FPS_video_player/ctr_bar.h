#pragma once

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QFontDatabase>
#include "helper.h"
#include "ui_ctr_bar.h"

class CtrBar : public QWidget
{
	Q_OBJECT

public:
	CtrBar(QWidget *parent = nullptr);
	~CtrBar();

protected:
	// �����麯��ʵ��
	void paintEvent(QPaintEvent* event) override;

private:
	// ��ʽ
	void setStyle();
	// �����źźͲ�
	void connectSignalSlots();

public slots:
	// ���ź���ͣ��ť��Ӧ�Ĳ�
	void do_PlayOrPauseBtnClicked();

private:
	// ����ͼ��
	QFont fontIcon;
	// ��¼��Ƶ�Ƿ񲥷���
	bool playing = false;

private:
	Ui::CtrBarClass ui;
};
