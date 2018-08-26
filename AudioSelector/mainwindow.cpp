#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    recorder = new QAudioRecorder();
    player = new QMediaPlayer();
    player2 = new QMediaPlayer();
    player->setMedia(QUrl::fromLocalFile("C:/Users/robin/Desktop/JF-Leitstelle/extra/Gong/Gong-Durchsage-BF-Hamm.mp3"));
    player2->setMedia(QUrl::fromLocalFile("C:/Users/robin/Desktop/JF-Leitstelle/extra/Gong/Gong-Durchsage-BF-Hamm.mp3"));
    player->setVolume(100);
    player2->setVolume(100);

    out = qobject_cast<QAudioOutputSelectorControl *>(player->service()->requestControl(QAudioOutputSelectorControl_iid));

    foreach(QString device, out->availableOutputs()) {
        ui->comboBox_Funk_Senden->addItem(out->outputDescription(device), device);
        ui->comboBox_Lautsprecher_Wache->addItem(out->outputDescription(device), device);
        ui->comboBox_Headset_Lautsprecher->addItem(out->outputDescription(device), device);
        ui->comboBox_Lautsprecher_Leitstelle->addItem(out->outputDescription(device), device);
    }
    foreach(QString device, recorder->audioInputs()) {
        ui->comboBox_Funk_Empfangen->addItem(recorder->audioInputDescription(device), device);
        ui->comboBox_Headset_Mikrofon->addItem(recorder->audioInputDescription(device), device);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    player->play();
    player2->play();
}

void MainWindow::on_comboBox_Lautsprecher_Leitstelle_currentIndexChanged(int index)
{
    QString device = ui->comboBox_Lautsprecher_Leitstelle->currentData().toString();
    out->setActiveOutput(device);
}
