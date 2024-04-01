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
	// 绘制虚函数实现
	void paintEvent(QPaintEvent* event) override;

private:
	// 样式
	void setStyle();

private:
	// 字体图标
	QFont fontIcon;

private:
	Ui::CtrBarClass ui;
};
