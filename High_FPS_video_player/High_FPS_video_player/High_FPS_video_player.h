#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_High_FPS_video_player.h"

typedef struct WinZoomStatus {
    //bool onTop = false, onRight = false, onBottom = false, onLeft = false;
    int mouseBorderStatu = 11;
    bool reSize = false; // 鼠标位置是否可改变窗口大小
    bool reSizeIng = false; // 鼠标位置正在改变窗口大小
    bool mouseIsArrow = true; // 记录鼠标状态，防止恢复鼠标时多次执行
    QPoint clickPoin; // 记录鼠标点击的位置
    
}WinZoomStatus;

class High_FPS_video_player : public QMainWindow
{
    Q_OBJECT

public:
    High_FPS_video_player(QWidget *parent = nullptr);
    ~High_FPS_video_player();

protected:
    // 鼠标按下虚函数实现
    void mousePressEvent(QMouseEvent* event) override;
    // 鼠标移动虚函数实现
    void mouseMoveEvent(QMouseEvent* event) override;
    // 鼠标抬起虚函数实现
    void mouseReleaseEvent(QMouseEvent* event) override;
    // 事件过滤器虚函数
    bool eventFilter(QObject* obj, QEvent* event);

private:
    // 样式
    void setStyle();
    // 连接信号和槽
    void connectSignalSlots();
    // 窗口缩放处理(函数内处理的是鼠标样式的变化和判断当前鼠标位置是否具备改变窗口大小条件)
    void winZoom(QPoint& curPos);
    // 处理四个边的扩展
    void borderExtension(QPoint& curPoint, int flag);
    // 大小改变虚函数
    void resizeEvent(QResizeEvent* event) override;
    
private slots:
    // 处理窗口移动的槽函数
    void do_MoveWindow(QPoint& tempPos);
private:
    int borderWidth = 5; // 边框宽度   由于检测鼠标是否在边框上
    // 窗口缩放状态记录
    WinZoomStatus winZoomStatus;
    // 鼠标样式对象
    QCursor cursor;
    // 窗口当前的位置和大小信息
    QRect curGeometry;
private:
    Ui::High_FPS_video_playerClass ui;
};
