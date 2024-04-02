#pragma once

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QFontDatabase>
#include "helper.h"
#include "ui_title.h"


class Title : public QWidget
{
	Q_OBJECT

public:
	Title(QWidget *parent = nullptr);
	~Title();

	// 获取窗口最大化的状态
	bool getMaximize();

protected:
	// 绘制虚函数实现
	void paintEvent(QPaintEvent* event) override;
	// 鼠标按下虚函数实现
	void mousePressEvent(QMouseEvent* event) override;
	// 鼠标抬起虚函数实现
	void mouseReleaseEvent(QMouseEvent* event) override;
	// 鼠标移动虚函数实现
	void mouseMoveEvent(QMouseEvent* event) override;

private:
	// 样式
	void setStyle();
    // 事件过滤  处理委托的任务
	bool eventFilter(QObject* watched, QEvent* event) override;
	// 连接信号和槽
	void connectSignalSlots();


signals:
	// 点击关闭按钮
	void sig_CloseBtnClicked();
	// 点击最小化按钮
	void sig_MinBtnClicked();
	// 点击最大化按钮
	void sig_MaxBtnClicked();
	// 点击最大化按钮    最大化后恢复
	void sig_ResMaxBtnClicked();
	// 鼠标移动信号
	void sig_MoveWindow(QPoint& tempPos);
	// 向主窗口发射信号触发主窗口鼠标抬起事件
	void sig_borderExtension(QMouseEvent* event);

public slots:
	// 关闭按钮对应的槽
	void do_closeBtnClicked();
	// 最大化按钮对应的槽
	void do_maxBtnClicked();
	// 最小化按钮对应的槽
	void do_minBtnClicked();

private:
	// 记录窗口最大化状态
	bool maximize = false;
	// 当前是否在移动状态
	bool m_moving = false;
	// 鼠标单击的位置相对于窗口左上角坐标
	QPoint m_lastPos;
	// 窗口没有最大化时的宽度
	int NonMaximizeWidth;
	// 字体图标
	QFont fontIcon;

	const QString btnEnter = "background-color: rgb(47, 49, 52);\
												color: rgb(255, 255, 255);\
												text-align: center;\
												line-height: 32px;\
												border-radius: 5px;";

	const QString btnLeave = "background-color: rgb(30, 32, 34);\
												color: rgb(255, 255, 255);\
												text-align: center;\
												line-height: 32px;\
												border-radius: 5px;";

private:
	Ui::TitleClass ui;
};
