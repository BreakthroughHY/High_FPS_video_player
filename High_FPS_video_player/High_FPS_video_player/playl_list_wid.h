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
	// 绘制虚函数实现
	void paintEvent(QPaintEvent* event) override;

private:
	Ui::PlaylListWidClass ui;
};
