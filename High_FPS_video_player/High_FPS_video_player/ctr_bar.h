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
	// ����λ�øı���麯��
	void moveEvent(QMoveEvent* event) override;
	// ��С�ı��麯��
	void resizeEvent(QResizeEvent* event) override;
	// �¼��������麯��
	bool eventFilter(QObject* obj, QEvent* event);

private:
	// ��ʽ
	void setStyle();
	// �����źźͲ�
	void connectSignalSlots();

signals:
	// ������������λ�õ��ź�
	void sig_SetVolumeSliderPos(int x);
	// ��������������ʾ�����ź�
	void sig_SetVolumeSliderShowHide(bool flag);
	// ȫ���ź�
	void sig_fullScreen(bool flag);

public slots:
	// ���ź���ͣ��ť��Ӧ�Ĳ�
	void do_PlayOrPauseBtnClicked();
	// ����ȫ����ť
	void do_FullScreenBtnClicked();
	// ������Ƶ��ʱ��
	void do_SetVideoTotalTimeTimeEdit(int second);

private:
	// ����ͼ��
	QFont fontIcon;
	// ��¼��Ƶ�Ƿ񲥷���
	bool playing = false;
	// ��ȫ��
	bool sig_FullScreen = false;

private:
	Ui::CtrBarClass ui;
};
