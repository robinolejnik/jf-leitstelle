#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaService>
#include <QAudioOutputSelectorControl>
#include <QMediaPlayer>
#include <QAudioRecorder>

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
    void on_pushButton_clicked();

    void on_comboBox_Lautsprecher_Leitstelle_currentIndexChanged(int index);

    void on_pushButton_save_clicked();

    void on_pushButton_reset_clicked();

private:
    Ui::MainWindow *ui;
    QMediaPlayer *player;
    QMediaPlayer *player2;
    QAudioRecorder *recorder;
    QAudioOutputSelectorControl *out;
};

#endif // MAINWINDOW_H
