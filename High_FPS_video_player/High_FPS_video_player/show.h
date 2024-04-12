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

	// ͬ��openGL�е�����
	void setOpenGLParameters(double WdividedH);

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
	// �������ڷ��ź�������Ƶ�б��Ƿ���ʾ
	void sig_videoListShowHide(bool flag);
	// �������ڷ��źŴ���
	void sig_enterVolumeWid(bool flag);
	// �������ڷ��źŴ���
	void sig_enterBtnCloseVideoList(bool flag);

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
	// ��������������Ƶ�б�رհ�ť
	void do_hideBtnCloseVideoList();
	// ͨ������ı�����
	void do_valueChanged(int value);

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
	// ����BtnCloseVideoList
	bool enterBtnCloseVideoList = false;

private:
	Ui::ShowClass ui;
};
