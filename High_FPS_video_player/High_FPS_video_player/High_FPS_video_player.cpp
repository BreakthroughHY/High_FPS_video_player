#include "High_FPS_video_player.h"
#include "qpushbutton.h"

High_FPS_video_player::High_FPS_video_player(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    // �رմ��ڿؼ�
    setWindowFlags(Qt::FramelessWindowHint);
    // ��ʽ
    setStyle();
    // �������׷��   �����δ����ʱҲ�ܼ������ƶ�
    centralWidget()->setMouseTracking(true); // �������Ĵ��ڵ�������
    qApp->installEventFilter(this); // �����������ڰ�װ�¼�������

    connectSignalSlots();
    //QCursor cursor(Qt::SizeBDiagCursor); // ʹ��ʮ�ֹ��
    //setCursor(cursor);


}

High_FPS_video_player::~High_FPS_video_player()
{}

// ��갴���麯��ʵ��
void High_FPS_video_player::mousePressEvent(QMouseEvent* event)
{
    // �����϶����ڴ�Сʱ�ɿ��������Ĵ���
    if (winZoomStatus.reSize && event->button() == Qt::LeftButton)
    {
        // ��¼�������λ�ã��������Ļ��
        winZoomStatus.clickPoin = event->globalPosition().toPoint();
        // ���ô��ڵ�ǰΪ��չ״̬
        winZoomStatus.reSizeIng = true;
        // ��¼���ڸı��Сǰ��λ�úʹ�С
        curGeometry = geometry();
    }

    return QMainWindow::mouseMoveEvent(event);
}

// ����ƶ��麯��ʵ��
void High_FPS_video_player::mouseMoveEvent(QMouseEvent* event)
{
    return QMainWindow::mouseMoveEvent(event);
}

// ���̧���麯��ʵ��
void High_FPS_video_player::mouseReleaseEvent(QMouseEvent* event)
{
    // �����϶����ڴ�Сʱ�ɿ��������Ĵ���
    if (winZoomStatus.reSizeIng && event->button() == Qt::LeftButton)
    {
        winZoomStatus.reSizeIng = false;
    }

    QMainWindow::mouseReleaseEvent(event);
}

bool High_FPS_video_player::eventFilter(QObject* obj, QEvent* event)
{
    // ��������ƶ����¼�
    if (event->type() == QEvent::MouseMove)
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        QPoint globalPos = mouseEvent->globalPos(); // ��ȡȫ������
        QPoint curPos = mapFromGlobal(globalPos); // ��ȫ������ת��Ϊ����ڴ��ڵ�����
        // �������Ŵ���(�����ڴ�����������ʽ�ı仯���жϵ�ǰ���λ���Ƿ�߱��ı䴰�ڴ�С����)
        winZoom(curPos);

        if (winZoomStatus.reSizeIng)
        {
            QPoint curPoint = mouseEvent->globalPosition().toPoint();
            switch (winZoomStatus.mouseBorderStatu)
            {
            case 0b1000:    // ��
                borderExtension(curPoint, 0b1000);
                break;
            case 0b0100:    // ��
                borderExtension(curPoint, 0b0100);
                break;
            case 0b0010:    // ��
                borderExtension(curPoint, 0b0010);
                break;
            case 0b0001:    // ��
                borderExtension(curPoint, 0b0001);
                break;
            case 0b1100:    // ����
                borderExtension(curPoint, 0b1000);
                borderExtension(curPoint, 0b0100);
                break;
            case 0b0110:    // ����
                borderExtension(curPoint, 0b0100);
                borderExtension(curPoint, 0b0010);
                break;
            case 0b0011:    // ����
                borderExtension(curPoint, 0b0010);
                borderExtension(curPoint, 0b0001);
                break;
            case 0b1001:    // ����
                borderExtension(curPoint, 0b0001);
                borderExtension(curPoint, 0b1000);
                break;
            default:
                break;
            }
            // ���µĴ����������ø�����
            setGeometry(curGeometry);
            // �����µ����λ��
            winZoomStatus.clickPoin = curPoint;
        }
    }

    // ��������ƶ����¼�
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

// ��ʽ
void High_FPS_video_player::setStyle()
{
}

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
    // ����title�������ڵ����̧���¼�
    connect(ui.TitleWid, &Title::sig_borderExtension, this, &High_FPS_video_player::mouseReleaseEvent);
}

// �������Ŵ���(�����ڴ�����������ʽ�ı仯���жϵ�ǰ���λ���Ƿ�߱��ı䴰�ڴ�С����)
void High_FPS_video_player::winZoom(QPoint& curPos)
{
    if (winZoomStatus.reSizeIng) // ��������ڸı��С�У��Ͳ��������״̬��   ���ͱ߿�״̬�������޸�
        return;

    int temp = winZoomStatus.mouseBorderStatu;
    // ������Ƿ����ĸ��ߵ�״̬ȫ����Ϊfalse
    winZoomStatus.mouseBorderStatu = winZoomStatus.mouseBorderStatu & 0b0000;

    if (curPos.y() < borderWidth)
        winZoomStatus.mouseBorderStatu = winZoomStatus.mouseBorderStatu | 0b1000;
    else if (curPos.y() >= height() - borderWidth)
        winZoomStatus.mouseBorderStatu = winZoomStatus.mouseBorderStatu | 0b0010;

    if (curPos.x() < borderWidth)
        winZoomStatus.mouseBorderStatu = winZoomStatus.mouseBorderStatu | 0b0001;
    else if (curPos.x() >= width() - borderWidth)
        winZoomStatus.mouseBorderStatu = winZoomStatus.mouseBorderStatu | 0b0100;

    // ���õ�ǰ����ͨ����ק�ı䴰�ڴ�С
    if (winZoomStatus.mouseBorderStatu)
    {
        winZoomStatus.reSize = true;
        winZoomStatus.mouseIsArrow = false; // ������겻���Ǽ�ͷ
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
        // �����͵ײ�
        cursor.setShape(Qt::SizeVerCursor);
        setCursor(cursor);
        break;
    case 0b0001:
    case 0b0100:
        // ����
        cursor.setShape(Qt::SizeHorCursor);
        setCursor(cursor);
        break;
    case 0b0011:
    case 0b1100:
        // ���º�����
        cursor.setShape(Qt::SizeBDiagCursor);
        setCursor(cursor);
        break;
    case 0b1001:
    case 0b0110:
        // ���Ϻ�����
        cursor.setShape(Qt::SizeFDiagCursor);
		setCursor(cursor);
        break;
    default:
        break;
    }
}

// �����ĸ��ߵ���չ
void High_FPS_video_player::borderExtension(QPoint& curPoint, int flag)
{
    int xVariation, yVariation;
    int temp;

    switch (flag)
    {
    case 0b1000: // �����ϱ߿�
        temp = curGeometry.y();
        yVariation = curPoint.y() - winZoomStatus.clickPoin.y();
        curGeometry.setY(curGeometry.y() + yVariation);
        if (curGeometry.height() < 500)
        {
            curGeometry.setY(temp);
            curPoint.setY(winZoomStatus.clickPoin.y());
        }
        break;
    case 0b0100: // �����ұ߿�
        xVariation = curPoint.x() - winZoomStatus.clickPoin.x();
        curGeometry.setWidth(curGeometry.width() + xVariation);
        break;
    case 0b0010: // �����±߿�
        yVariation = curPoint.y() - winZoomStatus.clickPoin.y();
        curGeometry.setHeight(curGeometry.height() + yVariation);
        break;
    case 0b0001: // ������߿�
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

// ��С�ı��麯��
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

// �������ƶ��Ĳۺ���
void High_FPS_video_player::do_MoveWindow(QPoint& tempPos)
{
    this->move(tempPos);
}
