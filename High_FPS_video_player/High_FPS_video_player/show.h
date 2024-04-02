#pragma once

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QSlider>
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
	// 鼠标移动虚函数实现
	void mouseMoveEvent(QMouseEvent* event) override;

signals:
	// 向主窗口发信号设置CtrlBarWid位置
	void sig_setCtrlBarWidPos();

private:
	// openGL窗口比例   临时的
	double WdividedH = 1.777;


private:
	Ui::ShowClass ui;
};
