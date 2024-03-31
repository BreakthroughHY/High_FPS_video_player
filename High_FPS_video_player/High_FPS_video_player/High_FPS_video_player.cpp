#include "High_FPS_video_player.h"
#include "qpushbutton.h"

High_FPS_video_player::High_FPS_video_player(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    // 关闭窗口控件
    setWindowFlags(Qt::FramelessWindowHint);
    // 样式
    setStyle();
    // 设置鼠标追踪   在鼠标未按下时也能检测鼠标移动
    centralWidget()->setMouseTracking(true); // 启用中心窗口的鼠标跟踪
    qApp->installEventFilter(this); // 在主窗口类内安装事件过滤器

    connectSignalSlots();
    //QCursor cursor(Qt::SizeBDiagCursor); // 使用十字光标
    //setCursor(cursor);


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

    // 处理鼠标移动的事件
    if (event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if (winZoomStatus.reSizeIng && mouseEvent->button() == Qt::LeftButton)
        {
            winZoomStatus.reSizeIng = false;
        }
    }

    return QMainWindow::eventFilter(obj, event);
}

// 样式
void High_FPS_video_player::setStyle()
{
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

// 大小改变虚函数
void High_FPS_video_player::resizeEvent(QResizeEvent* event)
{
    if (height() < 600 || width() < 980)
    {
        ui.PlaylistWid->hide();
    }
    else
    {
        ui.PlaylistWid->show();
    }
    ui.CtrlBarWid->setGeometry(0, ui.ShowWid->height() - 72, ui.ShowWid->width(), 72);

    //ui.label

    ui.CtrlBarWid->raise();
}

// 处理窗口移动的槽函数
void High_FPS_video_player::do_MoveWindow(QPoint& tempPos)
{
    this->move(tempPos);
}
