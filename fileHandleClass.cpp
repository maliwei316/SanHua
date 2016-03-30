#include "fileHandleClass.h"
#include <QFileSystemWatcher>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QFileInfo>
#include <QTextStream>
#include <QDateTime>
#include <QMessageBox>


fileHandleClass::fileHandleClass(const MainWindow &w,QObject *parent):QObject(parent)
{
    this->sourcePath=w.sourcePath;
    this->targetPath=w.targetPath;
    this->backupPath=w.backupPath;
    this->backupEnabled=w.backupEnabled;

    QFile file1("log_display.txt");
    //limit the size of the log file:log_dispaly.txt
    if(file1.size()>1048576)
    {
        if(QFile::exists("log_display_old.txt"))
           { QFile::remove("log_display_old.txt");}
        file1.rename("log_display_old.txt");
        //generate an empty "log_display.txt"
        QFile file1("log_display.txt");
        file1.open(QIODevice::WriteOnly|QIODevice::Append);
        file1.close();
        emit LogEventTriggered_FileHandle("log_display's size exceeds 1M, saved as log_display_old.txt",
                                          int(200),int(2));
    }
    file1.open(QIODevice::WriteOnly|QIODevice::Append);

    file1.close();
    //QFileInfo info1("log_display.txt");
    //path_log_display=info1.absoluteFilePath();



    QFile file2("log_debug.txt");
    //limit the size of the log file:log_debug.txt
    if(file2.size()>int(1048576))
    {
        if(QFile::exists("log_debug_old.txt"))
           { QFile::remove("log_debug_old.txt");}
        file2.rename("log_debug_old.txt");
        //generate an empty "log_display.txt"
        QFile file2("log_debug.txt");
        file2.open(QIODevice::WriteOnly|QIODevice::Append);
        file2.close();
        emit LogEventTriggered_FileHandle("log_debug's size exceeds 1M, saved as log_debug_old.txt",int(201),int(2));
    }
    file2.open(QIODevice::WriteOnly|QIODevice::Append);
    file2.close();


}
fileHandleClass::~fileHandleClass()
{

}

QString fileHandleClass::generateNewName(const QString &oldname)
{
//newName:设备号_条码号_日期时间_合格标志.CSV
//S01_45027000AH6P1LS0X821509150086_20150918133950_OK.CSV
    qDebug()<<"generating new name";
    QFile file(oldname);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            emit LogEventTriggered_FileHandle("failed to open file , fine name: "+oldname,int(300));
            return "NB";
        }
        else
        {qDebug()<<"file open OK";}

        QString line=file.readLine();
        qDebug()<<"The first line:"+line;
        QString csvHeader=line;
        QStringList Headers=csvHeader.split(',');
        int index_welderName, index_barcode,index_testTime,index_weldResult;
        index_welderName=0;
        index_testTime=0;
        index_barcode=0;
        index_weldResult=0;

        for(int i=0;i<Headers.size();i++)
        {

          QString str2=Headers[i];
           str2.remove("\n");
          if(str2=="Part Analysis")
          {index_weldResult=i;}
          else if(str2=="工件分析")
          {index_weldResult=i;}
          else if(str2=="Time of Cycle")
          {index_testTime=i;}
          else if(str2=="循环时间")
          {index_testTime=i;}
          else if(str2=="Barcode")
          {index_barcode=i;}
          else if(str2=="条形码")
          {index_barcode=i;}
          else if(str2=="Welder Name")
           {index_welderName=i;}
          else if(str2=="焊机名称")
           {index_welderName=i;}
          else
          {}
          qDebug()<<"Header[i]:"<<i<<Headers[i]<<" " <<str2;
        }
         qDebug()<<"index_testTime="<<index_testTime;
         qDebug()<<"index_WeldResult="<<index_weldResult;
         qDebug()<<"index_Barcode="<<index_barcode;
         qDebug()<<"index_welderName="<<index_welderName;
        QStringList lines;
        //Write the first line into the StringList(Lines)
        lines<<line;
        //Write the rest lines into the StringList(Lines)
        while (!line.isEmpty()) {
            line= file.readLine();
            lines<<line;
            //qDebug()<<"Reading file lines"+line;
        }
        //Cycle data in the lines[lines.size-2]
        qDebug()<<"cycleData:"+lines[lines.size()-2];
         QString cycleData=lines[lines.size()-2];
        //split cycle data into sub strings , and store them into stringlist(strList_cycleData)
         QStringList strList_cycleData=cycleData.split(',');
        //cycle data size >=6, means the file is valid
         if(strList_cycleData.size()>=6)
         {
             QDateTime dt1;
             QString testDateTime;
// "if ,else if"s  below is used to generate the time_filed of the new file name according to the time format
//in the CSV
             if(
                  dt1=QDateTime::fromString(strList_cycleData[index_testTime],"M/dd/yyyy hh:mm:ss A"),
                  testDateTime=dt1.toString("yyyyMMddhhmmss"),
                  !testDateTime.isEmpty()
                ){}

             else if (
                     dt1=QDateTime::fromString(strList_cycleData[index_testTime],"M/dd/yyyy hh:mm:ss"),
                     testDateTime=dt1.toString("yyyyMMddhhmmss"),
                     !testDateTime.isEmpty()
                    ){}
             else if (
                     dt1=QDateTime::fromString(strList_cycleData[index_testTime],"M/dd/yyyy hh:mm"),
                     testDateTime=dt1.toString("yyyyMMddhhmm"),
                     !testDateTime.isEmpty()
                    ){testDateTime+="00";}
             else if (
                     dt1=QDateTime::fromString(strList_cycleData[index_testTime],"M/dd/yyyy h:mm"),
                     testDateTime=dt1.toString("yyyyMMddhhmm"),
                     !testDateTime.isEmpty()
                    ){testDateTime+="00";}
             else if (
                     dt1=QDateTime::fromString(strList_cycleData[index_testTime],"M/dd/yyyy h:mm:ss"),
                     testDateTime=dt1.toString("yyyyMMddhhmmss"),
                     !testDateTime.isEmpty()
                    ){}
             else if (
                     dt1=QDateTime::fromString(strList_cycleData[index_testTime],"M/dd/yyyy h:mm:ss A"),
                     testDateTime=dt1.toString("yyyyMMddhhmmss"),
                     !testDateTime.isEmpty()
                    ){}
             else
             {testDateTime="BAD_Time_Format";}

             //Time Format #1:   12/11/2015 4:47:18 PM
             //Time Format #2:   10/24/2015 18:03
             //QDateTime dt1=QDateTime::fromString(strList_cycleData[index_testTime],"MM/dd/yyyy hh:mm");
             //dt1=QDateTime::fromString(strList_cycleData[index_testTime],"MM/dd/yyyy hh:mm:ss A");
             //Time in CSV before convertion:10/24/2015 18:03  10/24/2015 6:03:45 PM
             //Time in CSV after convertion:20151024180300;
             //QString testDateTime=dt1.toString("yyyyMMddhhmm")+"00";
             //testDateTime=dt1.toString("yyyyMMddhhmmss");
             //if(testDateTime.isEmpty())
             //{
              //  testDateTime="BAD_Time_Format";
             //}
             QString cycleData_weldResult=(strList_cycleData[index_weldResult]=="Good" or strList_cycleData[index_weldResult]=="合格件")? "OK":"NG";
             QString cycleData_barcode;
             if(index_barcode!=0)
             {
                cycleData_barcode=strList_cycleData[index_barcode];

             }
             else
             {
               cycleData_barcode="EMPTY_BARCODE";
             }

             QString newName=strList_cycleData[index_welderName]+"_"+cycleData_barcode+"_"+testDateTime+"_"+cycleData_weldResult+".CSV";
             newName.remove("\n");
             emit LogEventTriggered_FileHandle("fileHandleClass::generateNewName(const QString &oldname) executed,"
                                               "return newname:"+newName,int(202),int(2));
             qDebug()<<"ReturnedNewName:"+newName;
             return newName;
         }
         else
         {
             //The size of cycle data fields less than 6, which means it's a bad file
             emit LogEventTriggered_FileHandle(tr("bad CSV，too less items inside it(less than 6)")+oldname,int(203));
             return "NB";
         }


}
void fileHandleClass::copyCSV()
{
  qDebug()<<"copyCSV executed;";
  if(runStatus)
    {
      qDebug()<<"RunStatus:true";

      QDir dir_source(this->sourcePath);
      QStringList namefilter;
      namefilter<<"*.CSV"<<"*.csv";
      dir_source.setNameFilters(namefilter);
      dir_source.setFilter(QDir::Files | QDir::NoSymLinks);
      //the action above is to set filter, to make sure only CSV file will be recorded into the file list
      QFileInfoList list_csv = dir_source.entryInfoList();
      for (int i = 0; i < list_csv.size(); i++)
       {
          QFileInfo fileInfo = list_csv.at(i);
          QString newName_target=targetPath+"/"+fileInfo.baseName()+".tmp";
          qDebug()<<"tatgetPath:"+targetPath;
          qDebug()<<"newName_target:"+newName_target;
          //if backup feature is checked , then copy the CSV into the backup path
          if(backupEnabled)
          {
              QString newName_backup=backupPath+"/"+fileInfo.baseName()+".csv";
              qDebug()<<"backupPath:"+backupPath;
              qDebug()<<"newName_backup:"+newName_backup;
              if(QFile::copy(fileInfo.absoluteFilePath(),newName_backup))
              {
                emit LogEventTriggered_FileHandle("suceed to copy CSV to backup path,"+newName_backup,int(205));
              }
              else
              {
                emit LogEventTriggered_FileHandle("failed to copy CSV to backup path,"+newName_backup,int(305));
              }
          }
          //Before rename the CSV , the first step is to copy the CSV into the target path without changing its name
          //After finished the copy , then delete the original CSV in the source path
          if(QFile::copy(fileInfo.absoluteFilePath(),newName_target))
          {
              emit LogEventTriggered_FileHandle("succeed to copy CSV to target path,"+newName_target,int(206));
              if(dir_source.remove(fileInfo.fileName()))
              {emit LogEventTriggered_FileHandle("deleted CSV in source path,"+fileInfo.absoluteFilePath(),int(207));}
              else
              {emit LogEventTriggered_FileHandle("failed to delete CSV in source path,"+fileInfo.absoluteFilePath(),int(207));}

          }
          //if failed to copy the CSV to target path , write a record into log file
          else
          {emit LogEventTriggered_FileHandle("Failed to copy CSV to target path,"+newName_target,int(306));}

       }
    }
  else
  {qDebug()<<"RunStatus:false";}
}
void fileHandleClass::renameCSV()
{
 if(runStatus)
 {
     qDebug()<<"renameCSV executed;";
     QDir dir_target(this->targetPath);
     QStringList namefilter;
     namefilter<<"*.TMP"<<"*.tmp";
     dir_target.setNameFilters(namefilter);
     dir_target.setFilter(QDir::Files | QDir::NoSymLinks);
     QFileInfoList list_tmp = dir_target.entryInfoList();
     for (int i = 0; i < list_tmp.size(); i++)
      {
         qDebug()<<"rename processing;";
         QFileInfo fileInfo = list_tmp.at(i);
         //Call the function to rename the tmp file
         QString newName_target=generateNewName(fileInfo.absoluteFilePath());
         //only if newName_target is not "NB", which indicates the above function generated a valid name
         //then execute the renaming action
         if(newName_target!="NB")
         {
             qDebug()<<"newName_target:"+fileInfo.absolutePath()+"/" +newName_target;
             if(dir_target.rename(fileInfo.absoluteFilePath(),fileInfo.absolutePath()+"/" +newName_target))
             {
                emit LogEventTriggered_FileHandle("succeed to rename tmp in target path to csv,old name:"+fileInfo.fileName()+",new name:"+newName_target,int(208));
             }
             else
             {
                emit LogEventTriggered_FileHandle("Failed to rename tmp in target path to csv,old name:"+fileInfo.fileName()+",new name:"+newName_target,int(308));
             }

         }


      }
 }


}
void fileHandleClass::updateHandlePath_source(const QString newPath)
{
    this->sourcePath=newPath;
    qDebug()<<"updateWatcherPath_source executed;";
    emit LogEventTriggered_FileHandle("fileHandleClass::updateHandlePath_source(const QString newPath) executed",int(209),int(2));
}

void fileHandleClass::updateHandlePath_target(const QString newPath)
{
    this->targetPath=newPath;
    qDebug()<<"updateWatcherPath_tatget executed;";
    emit LogEventTriggered_FileHandle("fileHandleClass::updateHandlePath_target(const QString newPath) executed",int(210),int(2));
}

void fileHandleClass::updateHandlePath_backup(const QString newPath)
{
    this->backupPath=newPath;
    emit LogEventTriggered_FileHandle("fileHandleClass::updateHandlePath_backup(const QString newPath) executed",int(211),int(2));
}
void fileHandleClass::updateBackupFeature(const bool checked)
{
    this->backupEnabled=checked;
    qDebug()<<"updateBackupFeature executed;";
    emit LogEventTriggered_FileHandle("fileHandleClass::updateBackupFeature(const bool checked)",int(213),int(2));

}
void fileHandleClass::updateRunStatus(const bool running)
{
    this->runStatus=running;
    emit LogEventTriggered_FileHandle("fileHandleClass::updateRunStatus(const bool running)",int(214),int(2));
}
void fileHandleClass::writeLogFile(QString LogInfo,int eventNO,int eventLevel)
{

   QString currentTime=QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    if(eventLevel==1)
    {
        QFile file("log_display.txt");
        if(file.open(QIODevice::Append|QIODevice::Text))
        {
            QTextStream out(&file);
            out<<(currentTime+" eventNO:"+QString::number(eventNO)+"  "+LogInfo+'\n');
        }
        if(file.isOpen())
        {file.close();}
    }
    else if (eventLevel==2)
    {
        QFile file("log_debug.txt");
        if(file.open(QIODevice::Append|QIODevice::Text))
        {
            QTextStream out(&file);
            out<<(currentTime+" eventNO:"+QString::number(eventNO)+"  "+LogInfo+'\n');
        }
        if(file.isOpen())
        {file.close();}
    }
    else
    {

            QFile file1("log_display.txt");
            if(file1.open(QIODevice::Append|QIODevice::Text))
            {
                QTextStream out1(&file1);
                out1<<(currentTime+" eventNO:"+QString::number(eventNO)+"  "+LogInfo+'\n');
            }
            if(file1.isOpen())
            {file1.close();}


            QFile file2("log_debug.txt");
            if(file2.open(QIODevice::Append|QIODevice::Text))
            {
                QTextStream out2(&file2);
                out2<<(currentTime+" eventNO:"+QString::number(eventNO)+"  "+LogInfo+'\n');
            }
            if(file2.isOpen())
            {file2.close();}

    }
}
