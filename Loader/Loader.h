#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Loader.h"

class QProcess;

QT_BEGIN_NAMESPACE
namespace Ui { class LoaderClass; };
QT_END_NAMESPACE

using byte = unsigned char;

struct Color
{
    byte R, G, B;

    QString ToHex() const noexcept
    {
        return QColor(R, G, B).name(QColor::HexRgb);
    }
};

class Loader : public QMainWindow
{
    Q_OBJECT

public:
    Loader(QWidget *parent = nullptr);
    ~Loader();

private:
    Ui::LoaderClass *ui;

    QRect _windowGeometry;
    QColor _selectionColor;

    QProcess* _triggerBotProcess { nullptr };

    inline static QString _triggerBotExeName = QString("HeadShot!.exe");

private slots:
    void TargetColorSettingsChanged();
    void TargetColorSettingsHexInputChanged();
    void StartTriggerProcess();
};
