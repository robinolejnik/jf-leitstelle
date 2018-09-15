#ifndef EINSATZ_H
#define EINSATZ_H

#include <QObject>
#include <QTime>
#include <QDate>
#include <QSettings>
#include <QList>

class Einsatz : public QObject {
    Q_OBJECT
public:
    explicit Einsatz(QObject *parent = nullptr);
    Einsatz(const Einsatz &e, QObject *parent = nullptr);
    void operator=(const Einsatz &e);
    static QList<Einsatz> load();
    static void save(QList<Einsatz> &list);
    static Einsatz Beispiel();
    static Einsatz BMA();
    static unsigned int Einsatznummer();
    QString einsatztext();
    QString anrufer;
    QString anrufer_telefonnummer;
    QString geschaedigter;
    QString strasse;
    QString ort;
    QString anfahrt;
    QString objekt;
    QString stichwort;
    QString bemerkung;
    QString hausnummer;
    QStringList fahrzeuge_name;
    QStringList fahrzeuge_file;
    bool sonderrechte;
    QString rueckmeldungen;
    QTime zeit_annahme;
    QDate datum_annahme;
    QTime zeit_alarm;
    QDate datum_alarm;
    unsigned int einsatznummer;
};

Q_DECLARE_METATYPE(Einsatz);

#endif // EINSATZ_H
