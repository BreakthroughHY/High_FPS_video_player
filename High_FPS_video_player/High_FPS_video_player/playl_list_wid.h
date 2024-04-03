#pragma once

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
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
	Ui::PlaylListWidClass ui;
};
