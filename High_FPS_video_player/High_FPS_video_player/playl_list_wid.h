#pragma once

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QFileDialog>
#include "ui_playl_list_wid.h"

class PlaylListWid : public QWidget
{
	Q_OBJECT

public:
	PlaylListWid(QWidget *parent = nullptr);
	~PlaylListWid();

protected:
	// 绘制虚函数实现
	void paintEvent(QPaintEvent* event) override;

private:
	// 样式
	void setStyle();
	// 连接信号和槽
	void connectSignalSlots();

public slots:
	// 当前显示的视频列表的切换
	void do_btnLocal();
	// 当前显示的视频列表的切换
	void do_btnNetwork();
	// 处理添加视频按钮的槽函数
	void do_btnAddClicked();

private:
	// 当前显示的是否为本地的视频列表
	bool isLoca = true;
	// 本地还是网络被选中的样式
	QString selectStyle = "padding-bottom: -5px;\
							border-bottom: 3px solid rgb(78, 194, 145);\
							color: rgb(78, 194, 145); ";

private:
	Ui::PlaylListWidClass ui;
};
