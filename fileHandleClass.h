#ifndef FILEHANDLECLASS_H
#define FILEHANDLECLASS_H

#include <QObject>
#include "mainwindow.h"


class MainWindow;
class QFile;
class fileHandleClass : public QObject
{
    Q_OBJECT
public:
    explicit fileHandleClass(const MainWindow &w, QObject *parent = 0);
    ~fileHandleClass();
    QString generateNewName(const QString &oldname);



signals:
     void LogEventTriggered_FileHandle(QString LogInfo,int eventNO=0,int eventLevel=0);

public slots:
     void copyCSV();
     void renameCSV();
     void updateHandlePath_source(const QString newPath);
     void updateHandlePath_target(const QString newPath);
     void updateHandlePath_backup(const QString newPath);
     void updateBackupFeature(const bool checked);
     void updateRunStatus(const bool running);
     void updateTimeFormat(const int index);
     void writeLogFile(QString LogInfo,int eventNO=0,int eventLevel=0);
public:

    QString sourcePath;
    QString targetPath;
    QString backupPath;
    bool backupEnabled;
    bool runStatus;
    int timeFormat;
    //QString path_log_display;

};

#endif // FILEHANDLECLASS_H
