#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QFile>
#include <QSettings>
#include <QMessageBox>
#include <QDebug>
#include <QDateTime>
#include <QDir>





MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->readSettings();
    this->runningStatus=false;
    ui->statusBar->showMessage(tr("已停止"));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionQuit_triggered()
{

    emit this->LogEventTriggered_MainWindow(tr("Quitting the program"),int(105));
    emit this->LogEventTriggered_MainWindow("MainWindow::on_actionQuit_triggered()executed",int(107),int (2));
    QApplication::quit();
}

void MainWindow::on_PB_SourceSetting_clicked()
{

    if(!runningStatus)
    {
        QString dir= QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                                   "C:/",
                                                                   QFileDialog::ShowDirsOnly
                                                                   | QFileDialog::DontResolveSymlinks);
        if(dir==targetPath or dir==backupPath)
        {
            QMessageBox::warning(this, tr("Warning"),
                                      tr("源路径，目标路径，备份路径需要互不相同"),
                                      QMessageBox::Ok);

        }
        else
        {
            this->sourcePath=dir;
            ui->lineEdit_SourcePath->setText(dir);
            this->writeSettings();
        }


    }
    else
    {QMessageBox::warning(this, tr("Warning"),
                          tr("修改参数前需停止运行此程序"),
                          QMessageBox::Ok);
                          }
    emit this->LogEventTriggered_MainWindow("MainWindow::on_PB_SourceSetting_clicked() executed",int(106),int (2));
}

void MainWindow::on_PB_targetPath_clicked()
{
    if(!runningStatus)
    {
        QString dir= QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                                   "C:/",
                                                                   QFileDialog::ShowDirsOnly
                                                                   | QFileDialog::DontResolveSymlinks);
        if(dir==sourcePath or dir==backupPath)
        {
            QMessageBox::warning(this, tr("Warning"),
                                      tr("源路径，目标路径，备份路径需要互不相同"),
                                      QMessageBox::Ok);

        }
        else
        {
            this->targetPath=dir;
            ui->lineEdit_TargetPath->setText(dir);
            this->writeSettings();
        }


    }
    else
    {QMessageBox::warning(this, tr("Warning"),
                          tr("修改参数前需停止运行此程序"),
                          QMessageBox::Ok);
                          }
    emit this->LogEventTriggered_MainWindow("MainWindow::on_PB_targetPath_clicked() executed",int(107),int (2));
}

void MainWindow::on_PB_BackupPath_clicked()
{
    if(!runningStatus)
    {
        QString dir= QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                                   "C:/",
                                                                   QFileDialog::ShowDirsOnly
                                                                   | QFileDialog::DontResolveSymlinks);

        if(dir==sourcePath or dir==targetPath)
        {
            QMessageBox::warning(this, tr("Warning"),
                                      tr("源路径，目标路径，备份路径需要互不相同"),
                                      QMessageBox::Ok);

        }
        else
        {
            this->backupPath=dir;
            ui->lineEdit_BackupPath->setText(dir);
            this->writeSettings();
        }


    }
    else
    {QMessageBox::warning(this, tr("Warning"),
                          tr("修改参数前需停止运行此程序"),
                          QMessageBox::Ok);
                          }

    emit this->LogEventTriggered_MainWindow("void MainWindow::on_PB_BackupPath_clicked() executed",int(108),int (2));
}
void MainWindow::on_checkBox_EnableBackup_clicked(bool checked)
{
    if(!runningStatus)
    {
        this->backupEnabled=checked;
        this->writeSettings();
        emit this->LogEventTriggered_MainWindow("void MainWindow::on_checkBox_EnableBackup_clicked(bool checked) executed",int(109),int (2));
    }
    else
    {QMessageBox::warning(this, tr("Warning"),
                          tr("修改参数前需停止运行此程序"),
                          QMessageBox::Ok);

    ui->checkBox_EnableBackup->setChecked((ui->checkBox_EnableBackup->isChecked())?false:true);
    }

}


void MainWindow::on_lineEdit_SourcePath_textChanged(const QString &arg1)
{
    emit this->TextChanged_Source(arg1);
}

void MainWindow::on_lineEdit_TargetPath_textChanged(const QString &arg1)
{
    emit this->TextChanged_Target(arg1);
}

void MainWindow::on_lineEdit_BackupPath_textChanged(const QString &arg1)
{
    emit this->TextChanged_Backup(arg1);
}

void MainWindow::on_checkBox_EnableBackup_toggled(bool checked)
{
    if(!runningStatus)
    emit this->backupFeature_changed(checked);
}
void MainWindow::on_comboBox_currentIndexChanged(int index)
{
 if(!runningStatus)
 {
     this->timeFormat=index;
     this->writeSettings();
     emit this->timeFormatSettingChanged(index);
     emit this->LogEventTriggered_MainWindow("time format changed to index:"+QString::number(index),int(183),int (2));
 }
  else
 {   this->ui->comboBox->setCurrentIndex(this->timeFormat);
     QMessageBox::warning(this, tr("Warning"),
                       tr("修改参数前需停止运行此程序"),
                       QMessageBox::Ok);
                       }

}

void MainWindow::writeSettings()
{
    QSettings settings("dukane.ini",
                       QSettings::IniFormat);
    settings.setValue("sourcePath", this->sourcePath);
    settings.setValue("targetPath", this->targetPath);
    settings.setValue("backupPath", this->backupPath);
    settings.setValue("backupEnabled", this->backupEnabled);
    settings.setValue("timeFormat",this->timeFormat);
    emit this->LogEventTriggered_MainWindow("void MainWindow::writeSettings() executed,Source:"+sourcePath+
                                            ",Target:"+targetPath+",backup:"+(backupEnabled?"true":"false")+",path:"+backupPath,int(104),int(2));

}

void MainWindow::readSettings()
{

    if(QFile::exists("dukane.ini"))
    {
        qDebug()<<"found dukane.ini";

        QSettings settings("dukane.ini",
                           QSettings::IniFormat);
        this->sourcePath=settings.value("sourcePath").toString();
        this->targetPath=settings.value("targetPath").toString();
        this->backupPath=settings.value("backupPath").toString();
        this->backupEnabled=settings.value("backupEnabled").toBool();
        this->timeFormat=settings.value("timeFormat").toInt();
        if(-1==this->timeFormat)
        {this->timeFormat=0;}
        ui->lineEdit_SourcePath->setText(this->sourcePath);
        ui->lineEdit_TargetPath->setText(this->targetPath);
        ui->lineEdit_BackupPath->setText(this->backupPath);
        ui->checkBox_EnableBackup->setChecked(this->backupEnabled);
        ui->comboBox->setCurrentIndex(this->timeFormat);
        emit this->LogEventTriggered_MainWindow(tr("found dukane.ini ,will use configuration inside it"),int(103),int(2));

    }
    else
    {
       //try to make path for source,target and backup
        QDir dir1;
       if(dir1.mkpath("C:/Dukane/SourceData/"))
       {qDebug()<<"C:/Dukane/SourceData created";}
       else
       {qDebug()<<"C:/Dukane/SourceData  creation failed";}
       if(dir1.mkpath("C:/Dukane/TargetData/"))
       {qDebug()<<"C:/Dukane/TargetData created";}
       else
       {qDebug()<<"C:/Dukane/TargetData creation failed";}

       if(dir1.mkpath("C:/Dukane/BackupedData/"))
       {qDebug()<<"C:/Dukane/BackupedData created";}
       else
       {qDebug()<<"C:/Dukane/BackupedData creation failed";}
       //initialize the path with default strings
       if (dir1.exists("C:/Dukane/SourceData"))
       {
         this->sourcePath="C:/Dukane/SourceData";
         this->ui->lineEdit_SourcePath->setText(sourcePath);
       }
        if(dir1.exists("C:/Dukane/TargetData"))
        {
            this->targetPath="C:/Dukane/TargetData";
            this->ui->lineEdit_TargetPath->setText(targetPath);
        }
        if(dir1.exists("C:/Dukane/BackupedData"))
        {
           this->backupPath="C:/Dukane/BackupedData";
           this->ui->lineEdit_BackupPath->setText(backupPath);
        }

        this->backupEnabled=true;
        this->ui->checkBox_EnableBackup->setChecked(backupEnabled);
        this->timeFormat=int(0);
        this->ui->comboBox->setCurrentIndex(timeFormat);

        QSettings settings("dukane.ini",
                           QSettings::IniFormat);
        settings.setValue("sourcePath", this->sourcePath);
        settings.setValue("targetPath", this->targetPath);
        settings.setValue("backupPath", this->backupPath);
        settings.setValue("backupEnabled", this->backupEnabled);
        settings.setValue("timeFormat",this->timeFormat);
       emit this->LogEventTriggered_MainWindow(tr("did not found dukane.ini , will use default settings"),int(102));
    }


}

void MainWindow::on_pushButton_Run_clicked()
{
    runningStatus=true;
    emit this->runningStatusChanged(runningStatus);
    ui->statusBar->showMessage(tr("正在运行"));
    emit this->LogEventTriggered_MainWindow(tr("start to run"),int(101));
}

void MainWindow::on_pushButton_Pause_clicked()
{
    runningStatus=false;
    emit this->runningStatusChanged(runningStatus);
    ui->statusBar->showMessage(tr("已停止"));
    emit this->LogEventTriggered_MainWindow(tr("stopped"),int(100));
}

void MainWindow::writeLogToTextEdit(QString LogInfo,int eventNO,int eventLevel)
{
    if(eventLevel!=2)
    {
        QString currentTime=QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
        ui->textEdit->append(currentTime+" eventNO:"+QString::number(eventNO)+"  "+LogInfo+'\n');
    }
}


