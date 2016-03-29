#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void writeSettings();
    void readSettings();
    void setTextBrowserSource(const QUrl &filename);
signals:
 void TextChanged_Source(const QString &newString);
 void TextChanged_Target(const QString &newString);
 void TextChanged_Backup(const QString &newString);
 void backupFeature_changed(const bool checked);
 void runningStatusChanged(const bool running);
 void timeFormatSettingChanged(const int index);
 void LogEventTriggered_MainWindow(QString LogInfo,int eventNO=0,int eventLevel=0);
private slots:
    void on_actionQuit_triggered();

    void on_PB_SourceSetting_clicked();

    void on_PB_targetPath_clicked();

    void on_PB_BackupPath_clicked();

    void on_checkBox_EnableBackup_toggled(bool checked);

    void on_checkBox_EnableBackup_clicked(bool checked);

    void on_lineEdit_SourcePath_textChanged(const QString &arg1);

    void on_lineEdit_TargetPath_textChanged(const QString &arg1);

    void on_lineEdit_BackupPath_textChanged(const QString &arg1);

    void on_pushButton_Run_clicked();

    void on_pushButton_Pause_clicked();

    void writeLogToTextEdit(QString LogInfo,int eventNO=0,int eventLevel=0);
    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
public:
    QString sourcePath;
    QString targetPath;
    QString backupPath;
    bool backupEnabled;
    bool runningStatus;
    int timeFormat;
};

#endif // MAINWINDOW_H
