#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_High_FPS_video_player.h"

class High_FPS_video_player : public QMainWindow
{
    Q_OBJECT

public:
    High_FPS_video_player(QWidget *parent = nullptr);
    ~High_FPS_video_player();

private:
    Ui::High_FPS_video_playerClass ui;
};
