#include "touchview.h"
#include "ui_touchview.h"

TouchView::TouchView(QWidget *parent) : QWidget(parent), ui(new Ui::TouchView) {
    ui->setupUi(this);
    QSettings settings("config.ini", QSettings::IniFormat);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    timer->start(1000);
    durchsage = new Durchsage();
    showTime();

    QDir tel_dir(settings.value("tel_list").toString(), "*.wav", QDir::Time, QDir::Files);
    QFileInfoList l = tel_dir.entryInfoList(QDir::Files, QDir::Time);
    foreach(QFileInfo f, l) {
        QListWidgetItem *item = new QListWidgetItem(f.fileName() + "\n" + f.created().toString("dd.MM.yyyy hh:mm:ss"));
        item->setData(Qt::UserRole, QVariant(f.absoluteFilePath()));
        ui->listWidget_tel->addItem(item);
    }

    int size = settings.beginReadArray("fahrzeuge");
    for(int i=0;i<size;i++) {
        settings.setArrayIndex(i);
        QListWidgetItem *item = new QListWidgetItem("\n" + settings.value("name").toString() + "\n");
        item->setData(Qt::UserRole, QVariant(settings.value("name")));
        item->setData(Qt::UserRole+1, QVariant(settings.value("file")));
        ui->listWidget_melder->addItem(item);
    }
    settings.endArray();
    ui->toolButton_tel->setChecked(true);
    playlist = new QMediaPlaylist;
    player = new QMediaPlayer;
    player->setPlaylist(playlist);
    on_toolButton_tel_clicked();
    ui->toolButton_tel->setChecked(true);
    ui->label_anzahl_ausdrucke->clear();
    ui->label_einsatztext->clear();
    ui->label_fahrzeuge->clear();
}

TouchView::~TouchView() {
    while(ui->listWidget_tel->count()>0) {
        QListWidgetItem *item = ui->listWidget_tel->item(0);
        ui->listWidget_tel->removeItemWidget(item);
        delete item;
    }
    delete ui;
}

void TouchView::on_toolButton_tel_recordings_clicked() {
    ui->toolButton_tel->setChecked(false);
    ui->toolButton_melder->setChecked(false);
    ui->toolButton_wachalarm->setChecked(false);
    ui->stackedWidget->setCurrentWidget(ui->page_tel_recordings);
}

void TouchView::on_toolButton_melder_clicked() {
    ui->toolButton_tel->setChecked(false);
    ui->toolButton_tel_recordings->setChecked(false);
    ui->toolButton_wachalarm->setChecked(false);
    ui->stackedWidget->setCurrentWidget(ui->page_melder);
}

void TouchView::on_toolButton_wachalarm_clicked() {
    ui->toolButton_tel->setChecked(false);
    ui->toolButton_tel_recordings->setChecked(false);
    ui->toolButton_melder->setChecked(false);
    ui->stackedWidget->setCurrentWidget(ui->page_wachalarm);
}

void TouchView::showTime() {
    QTime time = QTime::currentTime();
    QDate date = QDate::currentDate();
    ui->label_uhrzeit->setText(time.toString("hh:mm:ss"));
    ui->label_datum->setText(date.toString("dd.MM.yyyy"));
}

void TouchView::setEinsatz(Einsatz &e) {
    einsatz = e;
    ui->label_einsatztext->setText(einsatz.einsatztext());
    QString fahrzeuge;
    if(einsatz.fahrzeuge_name.size()>0)
        fahrzeuge = einsatz.fahrzeuge_name[0];
    for(int i=1;i<einsatz.fahrzeuge_name.size();i++)
        fahrzeuge += ", " + einsatz.fahrzeuge_name[i];
    ui->label_fahrzeuge->setText(fahrzeuge);
    ui->label_anzahl_ausdrucke->setText(QString::number(einsatz.fahrzeuge_name.size()));
    ui->pushButton_einzelausdruck->setEnabled(true);
    ui->toolButton_alarmieren->setEnabled(true);
}

void TouchView::resetView() {
    ui->label_einsatztext->setText("");
    ui->label_fahrzeuge->setText("");
    ui->label_anzahl_ausdrucke->setText("");
    ui->pushButton_einzelausdruck->setEnabled(false);
    ui->toolButton_alarmieren->setEnabled(false);
}

void TouchView::on_listWidget_tel_currentItemChanged(QListWidgetItem *, QListWidgetItem *) {
    player->stop();
    playlist->clear();
}

void TouchView::on_pushButton_tel_play_clicked() {
    if(ui->listWidget_tel->currentItem()) {
        player->stop();
        QProcess::execute("headset.exe");
        playlist->clear();
        playlist->addMedia(QUrl::fromLocalFile(ui->listWidget_tel->currentItem()->data(Qt::UserRole).toString()));
        player->play();
    }
}

void TouchView::on_pushButton_tel_stop_clicked() {
    player->stop();
    playlist->clear();
}

void TouchView::on_pushButton_tel_delete_clicked() {
    if(ui->listWidget_tel->currentItem()) {
        QFile file(ui->listWidget_tel->currentItem()->data(Qt::UserRole).toString());
        file.remove();
        on_pushButton_tel_refresh_clicked();
    }
}

void TouchView::on_pushButton_tel_refresh_clicked() {
    QSettings settings("config.ini", QSettings::IniFormat);
    player->stop();
    playlist->clear();
    while(ui->listWidget_tel->count()>0) {
        QListWidgetItem *item = ui->listWidget_tel->item(0);
        ui->listWidget_tel->removeItemWidget(item);
        delete item;
    }
    QDir tel_dir(settings.value("tel_list").toString(), "*.wav", QDir::Time, QDir::Files);
    QFileInfoList l = tel_dir.entryInfoList(QDir::Files, QDir::Time);
    foreach(QFileInfo f, l) {
        QListWidgetItem *item = new QListWidgetItem(f.fileName() + "\n" + f.created().toString("dd.MM.yyyy hh:mm:ss"));
        item->setData(Qt::UserRole, QVariant(f.absoluteFilePath()));
        ui->listWidget_tel->addItem(item);
    }
}

void TouchView::on_pushButton_kurzwahl_1_clicked() {
    QDesktopServices::openUrl(QUrl("tel:**610"));
}

void TouchView::on_pushButton_kurzwahl_2_clicked() {
    QDesktopServices::openUrl(QUrl("tel:**611"));
}

void TouchView::on_pushButton_kurzwahl_3_clicked() {
    QDesktopServices::openUrl(QUrl("tel:**612"));
}

void TouchView::on_pushButton_kurzwahl_4_clicked() {
    QDesktopServices::openUrl(QUrl("tel:**613"));
}

void TouchView::on_toolButton_tel_abheben_clicked() {
    emit neuerEinsatz();
    QProcess::startDetached("abheben.exe");
}

void TouchView::on_pushButton_tel_trennen_clicked() {
    QProcess::startDetached("trennen.exe");
    QTimer::singleShot(1000, this, SLOT(on_pushButton_tel_refresh_clicked()));
}

void TouchView::on_pushButton_melder_senden_clicked(){
    foreach(QListWidgetItem *item, ui->listWidget_melder->selectedItems()) {
        QFile file(item->data(Qt::UserRole+1).toString());
        if(file.open(QFile::WriteOnly | QFile::Truncate | QFile::Text)) {
            QTextStream stream(&file);
            stream << ui->textBrowser_melder->toPlainText();
            file.close();
        }
    }
}

void TouchView::on_pushButton_aufnahme_clicked() {
    if(ui->pushButton_aufnahme->text()=="Aufnahme\nstarten") {
        player->stop();
        playlist->clear();
        durchsage->record();
        ui->pushButton_aufnahme->setText("Aufnahme\nbeenden");
    }
    else if(ui->pushButton_aufnahme->text()=="Aufnahme\nbeenden") {
        player->stop();
        playlist->clear();
        durchsage->stop();
        ui->pushButton_aufnahme->setText("Aufnahme\nstarten");
    }
}

void TouchView::on_toolButton_play_clicked() {
    player->stop();
    QProcess::execute("lautsprecher_durchsage.exe");
    playlist->clear();
    playlist->addMedia(QUrl::fromLocalFile(durchsage->filename));
    player->play();
}

void TouchView::on_toolButton_play_feuergong_clicked() {
    player->stop();
    QProcess::execute("lautsprecher_durchsage.exe");
    playlist->clear();
    playlist->addMedia(QUrl("qrc:/feuergong.mp3"));
    playlist->addMedia(QUrl::fromLocalFile(durchsage->filename));
    player->play();
}

void TouchView::on_toolButton_play_rettungsgong_clicked() {
    player->stop();
    QProcess::execute("lautsprecher_durchsage.exe");
    playlist->clear();
    playlist->addMedia(QUrl("qrc:/rettungsgong.mp3"));
    playlist->addMedia(QUrl::fromLocalFile(durchsage->filename));
    player->play();
}

void TouchView::on_toolButton_play_einzelfahrzeug_clicked() {
    player->stop();
    QProcess::execute("lautsprecher_durchsage.exe");
    playlist->clear();
    playlist->addMedia(QUrl("qrc:/einzelfahrzeug.mp3"));
    playlist->addMedia(QUrl::fromLocalFile(durchsage->filename));
    player->play();
}

void TouchView::on_toolButton_play_durchsage_clicked() {
    player->stop();
    QProcess::execute("lautsprecher_durchsage.exe");
    playlist->clear();
    playlist->addMedia(QUrl("qrc:/durchsage.mp3"));
    playlist->addMedia(QUrl::fromLocalFile(durchsage->filename));
    player->play();
}

void TouchView::on_toolButton_feuergong_clicked() {
    player->stop();
    QProcess::execute("lautsprecher_durchsage.exe");
    playlist->clear();
    playlist->addMedia(QUrl("qrc:/feuergong.mp3"));
    player->play();
}

void TouchView::on_toolButton_rettungsgong_clicked() {
    player->stop();
    QProcess::execute("lautsprecher_durchsage.exe");
    playlist->clear();
    playlist->addMedia(QUrl("qrc:/rettungsgong.mp3"));
    player->play();
}

void TouchView::on_toolButton_einzelfahrzeug_clicked() {
    player->stop();
    QProcess::execute("lautsprecher_durchsage.exe");
    playlist->clear();
    playlist->addMedia(QUrl("qrc:/einzelfahrzeug.mp3"));
    player->play();;
}

void TouchView::on_toolButton_durchsage_clicked() {
    player->stop();
    QProcess::execute("lautsprecher_durchsage.exe");
    playlist->clear();
    playlist->addMedia(QUrl("qrc:/durchsage.mp3"));
    player->play();
}

void TouchView::on_pushButton_aufnahme_anhoeren_clicked() {
    player->stop();
    QProcess::execute("headset.exe");
    playlist->clear();
    playlist->addMedia(QUrl::fromLocalFile(durchsage->filename));
    player->play();
}

void TouchView::on_toolButton_tel_clicked() {
    ui->toolButton_tel_recordings->setChecked(false);
    ui->toolButton_melder->setChecked(false);
    ui->toolButton_wachalarm->setChecked(false);
    ui->stackedWidget->setCurrentWidget(ui->page_telefon);
}

void TouchView::on_toolButton_leitstelle_clicked() {
    player->stop();
    QProcess::execute("lautsprecher_durchsage.exe");
    playlist->clear();
    playlist->addMedia(QUrl("qrc:/leitstelle.wav"));
    player->play();
}

void TouchView::on_toolButton_play_feuergong_2_clicked() {
    player->stop();
    QProcess::execute("lautsprecher_durchsage.exe");
    playlist->clear();
    playlist->addMedia(QUrl("qrc:/feuergong.mp3"));
    playlist->addMedia(QUrl::fromLocalFile(durchsage->filename));
    player->play();
}

void TouchView::on_toolButton_play_rettungsgong_2_clicked() {
    player->stop();
    QProcess::execute("lautsprecher_durchsage.exe");
    playlist->clear();
    playlist->addMedia(QUrl("qrc:/rettungsgong.mp3"));
    playlist->addMedia(QUrl::fromLocalFile(durchsage->filename));
    player->play();
}

void TouchView::on_toolButton_play_einzelfahrzeug_2_clicked() {
    player->stop();
    QProcess::execute("lautsprecher_durchsage.exe");
    playlist->clear();
    playlist->addMedia(QUrl("qrc:/einzelfahrzeug.mp3"));
    playlist->addMedia(QUrl::fromLocalFile(durchsage->filename));
    player->play();
}

void TouchView::on_toolButton_play_durchsage_2_clicked() {
    player->stop();
    QProcess::execute("lautsprecher_durchsage.exe");
    playlist->clear();
    playlist->addMedia(QUrl("qrc:/durchsage.mp3"));
    playlist->addMedia(QUrl::fromLocalFile(durchsage->filename));
    player->play();
}

void TouchView::on_pushButton_aufnahme_2_clicked() {
    if(ui->pushButton_aufnahme_2->text()=="Aufnahme\nstarten") {
        player->stop();
        playlist->clear();
        durchsage->record();
        ui->pushButton_aufnahme_2->setText("Aufnahme\nbeenden");
    }
    else if(ui->pushButton_aufnahme_2->text()=="Aufnahme\nbeenden") {
        player->stop();
        playlist->clear();
        durchsage->stop();
        ui->pushButton_aufnahme_2->setText("Aufnahme\nstarten");
    }
}

void TouchView::on_pushButton_aufnahme_anhoeren_2_clicked() {
    player->stop();
    QProcess::execute("headset.exe");
    playlist->clear();
    playlist->addMedia(QUrl::fromLocalFile(durchsage->filename));
    player->play();
}

void TouchView::on_pushButton_play_2_clicked() {
    player->stop();
    QProcess::execute("lautsprecher_durchsage.exe");
    playlist->clear();
    playlist->addMedia(QUrl::fromLocalFile(durchsage->filename));
    player->play();
}

void TouchView::on_toolButton_leitstelle_2_clicked() {
    player->stop();
    QProcess::execute("lautsprecher_durchsage.exe");
    playlist->clear();
    playlist->addMedia(QUrl("qrc:/leitstelle.wav"));
    player->play();
}

void TouchView::undisponierte_einsaetze() {
    if(player->state()==QMediaPlayer::StoppedState) {
        player->stop();
        QProcess::execute("lautsprecher_leitstelle.exe");
        playlist->clear();
        playlist->addMedia(QUrl("qrc:/undisponierte_einsaetze.wav"));
        player->play();
    }
}

void TouchView::on_toolButton_alarmieren_clicked() {
    emit EinsatzDisponiert(einsatz.einsatznummer);
    if(einsatz.einsatznummer>0 && einsatz.einsatztext()!="") {
        einsatz.zeit_alarm = QTime::currentTime();
        einsatz.datum_alarm = QDate::currentDate();
        Printer p;
        for(int i=0;i<einsatz.fahrzeuge_name.size();i++)
            p.print(einsatz);

        foreach(QString filename, einsatz.fahrzeuge_file) {
            QFile file(filename);
            if(file.open(QFile::WriteOnly | QFile::Truncate | QFile::Text)) {
                QTextStream stream(&file);
                stream << einsatz.einsatztext();
                file.close();
            }
        }
    }
}

void TouchView::on_pushButton_einzelausdruck_clicked() {
    if(einsatz.einsatznummer>0 && einsatz.einsatztext()!="") {
        Printer p;
        p.print(einsatz);
    }
}
