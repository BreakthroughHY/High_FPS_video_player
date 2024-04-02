#pragma once
//#include <QString>
#include <QFile>
#include <Qdebug>

class Helper {

public:
	Helper();

	~Helper();

	// º”‘ÿQSS
	static QString loadQssStr(QString strQssPath);
};