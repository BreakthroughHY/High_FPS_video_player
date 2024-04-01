#pragma once

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QFontDatabase>
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

private:
	// ����ͼ��
	QFont fontIcon;

private:
	Ui::CtrBarClass ui;
};
