#include "einsatzform.h"
#include "ui_einsatzform.h"

EinsatzForm::EinsatzForm(QWidget *parent) : QWidget(parent), ui(new Ui::EinsatzForm) {
    ui->setupUi(this);
    QList<Einsatz> list = Einsatz::load();
    foreach(Einsatz e, list) {
        QListWidgetItem *item = new QListWidgetItem(e.stichwort + "\n" + e.strasse + " " + e.hausnummer + "\n" + e.objekt);
        if(e.stichwort.contains("BMA Automatische Feuermeldung"))
            item->setIcon(QIcon(":/icon/building.png"));
        else if(e.stichwort.contains("Feuer"))
            item->setIcon(QIcon(":/icon/fire.png"));
        else if(e.stichwort.contains("TH"))
            item->setIcon(QIcon(":/icon/axes.png"));
        else
            item->setIcon(QIcon(":/icon/helmet.png"));
        item->setData(Qt::UserRole, qVariantFromValue(e));
        ui->listWidget->addItem(item);
    }
    QFile file("config/streets.txt");
    if(file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        in.setCodec("UTF-8");
           while (!in.atEnd())
               ui->comboBox_strasse->addItem(in.readLine());
           file.close();
    }
    else {
        qWarning("steets.txt not found");
    }
    file.setFileName("config/stichworte.txt");
    if(file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        in.setCodec("UTF-8");
           while (!in.atEnd())
               ui->comboBox_stichwort->addItem(in.readLine());
           file.close();
    }
    else {
        qWarning("stichworte.txt not found");
    }

    QSettings settings("config/config.ini", QSettings::IniFormat);
    int size = settings.beginReadArray("fahrzeuge");
    for(int i=0;i<size;i++) {
        settings.setArrayIndex(i);
        QListWidgetItem *item = new QListWidgetItem("\n" + settings.value("name").toString() + "\n");
        item->setData(Qt::UserRole, QVariant(settings.value("name")));
        item->setData(Qt::UserRole+1, QVariant(settings.value("file")));
        ui->listWidget_fahrzeuge->addItem(item);
    }
    settings.endArray();
    on_pushButton_reset_clicked();
    checktimer = new QTimer(this);
    connect(checktimer, SIGNAL(timeout()), this, SLOT(tick()));
    checktimer->start(30000);

    socket = new QUdpSocket(this);
    socket->bind(QHostAddress::Any, 15863);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
}

void EinsatzForm::readPendingDatagrams() {
    while(socket->hasPendingDatagrams()) {
        QJsonDocument doc(QJsonDocument::fromJson(socket->receiveDatagram().data()));
        QJsonObject json = doc.object();

        Einsatz e;
        e.einsatznummer = Einsatz::Einsatznummer();
        e.strasse = json["strasse"].toString();
        e.hausnummer = json["hausnummer"].toString();
        e.stichwort = json["stichwort"].toString();
        e.ort = json["ort"].toString();
        e.bemerkung = json["bemerkung"].toString();
        e.objekt = json["objekt"].toString();
        e.anrufer = "Brandmeldeanlage";
        e.anrufer_telefonnummer = json["bma_nummer"].toString();
        e.sonderrechte = true;

        DialogBMA bma;
        bma.setEinsatz(e);
        bma.exec();
        on_pushButton_reset_clicked();
        foreach(QListWidgetItem *item, ui->listWidget_fahrzeuge->selectedItems()) {
            e.fahrzeuge_file.append(item->data(Qt::UserRole+1).toString());
            e.fahrzeuge_name.append(item->data(Qt::UserRole).toString());
        }

        ui->label_einsatznummer->setText(QString::number(e.einsatznummer));
        QListWidgetItem *item = new QListWidgetItem(e.stichwort + "\n" + e.strasse + " " + e.hausnummer + "\n" + e.objekt);
        item->setIcon(QIcon(":/icon/building.png"));
        item->setData(Qt::UserRole, qVariantFromValue(e));
        ui->listWidget->addItem(item);
        QList<Einsatz> list;
        for(int i=0;i<ui->listWidget->count();i++)
            list.append(ui->listWidget->item(i)->data(Qt::UserRole).value<Einsatz>());
        touchview->setEinsatz(e);
        karte->setEinsatz(e);
        Einsatz::save(list);
    }
}

void EinsatzForm::tick() {
    for(int i=0;i<ui->listWidget->count();i++) {
        QListWidgetItem *item = ui->listWidget->item(i);
        QString zeit_alarm = item->data(Qt::UserRole).value<Einsatz>().zeit_alarm.toString("hh:mm:ss");
        QTime zeit_annahme = item->data(Qt::UserRole).value<Einsatz>().zeit_annahme;
        QDate datum_annahme = item->data(Qt::UserRole).value<Einsatz>().datum_annahme;
        QDateTime annahme(datum_annahme, zeit_annahme);
        if(zeit_alarm.isEmpty() && annahme.secsTo(QDateTime::currentDateTime()) > 60) {
            touchview->undisponierte_einsaetze();
        }
    }
}

EinsatzForm::~EinsatzForm() {
    while(ui->listWidget->count()>0) {
        QListWidgetItem *item = ui->listWidget->item(0);
        ui->listWidget->removeItemWidget(item);
        delete item;
    }
    while(ui->listWidget_fahrzeuge->count()>0) {
        QListWidgetItem *item = ui->listWidget_fahrzeuge->item(0);
        ui->listWidget_fahrzeuge->removeItemWidget(item);
        delete item;
    }
    delete ui;
}

void EinsatzForm::on_pushButton_save_clicked() {
    if(ui->comboBox_strasse->currentText()=="" || ui->comboBox_stichwort->currentText()=="") {
        QMessageBox::critical(this, "Fehler", "Gebe mindestens eine Straße und ein Alarmstichwort an");
        return;
    }
    Einsatz e;
    e.anfahrt = ui->lineEdit_anfahrt->text();
    e.anrufer = ui->lineEdit_anrufer->text();
    e.anrufer_telefonnummer = ui->lineEdit_telefonnummer->text();
    e.bemerkung = ui->textEdit_bemerkung->toPlainText();
    e.geschaedigter = ui->lineEdit_geschaedigter->text();
    e.objekt = ui->lineEdit_objekt->text();
    e.strasse = ui->comboBox_strasse->currentText();
    e.ort = ui->lineEdit_ort->text();
    e.sonderrechte = ui->checkBox_sosi->isChecked();
    e.hausnummer = ui->lineEdit_hnr->text();
    e.datum_alarm = QDate::fromString(ui->label_alarmdatum->text(), "dd.MM.yyyy");
    e.zeit_alarm = QTime::fromString(ui->label_alarmzeit->text(), "hh:mm:ss");
    e.datum_annahme = QDate::fromString(ui->label_annahmedatum->text(), "dd.MM.yyyy");
    e.zeit_annahme = QTime::fromString(ui->label_annahmezeit->text(), "hh:mm:ss");
    e.stichwort = ui->comboBox_stichwort->currentText();
    e.rueckmeldungen = ui->textEdit_rueckmeldungen->toPlainText();
    e.einsatznummer = ui->label_einsatznummer->text().toInt();

    foreach(QListWidgetItem *item, ui->listWidget_fahrzeuge->selectedItems()) {
        e.fahrzeuge_file.append(item->data(Qt::UserRole+1).toString());
        e.fahrzeuge_name.append(item->data(Qt::UserRole).toString());
    }

    if(ui->label_einsatznummer->text().toInt()==0) {   // new
        ui->label_annahmedatum->setText(QDate::currentDate().toString("dd.MM.yyyy"));
        ui->label_annahmezeit->setText(QTime::currentTime().toString("hh:mm:ss"));
        ui->label_alarmdatum->clear();
        ui->label_alarmzeit->clear();
        e.einsatznummer = Einsatz::Einsatznummer();
        ui->label_einsatznummer->setText(QString::number(e.einsatznummer));
        QListWidgetItem *item = new QListWidgetItem(e.stichwort + "\n" + e.strasse + " " + e.hausnummer + "\n" + e.objekt);
        if(e.stichwort.contains("BMA Automatische Feuermeldung"))
            item->setIcon(QIcon(":/icon/building.png"));
        else if(e.stichwort.contains("Feuer"))
            item->setIcon(QIcon(":/icon/fire.png"));
        else if(e.stichwort.contains("TH"))
            item->setIcon(QIcon(":/icon/axes.png"));
        else
            item->setIcon(QIcon(":/icon/helmet.png"));
        item->setData(Qt::UserRole, qVariantFromValue(e));
        ui->listWidget->addItem(item);
    }
    else {      // existing
        for(int i=0;i<ui->listWidget->count();i++) {
            QListWidgetItem *item = ui->listWidget->item(i);
            int enr = item->data(Qt::UserRole).value<Einsatz>().einsatznummer;
            if(enr==ui->label_einsatznummer->text().toInt()) {
                item->setText(e.stichwort + "\n" + e.strasse + " " + e.hausnummer + "\n" + e.objekt);
                if(e.stichwort.contains("BMA Automatische Feuermeldung"))
                    item->setIcon(QIcon(":/icon/building.png"));
                else if(e.stichwort.contains("Feuer"))
                    item->setIcon(QIcon(":/icon/fire.png"));
                else if(e.stichwort.contains("TH"))
                    item->setIcon(QIcon(":/icon/axes.png"));
                else
                    item->setIcon(QIcon(":/icon/helmet.png"));
                item->setData(Qt::UserRole, qVariantFromValue(e));
            }
        }
    }
    QList<Einsatz> list;
    for(int i=0;i<ui->listWidget->count();i++)
        list.append(ui->listWidget->item(i)->data(Qt::UserRole).value<Einsatz>());
    touchview->setEinsatz(e);
    karte->setEinsatz(e);
    Einsatz::save(list);
}

void EinsatzForm::on_pushButton_delete_clicked() {
    QListWidgetItem *item = ui->listWidget->currentItem();
    if(item) {
        ui->listWidget->removeItemWidget(item);
        delete item;
    }
    QList<Einsatz> list;
    for(int i=0;i<ui->listWidget->count();i++)
        list.append(ui->listWidget->item(i)->data(Qt::UserRole).value<Einsatz>());
    Einsatz::save(list);
    on_pushButton_reset_clicked();
}

void EinsatzForm::on_pushButton_reset_clicked() {
    ui->label_annahmedatum->setText(QDate::currentDate().toString("dd.MM.yyyy"));
    ui->label_annahmezeit->setText(QTime::currentTime().toString("hh:mm:ss"));
    ui->label_alarmdatum->clear();
    ui->label_alarmzeit->clear();
    ui->lineEdit_anfahrt->clear();
    ui->lineEdit_anrufer->clear();
    ui->label_einsatznummer->clear();
    ui->lineEdit_geschaedigter->clear();
    ui->lineEdit_hnr->clear();
    ui->lineEdit_objekt->clear();
    ui->lineEdit_telefonnummer->clear();
    ui->lineEdit_ort->setText("Hamm");
    ui->textEdit_rueckmeldungen->clear();
    ui->textEdit_bemerkung->clear();
    ui->comboBox_stichwort->setCurrentText("");
    ui->comboBox_strasse->setCurrentText("");
    ui->checkBox_sosi->setChecked(false);

    for(int i=0;i<ui->listWidget_fahrzeuge->count();i++)
        ui->listWidget_fahrzeuge->item(i)->setSelected(false);
    for(int i=0;i<ui->listWidget->count();i++)
        ui->listWidget->item(i)->setSelected(false);

    emit resetViews();
}

void EinsatzForm::on_listWidget_itemClicked(QListWidgetItem *current) {
    Einsatz e = current->data(Qt::UserRole).value<Einsatz>();
    touchview->setEinsatz(e);
    karte->setEinsatz(e);

    for(int i=0;i<ui->listWidget_fahrzeuge->count();i++) {
        ui->listWidget_fahrzeuge->item(i)->setSelected(false);
    }

    foreach(QString name, e.fahrzeuge_file) {
        for(int i=0;i<ui->listWidget_fahrzeuge->count();i++) {
            if(name==ui->listWidget_fahrzeuge->item(i)->data(Qt::UserRole+1).toString())
                ui->listWidget_fahrzeuge->item(i)->setSelected(true);
        }
    }

    ui->lineEdit_anfahrt->setText(e.anfahrt);
    ui->lineEdit_anrufer->setText(e.anrufer);
    ui->label_einsatznummer->setText(QString::number(e.einsatznummer));
    ui->lineEdit_geschaedigter->setText(e.geschaedigter);
    ui->lineEdit_hnr->setText(e.hausnummer);
    ui->lineEdit_objekt->setText(e.objekt);
    ui->lineEdit_ort->setText(e.ort);
    ui->lineEdit_telefonnummer->setText(e.anrufer_telefonnummer);
    ui->label_annahmedatum->setText(e.datum_annahme.toString("dd.MM.yyyy"));
    ui->label_annahmezeit->setText(e.zeit_annahme.toString("hh:mm:ss"));
    ui->label_alarmdatum->setText(e.datum_alarm.toString("dd.MM.yyyy"));
    ui->label_alarmzeit->setText(e.zeit_alarm.toString("hh:mm:ss"));
    ui->comboBox_stichwort->setCurrentText(e.stichwort);
    ui->comboBox_strasse->setCurrentText(e.strasse);
    ui->textEdit_bemerkung->setText(e.bemerkung);
    ui->checkBox_sosi->setChecked(e.sonderrechte);
    ui->textEdit_rueckmeldungen->setText(e.rueckmeldungen);
}

void EinsatzForm::EinsatzDisponiert(unsigned int einsatznummer) {
    QList<Einsatz> list;
    for(int i=0;i<ui->listWidget->count();i++) {
        QListWidgetItem *item = ui->listWidget->item(i);
        unsigned int enr = item->data(Qt::UserRole).value<Einsatz>().einsatznummer;
        if(enr==einsatznummer) {
            emit takeMapScreenshot();
            Einsatz einsatz(item->data(Qt::UserRole).value<Einsatz>());
            ui->label_alarmdatum->setText(QDate::currentDate().toString("dd.MM.yyyy"));
            ui->label_alarmzeit->setText(QTime::currentTime().toString("hh:mm:ss"));
            einsatz.datum_alarm = QDate::fromString(ui->label_alarmdatum->text(), "dd.MM.yyyy");
            einsatz.zeit_alarm = QTime::fromString(ui->label_alarmzeit->text(), "hh:mm:ss");
            item->setData(Qt::UserRole, qVariantFromValue(einsatz));
        }
        list.append(ui->listWidget->item(i)->data(Qt::UserRole).value<Einsatz>());
    }
    Einsatz::save(list);
}

void EinsatzForm::setTouchView(TouchView *t) {
    touchview = t;
    connect(touchview, SIGNAL(neuerEinsatz()), this, SLOT(neuerEinsatz()));
    connect(touchview, SIGNAL(EinsatzDisponiert(uint)), this, SLOT(EinsatzDisponiert(uint)));
    connect(this, SIGNAL(resetViews()), touchview, SLOT(resetView()));
}

void EinsatzForm::setKarte(Karte *k) {
    karte = k;
    connect(this, SIGNAL(takeMapScreenshot()), karte, SLOT(takeScreenshot()));
    connect(this, SIGNAL(resetViews()), karte, SLOT(resetMap()));
}
