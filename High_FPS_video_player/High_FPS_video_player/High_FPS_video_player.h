#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_High_FPS_video_player.h"

typedef struct WinZoomStatus {
    //bool onTop = false, onRight = false, onBottom = false, onLeft = false;
    int mouseBorderStatu = 11;
    bool reSize = false; // ���λ���Ƿ�ɸı䴰�ڴ�С
    bool reSizeIng = false; // ���λ�����ڸı䴰�ڴ�С
    bool mouseIsArrow = true; // ��¼���״̬����ֹ�ָ����ʱ���ִ��
    QPoint clickPoin; // ��¼�������λ��
    
}WinZoomStatus;

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
    // �¼��������麯��
    bool eventFilter(QObject* obj, QEvent* event);

private:
    // ��ʽ
    void setStyle();
    // �����źźͲ�
    void connectSignalSlots();
    // �������Ŵ���(�����ڴ�����������ʽ�ı仯���жϵ�ǰ���λ���Ƿ�߱��ı䴰�ڴ�С����)
    void winZoom(QPoint& curPos);
    // �����ĸ��ߵ���չ
    void borderExtension(QPoint& curPoint, int flag);
    // ��С�ı��麯��
    void resizeEvent(QResizeEvent* event) override;
    
private slots:
    // �������ƶ��Ĳۺ���
    void do_MoveWindow(QPoint& tempPos);
private:
    int borderWidth = 5; // �߿���   ���ڼ������Ƿ��ڱ߿���
    // ��������״̬��¼
    WinZoomStatus winZoomStatus;
    // �����ʽ����
    QCursor cursor;
    // ���ڵ�ǰ��λ�úʹ�С��Ϣ
    QRect curGeometry;
private:
    Ui::High_FPS_video_playerClass ui;
};
