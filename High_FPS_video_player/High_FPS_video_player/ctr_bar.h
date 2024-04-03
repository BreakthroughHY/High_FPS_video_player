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
	// ��С�ı��麯��
	void resizeEvent(QResizeEvent* event) override;

private:
	// ��ʽ
	void setStyle();
	// �����źźͲ�
	void connectSignalSlots();

signals:
	// ������������λ�õ��ź�
	void sig_SetVolumeSliderPos(int x);

public slots:
	// ���ź���ͣ��ť��Ӧ�Ĳ�
	void do_PlayOrPauseBtnClicked();
	// ��������show��������������λ�õ��ź�
	void do_SetVolumeSliderPos();

private:
	// ����ͼ��
	QFont fontIcon;
	// ��¼��Ƶ�Ƿ񲥷���
	bool playing = false;

private:
	Ui::CtrBarClass ui;
};
