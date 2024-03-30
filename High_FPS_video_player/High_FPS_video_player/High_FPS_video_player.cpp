#include "High_FPS_video_player.h"
#include "qpushbutton.h"

High_FPS_video_player::High_FPS_video_player(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    // �رմ��ڿؼ�
    setWindowFlags(Qt::FramelessWindowHint);
    
    connectSignalSlots();
}

High_FPS_video_player::~High_FPS_video_player()
{}

// �����źźͲ�
void High_FPS_video_player::connectSignalSlots()
{
    // �����ڹرհ�ť
    connect(ui.TitleWid, &Title::sig_CloseBtnClicked, this, &High_FPS_video_player::close);
    // ��������󻯰�ť     �ͻָ�
    connect(ui.TitleWid, &Title::sig_MaxBtnClicked, this, &High_FPS_video_player::showMaximized);
    connect(ui.TitleWid, &Title::sig_ResMaxBtnClicked, this, &High_FPS_video_player::showNormal);
    // ��������С����ť
    connect(ui.TitleWid, &Title::sig_MinBtnClicked, this, &High_FPS_video_player::showMinimized);
    // �����ƶ������ƶ�
    connect(ui.TitleWid, &Title::sig_MoveWindow, this, &High_FPS_video_player::do_MoveWindow);
}

// �������ƶ��Ĳۺ���
void High_FPS_video_player::do_MoveWindow(QPoint& tempPos)
{
    this->move(tempPos);
}
