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
	// 窗口位置改变的虚函数
	void moveEvent(QMoveEvent* event) override;
	// 大小改变虚函数
	void resizeEvent(QResizeEvent* event) override;
	// 事件过滤器虚函数
	bool eventFilter(QObject* obj, QEvent* event);

private:
	// 样式
	void setStyle();
	// 连接信号和槽
	void connectSignalSlots();

signals:
	// 设置音量滑块位置的信号
	void sig_SetVolumeSliderPos(int x);
	// 设置音量滑块显示与否的信号
	void sig_SetVolumeSliderShowHide(bool flag);
	// 全屏信号
	void sig_fullScreen(bool flag);

public slots:
	// 播放和暂停按钮对应的槽
	void do_PlayOrPauseBtnClicked();
	// 处理全屏按钮
	void do_FullScreenBtnClicked();
	// 设置视频总时长
	void do_SetVideoTotalTimeTimeEdit(int second);

private:
	// 字体图标
	QFont fontIcon;
	// 记录视频是否播放中
	bool playing = false;
	// 是全屏
	bool sig_FullScreen = false;

private:
	Ui::CtrBarClass ui;
};
