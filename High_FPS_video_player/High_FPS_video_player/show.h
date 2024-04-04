#pragma once

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QSlider>
#include <QFontDatabase>
#include <QTimer>
#include "ui_show.h"


class Show : public QWidget
{
	Q_OBJECT

public:
	Show(QWidget *parent = nullptr);
	~Show();

protected:
	// �����麯��ʵ��
	void paintEvent(QPaintEvent* event) override;
	// ��С�ı��麯��
	void resizeEvent(QResizeEvent* event) override;
	// ����ƶ��麯��ʵ��
	void mouseMoveEvent(QMouseEvent* event) override;
	// �¼��������麯��
	bool eventFilter(QObject* obj, QEvent* event);

private:
	// ��ʽ
	void setStyle();
	// �����źźͲ�
	void connectSignalSlots();

signals:
	// �������ڷ��ź�����CtrlBarWidλ��
	void sig_setCtrlBarWidPos();
	// 
	void sig_videoListShowHide(bool flag);

public slots:
	// ������������λ�õĲۺ���
	void do_SetVolumeSliderPos(int x);
	// ����VolumeWid
	void do_TimeoutShowVolumeWid(bool flag);
	// ��ʾVolumeWid
	void do_TimeoutHideVolumeWid();
	// ��ʾbtnCloseVideoList
	void do_TimeoutShowBtnVideoList();
	// ����btnCloseVideoList
	void do_TimeoutHideBtnVideoList();
	// �ر���Ƶ�б�Ĳ�
	void do_closeBtnVideoListClicked();

private:
	// openGL���ڱ���   ��ʱ��
	double WdividedH = 1.777;
	// ����ͼ��
	QFont fontIcon;
	// ��ʱ������ָ��
	QTimer* volumeWidTimer;
	QTimer* btnCloseVideoListTimer;
	// ��Ƶ�б��Ƿ�����
	bool videoListCollapse = false;

private:
	Ui::ShowClass ui;
};
