#include "High_FPS_video_player.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    High_FPS_video_player w;
    w.show();
    return a.exec();
}
