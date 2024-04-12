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

	// 同步openGL中的数据
	void setOpenGLParameters(double WdividedH);

protected:
	// 绘制虚函数实现
	void paintEvent(QPaintEvent* event) override;
	// 大小改变虚函数
	void resizeEvent(QResizeEvent* event) override;
	// 鼠标移动虚函数实现
	void mouseMoveEvent(QMouseEvent* event) override;
	// 事件过滤器虚函数
	bool eventFilter(QObject* obj, QEvent* event);

private:
	// 样式
	void setStyle();
	// 连接信号和槽
	void connectSignalSlots();

signals:
	// 向主窗口发信号设置CtrlBarWid位置
	void sig_setCtrlBarWidPos();
	// 向主窗口发信号设置视频列表是否显示
	void sig_videoListShowHide(bool flag);
	// 向主窗口发信号传输
	void sig_enterVolumeWid(bool flag);
	// 向主窗口发信号传输
	void sig_enterBtnCloseVideoList(bool flag);

public slots:
	// 设置音量滑块位置的槽函数
	void do_SetVolumeSliderPos(int x);
	// 隐藏VolumeWid
	void do_TimeoutShowVolumeWid(bool flag);
	// 显示VolumeWid
	void do_TimeoutHideVolumeWid();
	// 显示btnCloseVideoList
	void do_TimeoutShowBtnVideoList();
	// 隐藏btnCloseVideoList
	void do_TimeoutHideBtnVideoList();
	// 关闭视频列表的槽
	void do_closeBtnVideoListClicked();
	// 处理立刻隐藏视频列表关闭按钮
	void do_hideBtnCloseVideoList();
	// 通过滑块改变音量
	void do_valueChanged(int value);

private:
	// openGL窗口比例   临时的
	double WdividedH = 1.777;
	// 字体图标
	QFont fontIcon;
	// 定时器对象指针
	QTimer* volumeWidTimer;
	QTimer* btnCloseVideoListTimer;
	// 视频列表是否被收起
	bool videoListCollapse = false;
	// 进入BtnCloseVideoList
	bool enterBtnCloseVideoList = false;

private:
	Ui::ShowClass ui;
};
