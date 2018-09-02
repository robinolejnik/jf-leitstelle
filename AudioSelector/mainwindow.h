#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaService>
#include <QAudioOutputSelectorControl>
#include <QMediaPlayer>
#include <QAudioRecorder>
#include <QSettings>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_save_clicked();
    void on_pushButton_reset_clicked();

private:
    Ui::MainWindow *ui;
    QMediaPlayer *player;
    QAudioRecorder *recorder;
    QAudioOutputSelectorControl *out;
};

#endif // MAINWINDOW_H
