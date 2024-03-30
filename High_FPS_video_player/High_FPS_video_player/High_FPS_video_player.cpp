#include "High_FPS_video_player.h"
#include "qpushbutton.h"

High_FPS_video_player::High_FPS_video_player(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    // 关闭窗口控件
    setWindowFlags(Qt::FramelessWindowHint);
    
    connectSignalSlots();
}

High_FPS_video_player::~High_FPS_video_player()
{}

// 连接信号和槽
void High_FPS_video_player::connectSignalSlots()
{
    // 连窗口关闭按钮
    connect(ui.TitleWid, &Title::sig_CloseBtnClicked, this, &High_FPS_video_player::close);
    // 连窗口最大化按钮     和恢复
    connect(ui.TitleWid, &Title::sig_MaxBtnClicked, this, &High_FPS_video_player::showMaximized);
    connect(ui.TitleWid, &Title::sig_ResMaxBtnClicked, this, &High_FPS_video_player::showNormal);
    // 连窗口最小化按钮
    connect(ui.TitleWid, &Title::sig_MinBtnClicked, this, &High_FPS_video_player::showMinimized);
    // 连接移动窗口移动
    connect(ui.TitleWid, &Title::sig_MoveWindow, this, &High_FPS_video_player::do_MoveWindow);
}

// 处理窗口移动的槽函数
void High_FPS_video_player::do_MoveWindow(QPoint& tempPos)
{
    this->move(tempPos);
}
