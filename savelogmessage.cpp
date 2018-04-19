#include <QtDebug>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDateTime>
#include <QMutex>
#include <QString>
#include <QDir>
#include <qapplication.h>
#include "savelogmessage.h"

static QMutex mutexLogFile;

static QString currentLogFilePath;

void InitLogFileInfo(QString logFileDir, QString selfLogName, int maxLogFileNum)
{
    //Log20180418165041.txt
    static QString currentLogFileDir;
    static QString currentLogFileName;
    int count = 0;

    QDateTime current_date_time =QDateTime::currentDateTime();
    if(true==QDir(logFileDir).exists())
    {
      currentLogFileDir = logFileDir;
    }
    QDir thisDir(currentLogFileDir);
    //qDebug() << current_date_time.toString("yyyyMMddhhmmss");
    currentLogFileName = "Log" + selfLogName + current_date_time.toString("yyyyMMdd") + ".txt";
    currentLogFilePath =  thisDir.filePath(currentLogFileName);
    //qDebug() << currentLogFilePath;

    //创建过滤器，获取日志目录下所有日志文件，并按照文件名（时间）倒序排列
    QStringList filters;
    filters.append("Log" + selfLogName + "*.txt");
    QFileInfoList logFileInfoList	 = thisDir.entryInfoList(filters, QDir::Files|QDir::CaseSensitive, QDir::Name|QDir::Reversed);
    foreach(QFileInfo fileInfo, logFileInfoList)
    {
        count++;
        if(count > maxLogFileNum)
        {
            //qDebug() << fileInfo.filePath();
            //清理过期日志文件
            QFile(fileInfo.filePath()).remove();
        }
    }
}

void SaveLogMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    mutexLogFile.lock();

    QString text;

    switch(type)
    {
    case QtDebugMsg:
        text = QString("Debug:");
        break;

    case QtWarningMsg:
        text = QString("Warning:");
        break;

    case QtCriticalMsg:
        text = QString("Critical:");
        break;

    case QtFatalMsg:
        text = QString("Fatal:");
        break;

    case QtInfoMsg:
        text = QString("Info:");
    }

    QString context_info = QString("File:(%1) Line:(%2)").arg(QString(context.file)).arg(context.line);
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
    QString current_date = QString("(%1)").arg(current_date_time);
    QString message = QString("%1 %2 %3 %4").arg(text).arg(msg).arg(current_date).arg(context_info);

    QFile file(currentLogFilePath);
    //if(file.size()>)
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream << message << "\r\n";
    file.flush();
    file.close();

    mutexLogFile.unlock();
}


/*
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //注册MessageHandler
    qInstallMessageHandler(outputMessage);

    //打印日志到文件中
    qDebug("This is a debug message");
    qWarning("This is a warning message");
    qCritical("This is a critical message");
    qFatal("This is a fatal message");

    return app.exec();
}
 */
