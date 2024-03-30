#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_High_FPS_video_player.h"

class High_FPS_video_player : public QMainWindow
{
    Q_OBJECT

public:
    High_FPS_video_player(QWidget *parent = nullptr);
    ~High_FPS_video_player();

protected:
    // 鼠标抬起事件
    //virtual void mousePressEvent(QMouseEvent* event);

private:
    // 连接信号和槽
    void connectSignalSlots();
    
private slots:
    // 处理窗口移动的槽函数
    void do_MoveWindow(QPoint& tempPos);

private:
    Ui::High_FPS_video_playerClass ui;
};
