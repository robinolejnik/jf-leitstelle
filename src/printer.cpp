#include "printer.h"

Printer::Printer(QObject *parent) : QObject(parent) {
    printer = new QPrinter();
    //printer->setOutputFileName("output.pdf");
    //printer->setOutputFormat(QPrinter::PdfFormat);
    printer->setFullPage(true);
    printer->setPaperSize(QPrinter::A4);
}

void Printer::print() {
    Einsatz e;
    e = Einsatz::Beispiel();
    print(e);
}

void Printer::print(Einsatz &e) {
    QFile file("config/print.html");
    QFile cssfile("config/print.css");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    if(!cssfile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QString htmlContent;
    QString cssContent;
    QTextStream in(&file);
    QTextStream in_css(&cssfile);
    htmlContent = in.readAll();
    cssContent = in_css.readAll();

    QString fahrzeuge;
    if(e.fahrzeuge_name.size()>0)
        fahrzeuge = e.fahrzeuge_name[0];
    for(int i=1;i<e.fahrzeuge_name.size();i++)
        fahrzeuge += ", " + e.fahrzeuge_name[i];

    htmlContent.replace(QString("<!--FAHRZEUGE-->"), fahrzeuge);
    htmlContent.replace(QString("<!--ANFAHRT-->"), e.anfahrt);
    htmlContent.replace(QString("<!--ANRUFER-->"), e.anrufer);
    htmlContent.replace(QString("<!--TELNUMMER-->"), e.anrufer_telefonnummer);
    htmlContent.replace(QString("<!--BEMERKUNG-->"), e.bemerkung);
    htmlContent.replace(QString("<!--EINSATZNUMMER-->"), QString::number(e.einsatznummer));
    htmlContent.replace(QString("<!--GESCHAEDIGTER-->"), e.geschaedigter);
    htmlContent.replace(QString("<!--OBJEKT-->"), e.objekt);
    htmlContent.replace(QString("<!--ORT-->"), e.ort);
    htmlContent.replace(QString("<!--STICHWORT-->"), e.stichwort);
    htmlContent.replace(QString("<!--STRASSE-->"), e.strasse + " " + e.hausnummer);
    htmlContent.replace(QString("<!--ALARMZEIT-->"), e.zeit_alarm.toString("hh:mm:ss"));
    htmlContent.replace(QString("<!--ALARMDATUM-->"), e.datum_alarm.toString("dd.MM.yyyy"));
    htmlContent.replace(QString("<!--ANNAHMEZEIT-->"), e.zeit_annahme.toString("hh:mm:ss"));
    htmlContent.replace(QString("<!--ANNAHMEDATUM-->"), e.datum_annahme.toString("dd.MM.yyyy"));

    QTextDocument *document = new QTextDocument();
    document->setPageSize(printer->paperSize(QPrinter::Point));
    document->setDefaultStyleSheet(cssContent);
    document->setHtml(htmlContent);
    document->print(printer);
    delete document;
}

Printer::~Printer() {
    delete printer;
}
