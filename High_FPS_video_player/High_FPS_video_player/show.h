#pragma once

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include "ui_show.h"


class Show : public QWidget
{
	Q_OBJECT

public:
	Show(QWidget *parent = nullptr);
	~Show();

protected:
	// 绘制虚函数实现
	void paintEvent(QPaintEvent* event) override;
	// 大小改变虚函数
	void resizeEvent(QResizeEvent* event) override;

private:
	// openGL窗口比例   临时的
	double WdividedH = 1.777;


private:
	Ui::ShowClass ui;
};
