#pragma once

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QFileDialog>
#include "ui_playl_list_wid.h"

class PlaylListWid : public QWidget
{
	Q_OBJECT

public:
	PlaylListWid(QWidget *parent = nullptr);
	~PlaylListWid();

protected:
	// �����麯��ʵ��
	void paintEvent(QPaintEvent* event) override;

private:
	// ��ʽ
	void setStyle();
	// �����źźͲ�
	void connectSignalSlots();
	// �¼��������麯��
	bool eventFilter(QObject* obj, QEvent* event);

signals:
	// ��������������ʾ�����ź�
	void sig_playItem(QString path, QString videoName);

public slots:
	// ��ǰ��ʾ����Ƶ�б���л�
	void do_btnLocal();
	// ��ǰ��ʾ����Ƶ�б���л�
	void do_btnNetwork();
	// ���������Ƶ��ť�Ĳۺ���
	void do_btnAddClicked();
	// ��QListWidget�е�item�������ʱ��ִ�еĲۺ���
	void do_itemClicked(QListWidgetItem* item);

private:
	// ��ǰ��ʾ���Ƿ�Ϊ���ص���Ƶ�б�
	bool isLoca = true;
	// ���ػ������类ѡ�е���ʽ
	QString selectStyle = "padding-bottom: -5px;\
							border-bottom: 3px solid rgb(78, 194, 145);\
							color: rgb(78, 194, 145); ";

	// ��¼��ǰ���ϴα�ѡ�е�item�����ַ
	QListWidgetItem* currItem = nullptr;

private:
	Ui::PlaylListWidClass ui;
};
