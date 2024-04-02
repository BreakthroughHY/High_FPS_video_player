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
	// 绘制虚函数实现
	void paintEvent(QPaintEvent* event) override;

private:
	// 样式
	void setStyle();
	// 连接信号和槽
	void connectSignalSlots();

public slots:
	// 播放和暂停按钮对应的槽
	void do_PlayOrPauseBtnClicked();

private:
	// 字体图标
	QFont fontIcon;
	// 记录视频是否播放中
	bool playing = false;

private:
	Ui::CtrBarClass ui;
};
