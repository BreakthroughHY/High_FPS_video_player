#include "High_FPS_video_player.h"
#include "qpushbutton.h"
#include "video_class.h"
#include "demux_thread.h"
#include "decode_thread.h"
#include "audio_out_thread.h"

High_FPS_video_player::High_FPS_video_player(QWidget *parent)
    : QMainWindow(parent), dataSingleton(DataSingleton::getInstance())
{
    ui.setupUi(this);
    // 关闭窗口控件
    setWindowFlags(Qt::FramelessWindowHint);
    // 样式
    setStyle();
    // 设置鼠标追踪   在鼠标未按下时也能检测鼠标移动
    //centralWidget()->setMouseTracking(true); // 启用中心窗口的鼠标跟踪

    qApp->installEventFilter(this); // 在主窗口类内安装事件过滤器

    // 定时器对象指针  (要在连接槽函数之前创建计时器对象)
    ctr_barTimer = new QTimer(this);
    // 定时精度
    ctr_barTimer->setTimerType(Qt::PreciseTimer);
    // 定时间隔
    ctr_barTimer->setInterval(1000);
    // 设置单次计时
    ctr_barTimer->setSingleShot(true);

    // 连接信号和槽
    connectSignalSlots();
    
    // 隐藏CtrlBarWid
    ui.CtrlBarWid->hide();

    //ui.TitleWid->hide();
    //setWindowState(Qt::WindowNoState);

    initFFmpeg();
    startFFmpeg();
}

High_FPS_video_player::~High_FPS_video_player()
{}

// 鼠标按下虚函数实现
void High_FPS_video_player::mousePressEvent(QMouseEvent* event)
{
    // 处理拖动窗口大小时松开鼠标左键的处理
    if (winZoomStatus.reSize && event->button() == Qt::LeftButton)
    {
        // 记录鼠标点击的位置（相对于屏幕）
        winZoomStatus.clickPoin = event->globalPosition().toPoint();
        // 设置窗口当前为扩展状态
        winZoomStatus.reSizeIng = true;
        // 记录窗口改变大小前的位置和大小
        curGeometry = geometry();
    }

    return QMainWindow::mouseMoveEvent(event);
}

// 鼠标移动虚函数实现
void High_FPS_video_player::mouseMoveEvent(QMouseEvent* event)
{
    return QMainWindow::mouseMoveEvent(event);
}

// 鼠标抬起虚函数实现
void High_FPS_video_player::mouseReleaseEvent(QMouseEvent* event)
{
    // 处理拖动窗口大小时松开鼠标左键的处理
    if (winZoomStatus.reSizeIng && event->button() == Qt::LeftButton)
    {
        winZoomStatus.reSizeIng = false;
    }

    QMainWindow::mouseReleaseEvent(event);
}

bool High_FPS_video_player::eventFilter(QObject* obj, QEvent* event)
{
    // 处理鼠标移动的事件
    if (event->type() == QEvent::MouseMove)
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        QPoint globalPos = mouseEvent->globalPos(); // 获取全局坐标
        QPoint curPos = mapFromGlobal(globalPos); // 将全局坐标转换为相对于窗口的坐标
        // 窗口缩放处理(函数内处理的是鼠标样式的变化和判断当前鼠标位置是否具备改变窗口大小条件)

        // 窗口最大化时禁止进行窗口大小拖拽
        if (!ui.TitleWid->getMaximize() && !sig_FullScreen)
        {
            winZoom(curPos);

            if (winZoomStatus.reSizeIng)
            {
                QPoint curPoint = mouseEvent->globalPosition().toPoint();
                switch (winZoomStatus.mouseBorderStatu)
                {
                case 0b1000:    // 上
                    borderExtension(curPoint, 0b1000);
                    break;
                case 0b0100:    // 右
                    borderExtension(curPoint, 0b0100);
                    break;
                case 0b0010:    // 下
                    borderExtension(curPoint, 0b0010);
                    break;
                case 0b0001:    // 左
                    borderExtension(curPoint, 0b0001);
                    break;
                case 0b1100:    // 右上
                    borderExtension(curPoint, 0b1000);
                    borderExtension(curPoint, 0b0100);
                    break;
                case 0b0110:    // 右下
                    borderExtension(curPoint, 0b0100);
                    borderExtension(curPoint, 0b0010);
                    break;
                case 0b0011:    // 左下
                    borderExtension(curPoint, 0b0010);
                    borderExtension(curPoint, 0b0001);
                    break;
                case 0b1001:    // 左上
                    borderExtension(curPoint, 0b0001);
                    borderExtension(curPoint, 0b1000);
                    break;
                default:
                    break;
                }
                // 将新的窗口数据设置给窗口
                setGeometry(curGeometry);
                // 设置新的鼠标位置
                winZoomStatus.clickPoin = curPoint;
            }
        }
        // 处理鼠标进入窗口事件
        // 进入title下方
        if (curPos.y() >= ui.TitleWid->height())
        {
            // 显示鼠标光标
            QApplication::restoreOverrideCursor();  // 取消隐藏，显示鼠标光标
            // 向show发信号显示btnCloseVideoList
            if (!sig_FullScreen)
            {
                emit sig_showbtnCloseVideoList();
            }

            // 进入show
            if (!enterVolumeWid && !enterBtnCloseVideoList) // 如果鼠标进入音量滑块窗口就禁止通过鼠标位置更改ctr_bar的显示
            {
                if (curPos.x() < ui.ShowWid->width())
                {
                    ctr_barDisplay = true;
                    ui.CtrlBarWid->show();
                    if (curPos.y() > height() - ui.CtrlBarWid->height())
                    {
                        ctr_barTimer->stop();
                    }
                    else
                    {
                        ctr_barTimer->start();
                    }
                }
                else
                {
                    if (ctr_barDisplay)
                    {
                        ctr_barDisplay = false;
                        ctr_barTimer->stop();
                        ui.CtrlBarWid->hide();
                        emit sig_hideVolumeSlider();
                    }
                }
            }
        }
        else
        {
            if (ctr_barDisplay)
            {
                ctr_barDisplay = false;
                ctr_barTimer->stop();
                ui.CtrlBarWid->hide();
                emit sig_hideVolumeSlider();
            }
        }
    }

    // 处理鼠标抬起的事件
    if (event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if (winZoomStatus.reSizeIng && mouseEvent->button() == Qt::LeftButton)
        {
            winZoomStatus.reSizeIng = false;
            ctr_barTimer->start();
        }
    }

    return QMainWindow::eventFilter(obj, event);
}

// 样式
void High_FPS_video_player::setStyle()
{
    ui.TitleWid->setStyleSheet(Helper::loadQssStr(":/qss/qss/title.css"));

    ui.CtrlBarWid->setStyleSheet(Helper::loadQssStr(":/qss/qss/ctr_bar.css"));

    ui.ShowWid->setStyleSheet(Helper::loadQssStr(":/qss/qss/show.css"));

    ui.PlaylistWid->setStyleSheet(Helper::loadQssStr(":/qss/qss/playl_list_wid.css"));
    
}

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
    // 连接title和主窗口的鼠标抬起事件
    connect(ui.TitleWid, &Title::sig_borderExtension, this, &High_FPS_video_player::mouseReleaseEvent);
    // 连接ShowWid和主窗口的设置CtrlBarWid位置的函数
    connect(ui.ShowWid, &Show::sig_setCtrlBarWidPos, this, &High_FPS_video_player::setCtrlBarWidPos);
    // ctr_bar信号通过主窗口中转到show
    connect(ui.CtrlBarWid, &CtrBar::sig_SetVolumeSliderShowHide, this, &High_FPS_video_player::sig_SetVolumeSliderShowHide);
    connect(this, &High_FPS_video_player::sig_SetVolumeSliderShowHide, ui.ShowWid, &Show::do_TimeoutShowVolumeWid);
    // 窗口鼠标在title下方移动的信号发给show设置btnCloseVideoList可见
    connect(this, &High_FPS_video_player::sig_showbtnCloseVideoList, ui.ShowWid, &Show::do_TimeoutShowBtnVideoList);
    // 连接show的设置视频列表是否显示的信号和对应的在主窗口中的槽函数
    connect(ui.ShowWid, &Show::sig_videoListShowHide, this, &High_FPS_video_player::do_videoListShowHide);
    // 连接show传出的是否进入音量滑块窗口和对应在主窗口中的槽函数
    connect(ui.ShowWid, &Show::sig_enterVolumeWid, this, &High_FPS_video_player::do_enterVolumeWid);
    // 主窗口发射信号立即隐藏音量滑块窗口
    connect(this, &High_FPS_video_player::sig_hideVolumeSlider, ui.ShowWid, &Show::do_TimeoutHideVolumeWid);
    // 主窗口向show发射信号立刻隐藏视频列表关闭按钮槽函数
    connect(this, &High_FPS_video_player::sig_hideBtnCloseVideoList, ui.ShowWid, &Show::do_hideBtnCloseVideoList);
    // show发射信号主窗口根据信号进行相应的设置
    connect(ui.ShowWid, &Show::sig_enterBtnCloseVideoList, this, &High_FPS_video_player::do_btnCloseVideoList);
    // 连接ctr_bar中的全屏信号与主窗口中的全屏槽函数
    connect(ui.CtrlBarWid, &CtrBar::sig_fullScreen, this, &High_FPS_video_player::do_fullScreen);
    // 连接播放列表中的item被点击的播放信号和主窗口中的对应槽函数
    connect(ui.PlaylistWid, &PlaylListWid::sig_playItem, this, &High_FPS_video_player::playItem);


    // 自己连自己
    // 连接ctr_bar对应的定时器和隐藏槽函数
    connect(this->ctr_barTimer, &QTimer::timeout, this, &High_FPS_video_player::do_TimeoutHideCtr_bar);

    // 组件连接组件
    // ctr_bar中的播放空间大小改变发射信号连接show中的设置声音滑块位置的槽函数
    connect(ui.CtrlBarWid, &CtrBar::sig_SetVolumeSliderPos, ui.ShowWid, &Show::do_SetVolumeSliderPos);

}

// 初始化FFmpeg相关的线程和设置
void High_FPS_video_player::initFFmpeg()
{
    //dataSingleton.setFFmpegQueue(10, 10, 10, 10);
    videoClass = new VideoClass();
    demuxThread = new DemuxThread();
    vDecodeThread = new DecodeThread(DecodeThread::Types::VIDEO);
    aDecodeThread = new DecodeThread(DecodeThread::Types::AUDIO);
    audioOutThread = new AudioOutThread();
}

// 开启FFmpeg线程
void High_FPS_video_player::startFFmpeg()
{
    //videoClass->loadVideo("G:\\Python编程\\python项目\\PythonQt\\images\\xtl.mp4");
    //videoClass->loadVideo("G:\\系统默认\\桌面\\新建文件夹 (4)\\(pCodecCtx-width  900  pCodecCtx-height).mkv");
    /*demuxThread->setParameters();
    vDecodeThread->setParameters();
    aDecodeThread->setParameters();
    audioOutThread->setParameters();
    this->ui.ShowWid->setOpenGLParameters(dataSingleton.getWdividedH());*/

    //demuxThread->start();
    //vDecodeThread->start();
    //aDecodeThread->start();
    //audioOutThread->start();
}

// 窗口缩放处理(函数内处理的是鼠标样式的变化和判断当前鼠标位置是否具备改变窗口大小条件)
void High_FPS_video_player::winZoom(QPoint& curPos)
{
    if (winZoomStatus.reSizeIng) // 如果窗口在改变大小中，就不进行鼠标状态和   鼠标和边框状态变量的修改
        return;

    int temp = winZoomStatus.mouseBorderStatu;
    // 将鼠标是否在四个边的状态全部置为false
    winZoomStatus.mouseBorderStatu = winZoomStatus.mouseBorderStatu & 0b0000;

    if (curPos.y() < borderWidth)
        winZoomStatus.mouseBorderStatu = winZoomStatus.mouseBorderStatu | 0b1000;
    else if (curPos.y() >= height() - borderWidth)
        winZoomStatus.mouseBorderStatu = winZoomStatus.mouseBorderStatu | 0b0010;

    if (curPos.x() < borderWidth)
        winZoomStatus.mouseBorderStatu = winZoomStatus.mouseBorderStatu | 0b0001;
    else if (curPos.x() >= width() - borderWidth)
        winZoomStatus.mouseBorderStatu = winZoomStatus.mouseBorderStatu | 0b0100;

    // 设置当前可以通过拖拽改变窗口大小
    if (winZoomStatus.mouseBorderStatu)
    {
        winZoomStatus.reSize = true;
        winZoomStatus.mouseIsArrow = false; // 设置鼠标不在是箭头
    }
    else
    {
        if (!winZoomStatus.mouseIsArrow)
        {
            winZoomStatus.mouseIsArrow = true;
            cursor.setShape(Qt::ArrowCursor);
            setCursor(cursor);
        }
        winZoomStatus.reSize = false;
        return;
    }


    switch (winZoomStatus.mouseBorderStatu)
    {
    case 0b1000:
    case 0b0010:
        // 顶部和底部
        cursor.setShape(Qt::SizeVerCursor);
        setCursor(cursor);
        break;
    case 0b0001:
    case 0b0100:
        // 左右
        cursor.setShape(Qt::SizeHorCursor);
        setCursor(cursor);
        break;
    case 0b0011:
    case 0b1100:
        // 左下和右上
        cursor.setShape(Qt::SizeBDiagCursor);
        setCursor(cursor);
        break;
    case 0b1001:
    case 0b0110:
        // 左上和右下
        cursor.setShape(Qt::SizeFDiagCursor);
		setCursor(cursor);
        break;
    default:
        break;
    }
}

// 处理四个边的扩展
void High_FPS_video_player::borderExtension(QPoint& curPoint, int flag)
{
    int xVariation, yVariation;
    int temp;

    switch (flag)
    {
    case 0b1000: // 处理上边框
        temp = curGeometry.y();
        yVariation = curPoint.y() - winZoomStatus.clickPoin.y();
        curGeometry.setY(curGeometry.y() + yVariation);
        if (curGeometry.height() < 500)
        {
            curGeometry.setY(temp);
            curPoint.setY(winZoomStatus.clickPoin.y());
        }
        break;
    case 0b0100: // 处理右边框
        xVariation = curPoint.x() - winZoomStatus.clickPoin.x();
        curGeometry.setWidth(curGeometry.width() + xVariation);
        break;
    case 0b0010: // 处理下边框
        yVariation = curPoint.y() - winZoomStatus.clickPoin.y();
        curGeometry.setHeight(curGeometry.height() + yVariation);
        break;
    case 0b0001: // 处理左边框
        temp = curGeometry.x();
        xVariation = curPoint.x() - winZoomStatus.clickPoin.x();
        curGeometry.setX(curGeometry.x() + xVariation);
        if (curGeometry.width() < 800)
        {
            curGeometry.setX(temp);
            curPoint.setX(winZoomStatus.clickPoin.x());
        }
        break;
    default:
        break;
    }
}

// CtrlBarWid位置和大小设置函数，show中无法访问CtrlBarWid所有在这里设置
void High_FPS_video_player::setCtrlBarWidPos()
{
    ui.CtrlBarWid->setGeometry(0, ui.ShowWid->height() - 72, ui.ShowWid->width(), 72);
}

// 大小改变虚函数
void High_FPS_video_player::resizeEvent(QResizeEvent* event)
{
    if (!hiddenVideoList)
    {
        if (height() < 600 || width() < 980)
        {
            ui.PlaylistWid->hide();
        }
        else
        {
            ui.PlaylistWid->show();
        }
    }

    // 设置为父窗口的最前面
    ui.CtrlBarWid->raise();
}

// 鼠标离开窗口触发的事件
void High_FPS_video_player::leaveEvent(QEvent* event) {
    if (event->type() == QEvent::Leave) {
        // 鼠标离开窗口是如果没有开启ctr_bar对应的计时器就开起
        if (ctr_barDisplay)
        {
            ctr_barTimer->stop();
            ui.CtrlBarWid->hide();
            emit sig_hideVolumeSlider();
        }

        // 立刻隐藏视频列表关闭按钮信号发射
        emit sig_hideBtnCloseVideoList();
    }
}

// 处理窗口移动的槽函数
void High_FPS_video_player::do_MoveWindow(QPoint& tempPos)
{
    this->move(tempPos);
}

// 显示ctr_bar
void High_FPS_video_player::do_TimeoutHideCtr_bar()
{
    ctr_barDisplay = false;
    ui.CtrlBarWid->hide();
    QApplication::setOverrideCursor(Qt::BlankCursor);  // 隐藏鼠标光标
}

// 视频列表隐藏和显示
void High_FPS_video_player::do_videoListShowHide(bool flag)
{
    if (flag)
    {
        hiddenVideoList = true;
        ui.PlaylistWid->hide();
    }
    else
    {
        hiddenVideoList = false;
        ui.PlaylistWid->show();
    }
}

// 根据show传出的音量滑块窗口是否显示进行设置
void High_FPS_video_player::do_enterVolumeWid(bool flag)
{
    enterVolumeWid = flag;
    if (ctr_barTimer->isActive())
        ctr_barTimer->stop();
}

// 根据show传出的是否进入BtnCloseVideoList进行处理
void High_FPS_video_player::do_btnCloseVideoList(bool flag)
{
    enterBtnCloseVideoList = flag;
    if (ctr_barTimer->isActive())
        ctr_barTimer->stop();
    ui.CtrlBarWid->hide();
    emit sig_hideVolumeSlider();
}

// 全屏
void High_FPS_video_player::do_fullScreen(bool flag)
{
    sig_FullScreen = flag;
    if (flag)
    {
        emit sig_hideBtnCloseVideoList();
        setWindowState(Qt::WindowFullScreen);
        ui.TitleWid->hide();
        ui.PlaylistWid->hide();
    }
    else
    {
        setWindowState(Qt::WindowNoState);
        ui.TitleWid->show();
    }
}

// 播放指定item中的视频
void High_FPS_video_player::playItem(QString path, QString videoName)
{
    videoClass->loadVideo(path);
    //videoClass->loadVideo("G:\\系统默认\\桌面\\新建文件夹 (4)\\(pCodecCtx-width  900  pCodecCtx-height).mkv");
    demuxThread->setParameters();
    vDecodeThread->setParameters();
    aDecodeThread->setParameters();
    audioOutThread->setParameters();
    this->ui.ShowWid->setOpenGLParameters(dataSingleton.getWdividedH());

    demuxThread->start();
    vDecodeThread->start();
    aDecodeThread->start();
    audioOutThread->start();
}