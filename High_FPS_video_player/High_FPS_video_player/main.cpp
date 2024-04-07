#include "High_FPS_video_player.h"
#include <QtWidgets/QApplication>
#include "video_class.h"
#include "demux_thread.h"
#include "data_singleton.h"

#include <thread>
#include <chrono>


int main(int argc, char *argv[])
{
    /*QApplication a(argc, argv);
    High_FPS_video_player w;
    w.show();
    return a.exec();*/

    DataSingleton& d = DataSingleton::getInstance();

    d.setFFmpegQueue(1000, 1000, 1000, 1000);

    videoClass s;

    s.loadVideo("G:\\Python编程\\python项目\\PythonQt\\images\\xtl.mp4");

    DemuxThread dt;

    dt.setParameters();

    dt.start();

    // 使当前线程休眠500毫秒
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

}
