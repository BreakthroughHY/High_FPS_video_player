#pragma once
//#include <QString>
#include <QFile>
#include <Qdebug>

class Helper {

public:
	Helper();

	~Helper();

	// ����QSS
	static QString loadQssStr(QString strQssPath);
};