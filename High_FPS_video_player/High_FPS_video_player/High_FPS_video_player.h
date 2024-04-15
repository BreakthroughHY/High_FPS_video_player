#pragma once

#include <QtWidgets/QMainWindow>
#include <QTimer>
#include "ui_High_FPS_video_player.h"

typedef struct WinZoomStatus {
    //bool onTop = false, onRight = false, onBottom = false, onLeft = false;
    int mouseBorderStatu = 11; // 数据是进行按位运算的  0b1011
    bool reSize = false; // 鼠标位置是否可改变窗口大小
    bool reSizeIng = false; // 鼠标位置正在改变窗口大小
    bool mouseIsArrow = true; // 记录鼠标状态，防止恢复鼠标时多次执行
    QPoint clickPoin; // 记录鼠标点击的位置
    
}WinZoomStatus;

class VideoClass;
class DemuxThread;
class DecodeThread;
class AudioOutThread;

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
    // 大小改变虚函数
    void resizeEvent(QResizeEvent* event) override;
    // 鼠标离开窗口触发的事件
    void leaveEvent(QEvent* event);
    // 事件过滤器虚函数
    bool eventFilter(QObject* obj, QEvent* event);

private:
    // 样式
    void setStyle();
    // 连接信号和槽
    void connectSignalSlots();
    // 初始化FFmpeg相关的线程和设置
    void initFFmpeg();
    // 开启FFmpeg线程
    void startFFmpeg();
    // 窗口缩放处理(函数内处理的是鼠标样式的变化和判断当前鼠标位置是否具备改变窗口大小条件)
    void winZoom(QPoint& curPos);
    // 处理四个边的扩展
    void borderExtension(QPoint& curPoint, int flag);
    // CtrlBarWid位置和大小设置函数，show中无法访问CtrlBarWid所有在这里设置
    void setCtrlBarWidPos();

signals:
    // 设置音量滑块显示与否的信号
    void sig_SetVolumeSliderShowHide(bool flag);
    // 向show发信号显示btnCloseVideoList
    void sig_showbtnCloseVideoList();
    // 立刻隐藏音量滑块窗口信号
    void sig_hideVolumeSlider();
    // 立刻隐藏视频列表关闭按钮
    void sig_hideBtnCloseVideoList();
    // 向ctr_bar发送视频的总秒数
    void sig_SetVideoTotalTimeTimeEdit(int second);
    // 通过中转向ctr_bar发送当前视频播放的位置
    void sig_VideoPlayTimeTimeEdit(int second);
    // 设置窗口上方的视频名
    void sig_setVideoName(QString videoName);
    // 向tar_bar发送播放状态
    void sig_playing();
    
private slots:
    // 处理窗口移动的槽函数
    void do_MoveWindow(QPoint& tempPos);
    // 隐藏ctr_bar
    void do_TimeoutHideCtr_bar();
    // 视频列表隐藏和显示
    void do_videoListShowHide(bool flag);
    // 根据show传出的音量滑块窗口是否显示进行设置
    void do_enterVolumeWid(bool flag);
    // 根据show传出的是否进入BtnCloseVideoList进行处理
    void do_btnCloseVideoList(bool flag);
    // 全屏
    void do_fullScreen(bool flag);
    // 播放指定item中的视频
    void playItem(QString path, QString videoName);
    // 恢复FFmpeg为初始状态 关闭线程 清空容器
    void reInitState();
    // 处理ctr_bar发送的当前播放状态
    void do_playing(bool playing);


private:
    // 全局唯一的数据对象 支持多线程访问
    DataSingleton& dataSingleton;

    // FFmpeg
    // 解复用线程对象指针
    VideoClass* videoClass;
    DemuxThread* demuxThread;
    DecodeThread* vDecodeThread;
    DecodeThread* aDecodeThread;
    AudioOutThread* audioOutThread;

    // Qt
    int borderWidth = 5; // 边框宽度   由于检测鼠标是否在边框上
    // 窗口缩放状态记录
    WinZoomStatus winZoomStatus;
    // 鼠标样式对象
    QCursor cursor;
    // 窗口当前的位置和大小信息
    QRect curGeometry;
    // 定时器对象指针
    QTimer* ctr_barTimer;
    // ctr_bar是否显示
    bool ctr_barDisplay = false;
    // 用户是否隐藏了视频列表
    bool hiddenVideoList = false;
    // 进入音量滑块窗口
    bool enterVolumeWid = false;
    // 进入BtnCloseVideoList
    bool enterBtnCloseVideoList = false;
    // 是全屏
    bool sig_FullScreen = false;

private:
    Ui::High_FPS_video_playerClass ui;
};
