#include "High_FPS_video_player.h"
#include <QtWidgets/QApplication>
#include "video_class.h"
#include "demux_thread.h"
#include "data_singleton.h"
#include "decode_thread.h"
#include "audio_out_thread.h"

#include <thread>
#include <chrono>


int main(int argc, char *argv[])
{
    /*QApplication a(argc, argv);
    High_FPS_video_player w;
    w.show();
    return a.exec();*/

    DataSingleton& d = DataSingleton::getInstance();

    d.setFFmpegQueue(10, 10, 10, 10);

    videoClass s;

    s.loadVideo("G:\\Python���\\python��Ŀ\\PythonQt\\images\\xtl.mp4");


    DemuxThread dt;
    DecodeThread det(DecodeThread::Types::VIDEO);
    DecodeThread deta(DecodeThread::Types::AUDIO);
    AudioOutThread a1;
    

    dt.setParameters();
    det.setParameters();
    deta.setParameters();
    a1.setParameters();
    dt.start();
    det.start();
    deta.start();
    a1.start();

    // ʹ��ǰ�߳�����500����
    std::this_thread::sleep_for(std::chrono::milliseconds(5000000));
    std::cout << "timeOut" << endl;

    dt.stop();
    //det.stop();
    //deta.stop();
    //a1.stop();

    return 0;
}
