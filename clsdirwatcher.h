#ifndef CLSDIRWATCHER_H
#define CLSDIRWATCHER_H

#include <QObject>
#include <QFileSystemWatcher>

class MainWindow;
class clsDirWatcher : public QFileSystemWatcher
{
    Q_OBJECT
public:
    explicit clsDirWatcher(QObject *parent = 0);
    ~clsDirWatcher();
public slots:
    void updateWatcher_Path(QString newpath);
signals:
    void LogEventTriggered_DirWatcher(QString LogInfo,int eventNO=0,int eventLevel=0);
public:
  QString watchingPath;


};

#endif // CLSDIRWATCHER_H
