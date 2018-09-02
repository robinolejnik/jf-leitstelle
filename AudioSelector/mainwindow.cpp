#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    recorder = new QAudioRecorder();
    out = qobject_cast<QAudioOutputSelectorControl *>(player->service()->requestControl(QAudioOutputSelectorControl_iid));

    foreach(QString device, out->availableOutputs()) {
        ui->comboBox_Hoerer_Lautsprecher->addItem(out->outputDescription(device), device);
        ui->comboBox_Funk_Ausgang->addItem(out->outputDescription(device), device);
        ui->comboBox_Lautsprecher_Wache->addItem(out->outputDescription(device), device);
        ui->comboBox_Headset_Lautsprecher->addItem(out->outputDescription(device), device);
        ui->comboBox_Lautsprecher_Leitstelle->addItem(out->outputDescription(device), device);
    }
    foreach(QString device, recorder->audioInputs()) {
        ui->comboBox_Funk_Eingang->addItem(recorder->audioInputDescription(device), device);
        ui->comboBox_Headset_Mikrofon->addItem(recorder->audioInputDescription(device), device);
        ui->comboBox_Hoerer_Mikrofon->addItem(recorder->audioInputDescription(device), device);
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

void MainWindow::on_pushButton_save_clicked()
{

}

void MainWindow::on_pushButton_reset_clicked()
{

}
