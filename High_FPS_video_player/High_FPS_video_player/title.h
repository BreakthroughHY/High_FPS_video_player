#pragma once

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QFontDatabase>
#include "helper.h"
#include "ui_title.h"


class Title : public QWidget
{
	Q_OBJECT

public:
	Title(QWidget *parent = nullptr);
	~Title();

	// ��ȡ������󻯵�״̬
	bool getMaximize();

protected:
	// �����麯��ʵ��
	void paintEvent(QPaintEvent* event) override;
	// ��갴���麯��ʵ��
	void mousePressEvent(QMouseEvent* event) override;
	// ���̧���麯��ʵ��
	void mouseReleaseEvent(QMouseEvent* event) override;
	// ����ƶ��麯��ʵ��
	void mouseMoveEvent(QMouseEvent* event) override;

private:
	// ��ʽ
	void setStyle();
    // �¼�����  ����ί�е�����
	bool eventFilter(QObject* watched, QEvent* event) override;
	// �����źźͲ�
	void connectSignalSlots();


signals:
	// ����رհ�ť
	void sig_CloseBtnClicked();
	// �����С����ť
	void sig_MinBtnClicked();
	// �����󻯰�ť
	void sig_MaxBtnClicked();
	// �����󻯰�ť    ��󻯺�ָ�
	void sig_ResMaxBtnClicked();
	// ����ƶ��ź�
	void sig_MoveWindow(QPoint& tempPos);
	// �������ڷ����źŴ������������̧���¼�
	void sig_borderExtension(QMouseEvent* event);

public slots:
	// �رհ�ť��Ӧ�Ĳ�
	void do_closeBtnClicked();
	// ��󻯰�ť��Ӧ�Ĳ�
	void do_maxBtnClicked();
	// ��С����ť��Ӧ�Ĳ�
	void do_minBtnClicked();

private:
	// ��¼�������״̬
	bool maximize = false;
	// ��ǰ�Ƿ����ƶ�״̬
	bool m_moving = false;
	// ��굥����λ������ڴ������Ͻ�����
	QPoint m_lastPos;
	// ����û�����ʱ�Ŀ��
	int NonMaximizeWidth;
	// ����ͼ��
	QFont fontIcon;

	const QString btnEnter = "background-color: rgb(47, 49, 52);\
												color: rgb(255, 255, 255);\
												text-align: center;\
												line-height: 32px;\
												border-radius: 5px;";

	const QString btnLeave = "background-color: rgb(30, 32, 34);\
												color: rgb(255, 255, 255);\
												text-align: center;\
												line-height: 32px;\
												border-radius: 5px;";

private:
	Ui::TitleClass ui;
};
