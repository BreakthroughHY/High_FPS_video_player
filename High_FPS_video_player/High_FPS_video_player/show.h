#pragma once

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QSlider>
#include <QFontDatabase>
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

private:
	// ��ʽ
	void setStyle();

signals:
	// �������ڷ��ź�����CtrlBarWidλ��
	void sig_setCtrlBarWidPos();
	// ��ctr_bar�����ź�֪ͨ������������λ�õ�����
	void sig_SetVolumeSliderPos();

public slots:
	// ������������λ�õĲۺ���
	void do_SetVolumeSliderPos(int x);

private:
	// openGL���ڱ���   ��ʱ��
	double WdividedH = 1.777;
	// ����ͼ��
	QFont fontIcon;

private:
	Ui::ShowClass ui;
};
