#include "einsatz.h"

Einsatz::Einsatz(QObject *parent) : QObject(parent) {
    zeit_annahme = QTime::currentTime();
    datum_annahme = QDate::currentDate();
    ort = "Hamm";
}

unsigned int Einsatz::Einsatznummer() {
    QSettings settings;
    unsigned int enr = settings.value("einsatznummer").toUInt();
    enr++;
    settings.setValue("einsatznummer", QVariant(enr));
    if(enr>1000)
        enr = 1;
    return enr;
}

Einsatz::Einsatz(const Einsatz &e, QObject *parent) : QObject(parent) {
    datum_alarm = e.datum_alarm;
    zeit_alarm = e.zeit_alarm;
    datum_annahme = e.datum_annahme;
    zeit_annahme = e.zeit_annahme;
    einsatznummer = e.einsatznummer;
    bemerkung = e.bemerkung;
    fahrzeuge_file = e.fahrzeuge_file;
    fahrzeuge_name = e.fahrzeuge_name;
    strasse = e.strasse;
    stichwort = e.stichwort;
    rueckmeldungen = e.rueckmeldungen;
    anrufer = e.anrufer;
    anrufer_telefonnummer = e.anrufer_telefonnummer;
    geschaedigter = e.geschaedigter;
    anfahrt = e.anfahrt;
    hausnummer = e.hausnummer;
    sonderrechte = e.sonderrechte;
    objekt = e.objekt;
    ort = e.ort;
}

QString Einsatz::einsatztext() {
    return strasse + " " + hausnummer + "/" + objekt + "/" + stichwort + "/" + geschaedigter + "/" + anrufer;
}

void Einsatz::operator=(const Einsatz &e) {
    datum_alarm = e.datum_alarm;
    zeit_alarm = e.zeit_alarm;
    datum_annahme = e.datum_annahme;
    zeit_annahme = e.zeit_annahme;
    einsatznummer = e.einsatznummer;
    bemerkung = e.bemerkung;
    ort = e.ort;
    strasse = e.strasse;
    stichwort = e.stichwort;
    sonderrechte = e.sonderrechte;
    fahrzeuge_file = e.fahrzeuge_file;
    fahrzeuge_name = e.fahrzeuge_name;
    hausnummer = e.hausnummer;
    anrufer = e.anrufer;
    rueckmeldungen = e.rueckmeldungen;
    anrufer_telefonnummer = e.anrufer_telefonnummer;
    geschaedigter = e.geschaedigter;
    anfahrt = e.anfahrt;
    objekt = e.objekt;
}

Einsatz Einsatz::Beispiel() {
    Einsatz e;
    e.strasse = "Im Ried";
    e.stichwort = "BMA Automatische Feuermeldung";
    e.ort = "Hamm";
    e.anrufer = "BMA";
    e.geschaedigter = "BMA";
    e.anrufer_telefonnummer = "023811234567";
    e.anfahrt = "";
    e.hausnummer = "";
    e.sonderrechte = true;
    e.objekt = "Ehem. Asylbewerberunterkunft";
    e.rueckmeldungen = "";
    e.bemerkung = "";
    e.zeit_annahme = QTime::currentTime();
    e.datum_annahme = QDate::currentDate();
    return e;
}

Einsatz Einsatz::BMA() {
    Einsatz e;
    e.strasse = "Im Ried";
    e.stichwort = "BMA Automatische Feuermeldung";
    e.ort = "Hamm";
    e.anrufer = "BMA";
    e.geschaedigter = "BMA";
    e.anrufer_telefonnummer = "023811234567";
    e.anfahrt = "";
    e.hausnummer = "";
    e.sonderrechte = true;
    e.objekt = "Ehem. Asylbewerberunterkunft";
    e.rueckmeldungen = "";
    e.bemerkung = "";
    e.zeit_annahme = QTime::currentTime();
    e.datum_annahme = QDate::currentDate();
    return e;
}

QList<Einsatz> Einsatz::load() {
    QSettings settings;
    QList<Einsatz> list;
    int size = settings.beginReadArray("einsaetze");
    for(int i=0;i<size;i++) {
        settings.setArrayIndex(i);
        Einsatz e;
        e.datum_annahme = QDate::fromString(settings.value("datum_annahme").toString(), "dd.MM.yyyy");
        e.zeit_annahme = QTime::fromString(settings.value("zeit_annahme").toString(), "hh:mm:ss");
        e.datum_alarm = QDate::fromString(settings.value("datum_alarm").toString(), "dd.MM.yyyy");
        e.zeit_alarm = QTime::fromString(settings.value("zeit_alarm").toString(), "hh:mm:ss");
        e.anfahrt = settings.value("anfahrt").toString();
        e.anrufer = settings.value("anrufer").toString();
        e.anrufer_telefonnummer = settings.value("anrufer_telefonnummer").toString();
        e.bemerkung = settings.value("bemerkung").toString();
        e.einsatznummer = settings.value("einsatznummer").toUInt();
        e.geschaedigter = settings.value("geschaedigter").toString();
        e.hausnummer = settings.value("hnr").toString();
        e.objekt = settings.value("objekt").toString();
        e.ort = settings.value("ort").toString();
        e.stichwort = settings.value("stichwort").toString();
        e.fahrzeuge_file = settings.value("fahrzeuge_file").toStringList();
        e.fahrzeuge_name = settings.value("fahrzeuge_name").toStringList();
        e.strasse = settings.value("strasse").toString();
        e.sonderrechte = settings.value("sonderrechte").toBool();
        e.rueckmeldungen = settings.value("rueckmeldungen").toString();
        list.append(e);
    }
    settings.endArray();
    return list;
}

void Einsatz::save(QList<Einsatz> &list) {
    QSettings settings;
    settings.remove("einsaetze");
    settings.beginWriteArray("einsaetze");
    for(int i=0;i<list.size();i++) {
        settings.setArrayIndex(i);
        settings.setValue("datum_annahme", QVariant(list[i].datum_annahme.toString("dd.MM.yyyy")));
        settings.setValue("zeit_annahme", QVariant(list[i].zeit_annahme.toString("hh:mm:ss")));
        settings.setValue("datum_alarm", QVariant(list[i].datum_alarm.toString("dd.MM.yyyy")));
        settings.setValue("zeit_alarm", QVariant(list[i].zeit_alarm.toString("hh:mm:ss")));
        settings.setValue("anfahrt", QVariant(list[i].anfahrt));
        settings.setValue("anrufer", QVariant(list[i].anrufer));
        settings.setValue("anrufer_telefonnummer", QVariant(list[i].anrufer_telefonnummer));
        settings.setValue("bemerkung", QVariant(list[i].bemerkung));
        settings.setValue("einsatznummer", QVariant(list[i].einsatznummer));
        settings.setValue("hnr", QVariant(list[i].hausnummer));
        settings.setValue("geschaedigter", QVariant(list[i].geschaedigter));
        settings.setValue("objekt", QVariant(list[i].objekt));
        settings.setValue("sonderrechte", QVariant(list[i].sonderrechte));
        settings.setValue("ort", QVariant(list[i].ort));
        settings.setValue("stichwort", QVariant(list[i].stichwort));
        settings.setValue("strasse", QVariant(list[i].strasse));
        settings.setValue("rueckmeldungen", QVariant(list[i].rueckmeldungen));
        settings.setValue("fahrzeuge_name", QVariant(list[i].fahrzeuge_name));
        settings.setValue("fahrzeuge_file", QVariant(list[i].fahrzeuge_file));
    }
    settings.endArray();
}
