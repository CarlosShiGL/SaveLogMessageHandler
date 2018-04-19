#ifndef SAVELOGMESSAGE_H
#define SAVELOGMESSAGE_H
#include <QtDebug>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDateTime>
#include <QMutex>
#include <QString>
#include <qapplication.h>

extern void SaveLogMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
extern void InitLogFileInfo(QString logFileDir="./log/", QString selfLogName = "", int maxLogFileNum=10);

#endif // SAVELOGMESSAGE_H
