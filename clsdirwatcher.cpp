#include "clsdirwatcher.h"
#include <QDebug>

   clsDirWatcher::clsDirWatcher(QObject *parent):QFileSystemWatcher(parent)
   {

   }

   clsDirWatcher::~clsDirWatcher()
   {

   }
  void clsDirWatcher::updateWatcher_Path(QString newpath)
   {
    qDebug()<<"old watching path:"+watchingPath+"will be removed";
    this->removePath(watchingPath);
    qDebug()<<"new watching path:"+newpath+"will be added";
    watchingPath=newpath;
    this->addPath(watchingPath);
    qDebug()<<"new watching path is"+watchingPath;
    }


