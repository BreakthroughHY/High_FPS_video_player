#include "helper.h"

Helper::Helper()
{

}

Helper::~Helper()
{}

// º”‘ÿQSS
QString Helper::loadQssStr(QString QSSPath)
{
    QString strQSS;
    QFile FileQSS(QSSPath);
    if (FileQSS.open(QIODevice::ReadOnly))
    {
        strQSS = FileQSS.readAll();
        FileQSS.close();
    }
    else
    {
        qDebug() << "Failed to read style sheet£∫" << QSSPath;
    }
    return strQSS;
}