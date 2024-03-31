#pragma once

#include <QWidget>
#include "ui_show.h"


class Show : public QWidget
{
	Q_OBJECT

public:
	Show(QWidget *parent = nullptr);
	~Show();

private:
	Ui::ShowClass ui;
};
