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
    // �رմ��ڿؼ�
    setWindowFlags(Qt::FramelessWindowHint);
    // ��ʽ
    setStyle();
    // �������׷��   �����δ����ʱҲ�ܼ������ƶ�
    //centralWidget()->setMouseTracking(true); // �������Ĵ��ڵ�������

    qApp->installEventFilter(this); // �����������ڰ�װ�¼�������

    // ��ʱ������ָ��  (Ҫ�����Ӳۺ���֮ǰ������ʱ������)
    ctr_barTimer = new QTimer(this);
    // ��ʱ����
    ctr_barTimer->setTimerType(Qt::PreciseTimer);
    // ��ʱ���
    ctr_barTimer->setInterval(1000);
    // ���õ��μ�ʱ
    ctr_barTimer->setSingleShot(true);

    // �����źźͲ�
    connectSignalSlots();
    
    // ����CtrlBarWid
    ui.CtrlBarWid->hide();

    //ui.TitleWid->hide();
    //setWindowState(Qt::WindowNoState);

    initFFmpeg();
    startFFmpeg();
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

        // �������ʱ��ֹ���д��ڴ�С��ק
        if (!ui.TitleWid->getMaximize() && !sig_FullScreen)
        {
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
        // ���������봰���¼�
        // ����title�·�
        if (curPos.y() >= ui.TitleWid->height())
        {
            // ��ʾ�����
            QApplication::restoreOverrideCursor();  // ȡ�����أ���ʾ�����
            // ��show���ź���ʾbtnCloseVideoList
            if (!sig_FullScreen)
            {
                emit sig_showbtnCloseVideoList();
            }

            // ����show
            if (!enterVolumeWid && !enterBtnCloseVideoList) // ����������������鴰�ھͽ�ֹͨ�����λ�ø���ctr_bar����ʾ
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

    // �������̧����¼�
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

// ��ʽ
void High_FPS_video_player::setStyle()
{
    ui.TitleWid->setStyleSheet(Helper::loadQssStr(":/qss/qss/title.css"));

    ui.CtrlBarWid->setStyleSheet(Helper::loadQssStr(":/qss/qss/ctr_bar.css"));

    ui.ShowWid->setStyleSheet(Helper::loadQssStr(":/qss/qss/show.css"));

    ui.PlaylistWid->setStyleSheet(Helper::loadQssStr(":/qss/qss/playl_list_wid.css"));
    
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
    // ����ShowWid�������ڵ�����CtrlBarWidλ�õĺ���
    connect(ui.ShowWid, &Show::sig_setCtrlBarWidPos, this, &High_FPS_video_player::setCtrlBarWidPos);
    // ctr_bar�ź�ͨ����������ת��show
    connect(ui.CtrlBarWid, &CtrBar::sig_SetVolumeSliderShowHide, this, &High_FPS_video_player::sig_SetVolumeSliderShowHide);
    connect(this, &High_FPS_video_player::sig_SetVolumeSliderShowHide, ui.ShowWid, &Show::do_TimeoutShowVolumeWid);
    // ���������title�·��ƶ����źŷ���show����btnCloseVideoList�ɼ�
    connect(this, &High_FPS_video_player::sig_showbtnCloseVideoList, ui.ShowWid, &Show::do_TimeoutShowBtnVideoList);
    // ����show��������Ƶ�б��Ƿ���ʾ���źźͶ�Ӧ�����������еĲۺ���
    connect(ui.ShowWid, &Show::sig_videoListShowHide, this, &High_FPS_video_player::do_videoListShowHide);
    // ����show�������Ƿ�����������鴰�ںͶ�Ӧ���������еĲۺ���
    connect(ui.ShowWid, &Show::sig_enterVolumeWid, this, &High_FPS_video_player::do_enterVolumeWid);
    // �����ڷ����ź����������������鴰��
    connect(this, &High_FPS_video_player::sig_hideVolumeSlider, ui.ShowWid, &Show::do_TimeoutHideVolumeWid);
    // ��������show�����ź�����������Ƶ�б�رհ�ť�ۺ���
    connect(this, &High_FPS_video_player::sig_hideBtnCloseVideoList, ui.ShowWid, &Show::do_hideBtnCloseVideoList);
    // show�����ź������ڸ����źŽ�����Ӧ������
    connect(ui.ShowWid, &Show::sig_enterBtnCloseVideoList, this, &High_FPS_video_player::do_btnCloseVideoList);
    // ����ctr_bar�е�ȫ���ź����������е�ȫ���ۺ���
    connect(ui.CtrlBarWid, &CtrBar::sig_fullScreen, this, &High_FPS_video_player::do_fullScreen);
    // ���Ӳ����б��е�item������Ĳ����źź��������еĶ�Ӧ�ۺ���
    connect(ui.PlaylistWid, &PlaylListWid::sig_playItem, this, &High_FPS_video_player::playItem);


    // �Լ����Լ�
    // ����ctr_bar��Ӧ�Ķ�ʱ�������زۺ���
    connect(this->ctr_barTimer, &QTimer::timeout, this, &High_FPS_video_player::do_TimeoutHideCtr_bar);

    // ����������
    // ctr_bar�еĲ��ſռ��С�ı䷢���ź�����show�е�������������λ�õĲۺ���
    connect(ui.CtrlBarWid, &CtrBar::sig_SetVolumeSliderPos, ui.ShowWid, &Show::do_SetVolumeSliderPos);

}

// ��ʼ��FFmpeg��ص��̺߳�����
void High_FPS_video_player::initFFmpeg()
{
    //dataSingleton.setFFmpegQueue(10, 10, 10, 10);
    videoClass = new VideoClass();
    demuxThread = new DemuxThread();
    vDecodeThread = new DecodeThread(DecodeThread::Types::VIDEO);
    aDecodeThread = new DecodeThread(DecodeThread::Types::AUDIO);
    audioOutThread = new AudioOutThread();
}

// ����FFmpeg�߳�
void High_FPS_video_player::startFFmpeg()
{
    //videoClass->loadVideo("G:\\Python���\\python��Ŀ\\PythonQt\\images\\xtl.mp4");
    //videoClass->loadVideo("G:\\ϵͳĬ��\\����\\�½��ļ��� (4)\\(pCodecCtx-width  900  pCodecCtx-height).mkv");
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

// CtrlBarWidλ�úʹ�С���ú�����show���޷�����CtrlBarWid��������������
void High_FPS_video_player::setCtrlBarWidPos()
{
    ui.CtrlBarWid->setGeometry(0, ui.ShowWid->height() - 72, ui.ShowWid->width(), 72);
}

// ��С�ı��麯��
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

    // ����Ϊ�����ڵ���ǰ��
    ui.CtrlBarWid->raise();
}

// ����뿪���ڴ������¼�
void High_FPS_video_player::leaveEvent(QEvent* event) {
    if (event->type() == QEvent::Leave) {
        // ����뿪���������û�п���ctr_bar��Ӧ�ļ�ʱ���Ϳ���
        if (ctr_barDisplay)
        {
            ctr_barTimer->stop();
            ui.CtrlBarWid->hide();
            emit sig_hideVolumeSlider();
        }

        // ����������Ƶ�б�رհ�ť�źŷ���
        emit sig_hideBtnCloseVideoList();
    }
}

// �������ƶ��Ĳۺ���
void High_FPS_video_player::do_MoveWindow(QPoint& tempPos)
{
    this->move(tempPos);
}

// ��ʾctr_bar
void High_FPS_video_player::do_TimeoutHideCtr_bar()
{
    ctr_barDisplay = false;
    ui.CtrlBarWid->hide();
    QApplication::setOverrideCursor(Qt::BlankCursor);  // ���������
}

// ��Ƶ�б����غ���ʾ
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

// ����show�������������鴰���Ƿ���ʾ��������
void High_FPS_video_player::do_enterVolumeWid(bool flag)
{
    enterVolumeWid = flag;
    if (ctr_barTimer->isActive())
        ctr_barTimer->stop();
}

// ����show�������Ƿ����BtnCloseVideoList���д���
void High_FPS_video_player::do_btnCloseVideoList(bool flag)
{
    enterBtnCloseVideoList = flag;
    if (ctr_barTimer->isActive())
        ctr_barTimer->stop();
    ui.CtrlBarWid->hide();
    emit sig_hideVolumeSlider();
}

// ȫ��
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

// ����ָ��item�е���Ƶ
void High_FPS_video_player::playItem(QString path, QString videoName)
{
    videoClass->loadVideo(path);
    //videoClass->loadVideo("G:\\ϵͳĬ��\\����\\�½��ļ��� (4)\\(pCodecCtx-width  900  pCodecCtx-height).mkv");
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