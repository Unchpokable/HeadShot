#include "Loader.h"
#include <QtWidgets/QApplication>
#include <QFile>
#include <QMainWindow>
#include <QTextStream>

void ApplyStyleSheet(QApplication& app, const QString& fileName) {
    QFile file(fileName);
    if(file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&file);
        QString styleSheet = stream.readAll();
        app.setStyleSheet(styleSheet);
        file.close();
    } else {
        qWarning("Unable to open file: %s", qPrintable(fileName));
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Loader w;

    w.setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    ApplyStyleSheet(a, "window.qss");

    w.show();
    return a.exec();
}
