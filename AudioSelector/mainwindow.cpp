#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    on_pushButton_reset_clicked();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_pushButton_save_clicked() {
    QSettings settings;
    settings.setValue("funk_eingang", ui->comboBox_Funk_Eingang->currentData().toString());
    settings.setValue("funk_ausgang", ui->comboBox_Funk_Ausgang->currentData().toString());
    settings.setValue("hoerer_mikrofon", ui->comboBox_Hoerer_Mikrofon->currentData().toString());
    settings.setValue("headset_mikrofon", ui->comboBox_Headset_Mikrofon->currentData().toString());
    settings.setValue("lautsprecher_wache", ui->comboBox_Lautsprecher_Wache->currentData().toString());
    settings.setValue("hoerer_lautsprecher", ui->comboBox_Hoerer_Lautsprecher->currentData().toString());
    settings.setValue("headset_lautsprecher", ui->comboBox_Headset_Lautsprecher->currentData().toString());
    settings.setValue("lautsprecher_leitstelle", ui->comboBox_Lautsprecher_Leitstelle->currentData().toString());
}

void MainWindow::on_pushButton_reset_clicked() {
    recorder = new QAudioRecorder();
    player = new QMediaPlayer();
    out = qobject_cast<QAudioOutputSelectorControl *>(player->service()->requestControl(QAudioOutputSelectorControl_iid));
    QSettings settings;
    QString funk_eingang = settings.value("funk_eingang").toString();
    QString funk_ausgang = settings.value("funk_ausgang").toString();
    QString hoerer_mikrofon = settings.value("hoerer_mikrofon").toString();
    QString headset_mikrofon = settings.value("headset_mikrofon").toString();
    QString lautsprecher_wache = settings.value("lautsprecher_wache").toString();
    QString hoerer_lautsprecher = settings.value("hoerer_lautsprecher").toString();
    QString headset_lautsprecher = settings.value("headset_lautsprecher").toString();
    QString lautsprecher_leitstelle = settings.value("lautsprecher_leitstelle").toString();

    ui->comboBox_Funk_Ausgang->clear();
    ui->comboBox_Funk_Eingang->clear();
    ui->comboBox_Hoerer_Mikrofon->clear();
    ui->comboBox_Headset_Mikrofon->clear();
    ui->comboBox_Lautsprecher_Wache->clear();
    ui->comboBox_Hoerer_Lautsprecher->clear();
    ui->comboBox_Headset_Lautsprecher->clear();
    ui->comboBox_Lautsprecher_Leitstelle->clear();

    ui->comboBox_Funk_Eingang->addItem("*" + recorder->audioInputDescription(funk_eingang), funk_eingang);
    ui->comboBox_Hoerer_Mikrofon->addItem("*" + recorder->audioInputDescription(hoerer_mikrofon), hoerer_mikrofon);
    ui->comboBox_Headset_Mikrofon->addItem("*" + recorder->audioInputDescription(headset_mikrofon), headset_mikrofon);

    ui->comboBox_Funk_Ausgang->addItem("*" + out->outputDescription(funk_ausgang), funk_ausgang);
    ui->comboBox_Lautsprecher_Wache->addItem("*" + out->outputDescription(lautsprecher_wache), lautsprecher_wache);
    ui->comboBox_Hoerer_Lautsprecher->addItem("*" + out->outputDescription(hoerer_lautsprecher), hoerer_lautsprecher);
    ui->comboBox_Headset_Lautsprecher->addItem("*" + out->outputDescription(headset_lautsprecher), headset_lautsprecher);
    ui->comboBox_Lautsprecher_Leitstelle->addItem("*" + out->outputDescription(lautsprecher_leitstelle), lautsprecher_leitstelle);

    foreach(QString device, out->availableOutputs()) {
        ui->comboBox_Funk_Ausgang->addItem(out->outputDescription(device), device);
        ui->comboBox_Lautsprecher_Wache->addItem(out->outputDescription(device), device);
        ui->comboBox_Hoerer_Lautsprecher->addItem(out->outputDescription(device), device);
        ui->comboBox_Headset_Lautsprecher->addItem(out->outputDescription(device), device);
        ui->comboBox_Lautsprecher_Leitstelle->addItem(out->outputDescription(device), device);
    }
    foreach(QString device, recorder->audioInputs()) {
        ui->comboBox_Funk_Eingang->addItem(recorder->audioInputDescription(device), device);
        ui->comboBox_Hoerer_Mikrofon->addItem(recorder->audioInputDescription(device), device);
        ui->comboBox_Headset_Mikrofon->addItem(recorder->audioInputDescription(device), device);
    }
    delete player;
    delete recorder;
}
