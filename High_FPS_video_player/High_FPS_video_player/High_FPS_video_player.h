#pragma once

#include <QtWidgets/QMainWindow>
#include <QTimer>
#include "ui_High_FPS_video_player.h"

typedef struct WinZoomStatus {
    //bool onTop = false, onRight = false, onBottom = false, onLeft = false;
    int mouseBorderStatu = 11; // �����ǽ��а�λ�����  0b1011
    bool reSize = false; // ���λ���Ƿ�ɸı䴰�ڴ�С
    bool reSizeIng = false; // ���λ�����ڸı䴰�ڴ�С
    bool mouseIsArrow = true; // ��¼���״̬����ֹ�ָ����ʱ���ִ��
    QPoint clickPoin; // ��¼�������λ��
    
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
    // ��갴���麯��ʵ��
    void mousePressEvent(QMouseEvent* event) override;
    // ����ƶ��麯��ʵ��
    void mouseMoveEvent(QMouseEvent* event) override;
    // ���̧���麯��ʵ��
    void mouseReleaseEvent(QMouseEvent* event) override;
    // ��С�ı��麯��
    void resizeEvent(QResizeEvent* event) override;
    // ����뿪���ڴ������¼�
    void leaveEvent(QEvent* event);
    // �¼��������麯��
    bool eventFilter(QObject* obj, QEvent* event);

private:
    // ��ʽ
    void setStyle();
    // �����źźͲ�
    void connectSignalSlots();
    // ��ʼ��FFmpeg��ص��̺߳�����
    void initFFmpeg();
    // ����FFmpeg�߳�
    void startFFmpeg();
    // �������Ŵ���(�����ڴ�����������ʽ�ı仯���жϵ�ǰ���λ���Ƿ�߱��ı䴰�ڴ�С����)
    void winZoom(QPoint& curPos);
    // �����ĸ��ߵ���չ
    void borderExtension(QPoint& curPoint, int flag);
    // CtrlBarWidλ�úʹ�С���ú�����show���޷�����CtrlBarWid��������������
    void setCtrlBarWidPos();

signals:
    // ��������������ʾ�����ź�
    void sig_SetVolumeSliderShowHide(bool flag);
    // ��show���ź���ʾbtnCloseVideoList
    void sig_showbtnCloseVideoList();
    // ���������������鴰���ź�
    void sig_hideVolumeSlider();
    // ����������Ƶ�б�رհ�ť
    void sig_hideBtnCloseVideoList();
    // ��ctr_bar������Ƶ��������
    void sig_SetVideoTotalTimeTimeEdit(int second);
    // ͨ����ת��ctr_bar���͵�ǰ��Ƶ���ŵ�λ��
    void sig_VideoPlayTimeTimeEdit(int second);
    // ���ô����Ϸ�����Ƶ��
    void sig_setVideoName(QString videoName);
    // ��tar_bar���Ͳ���״̬
    void sig_playing();
    
private slots:
    // �������ƶ��Ĳۺ���
    void do_MoveWindow(QPoint& tempPos);
    // ����ctr_bar
    void do_TimeoutHideCtr_bar();
    // ��Ƶ�б����غ���ʾ
    void do_videoListShowHide(bool flag);
    // ����show�������������鴰���Ƿ���ʾ��������
    void do_enterVolumeWid(bool flag);
    // ����show�������Ƿ����BtnCloseVideoList���д���
    void do_btnCloseVideoList(bool flag);
    // ȫ��
    void do_fullScreen(bool flag);
    // ����ָ��item�е���Ƶ
    void playItem(QString path, QString videoName);
    // �ָ�FFmpegΪ��ʼ״̬ �ر��߳� �������
    void reInitState();
    // ����ctr_bar���͵ĵ�ǰ����״̬
    void do_playing(bool playing);


private:
    // ȫ��Ψһ�����ݶ��� ֧�ֶ��̷߳���
    DataSingleton& dataSingleton;

    // FFmpeg
    // �⸴���̶߳���ָ��
    VideoClass* videoClass;
    DemuxThread* demuxThread;
    DecodeThread* vDecodeThread;
    DecodeThread* aDecodeThread;
    AudioOutThread* audioOutThread;

    // Qt
    int borderWidth = 5; // �߿���   ���ڼ������Ƿ��ڱ߿���
    // ��������״̬��¼
    WinZoomStatus winZoomStatus;
    // �����ʽ����
    QCursor cursor;
    // ���ڵ�ǰ��λ�úʹ�С��Ϣ
    QRect curGeometry;
    // ��ʱ������ָ��
    QTimer* ctr_barTimer;
    // ctr_bar�Ƿ���ʾ
    bool ctr_barDisplay = false;
    // �û��Ƿ���������Ƶ�б�
    bool hiddenVideoList = false;
    // �����������鴰��
    bool enterVolumeWid = false;
    // ����BtnCloseVideoList
    bool enterBtnCloseVideoList = false;
    // ��ȫ��
    bool sig_FullScreen = false;

private:
    Ui::High_FPS_video_playerClass ui;
};
