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
	// 绘制虚函数实现
	void paintEvent(QPaintEvent* event) override;
	// 大小改变虚函数
	void resizeEvent(QResizeEvent* event) override;
	// 鼠标移动虚函数实现
	void mouseMoveEvent(QMouseEvent* event) override;

private:
	// 样式
	void setStyle();

signals:
	// 向主窗口发信号设置CtrlBarWid位置
	void sig_setCtrlBarWidPos();
	// 向ctr_bar发射信号通知进行音量滑块位置的设置
	void sig_SetVolumeSliderPos();

public slots:
	// 设置音量滑块位置的槽函数
	void do_SetVolumeSliderPos(int x);

private:
	// openGL窗口比例   临时的
	double WdividedH = 1.777;
	// 字体图标
	QFont fontIcon;

private:
	Ui::ShowClass ui;
};
