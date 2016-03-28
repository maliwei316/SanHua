#include "mainwindow.h"
#include <QApplication>
#include "fileHandleClass.h"
#include <QFileSystemWatcher>
#include "clsdirwatcher.h"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;


//Create three dir watchers to monitor the directory specified in the MainWindow.
// Make mainWindow be the parent of dir watchers ,to make sure they will be released properly when
// application quits.
     clsDirWatcher *dir_watcher_source=new clsDirWatcher(&w);
     clsDirWatcher *dir_watcher_target=new clsDirWatcher(&w);
     clsDirWatcher *dir_watcher_backup=new clsDirWatcher(&w);
     dir_watcher_source->addPath(w.sourcePath);
     dir_watcher_target->addPath(w.targetPath);
     dir_watcher_backup->addPath(w.backupPath);

     fileHandleClass csvRenameObj1(w,&w);
//user changed the path settings or running status, emit signal to let CSV handle object know it.
    QObject::connect(&w,SIGNAL(TextChanged_Source(QString)),&csvRenameObj1,SLOT(updateHandlePath_source(QString)));
    QObject::connect(&w,SIGNAL(TextChanged_Target(QString)),&csvRenameObj1,SLOT(updateHandlePath_target(QString)));
    QObject::connect(&w,SIGNAL(TextChanged_Backup(QString)),&csvRenameObj1,SLOT(updateHandlePath_backup(QString)));

    QObject::connect(&w,SIGNAL(backupFeature_changed(const bool)),&csvRenameObj1,SLOT(updateBackupFeature(const bool)));
    QObject::connect(&w,SIGNAL(runningStatusChanged(const bool)),&csvRenameObj1,SLOT(updateRunStatus(const bool)));
//user changed the parth settings, emit signal to let directory watcher konw it.
    QObject::connect(&w,SIGNAL(TextChanged_Source(QString)),dir_watcher_source,SLOT(updateWatcher_Path(QString)));
    QObject::connect(&w,SIGNAL(TextChanged_Target(QString)),dir_watcher_target,SLOT(updateWatcher_Path(QString)));
    QObject::connect(&w,SIGNAL(TextChanged_Backup(QString)),dir_watcher_backup,SLOT(updateWatcher_Path(QString)));

//If there's any modification to the monitoring dir, the dir watcher will send signal to the CSV handler.
    QObject::connect(dir_watcher_source,SIGNAL(directoryChanged(const QString &)),&csvRenameObj1,SLOT(copyCSV()));
    QObject::connect(dir_watcher_target,SIGNAL(directoryChanged(const QString &)),&csvRenameObj1,SLOT(renameCSV()));
//Connect signal and slots to write log information to specified files and textEdit on Ui

    QObject::connect(&w,SIGNAL(LogEventTriggered_MainWindow(QString,int,int)),&w,SLOT(writeLogToTextEdit(QString,int,int)));
    QObject::connect(&w,SIGNAL(LogEventTriggered_MainWindow(QString,int,int)),&csvRenameObj1,SLOT(writeLogFile(QString,int,int)));

     QObject::connect(&csvRenameObj1,SIGNAL(LogEventTriggered_FileHandle(QString,int,int)),&w,SLOT(writeLogToTextEdit(QString,int,int)));
     QObject::connect(&csvRenameObj1,SIGNAL(LogEventTriggered_FileHandle(QString,int,int)),&csvRenameObj1,SLOT(writeLogFile(QString,int,int)));
    w.show();
    return a.exec();

}
