#ifndef printer_H
#define printer_H

#include <QObject>
#include <QtPrintSupport/QPrinter>
#include <QFile>
#include <QTextStream>
#include <QTextDocument>
#include "einsatz.h"

class Printer : public QObject {
    Q_OBJECT
public:
    explicit Printer(QObject *parent = 0);
    void print();
    void print(Einsatz &e);
    ~Printer();

signals:

private:
    QPrinter *printer;

public slots:
};

#endif // Printer_H
