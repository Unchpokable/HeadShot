#include "Loader.h"

#include <QScreen>
#include <QProcess>

#include "ColorUtils.h"

Loader::Loader(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LoaderClass())
{
    ui->setupUi(this);

    connect(ui->widthSlider, &QSlider::valueChanged, this, [this]() {
        ui->widLabel->setText(QString::number(ui->widthSlider->value()));
    });

    connect(ui->heightSlider, &QSlider::valueChanged, this, [this]() {
        ui->hiLabel->setText(QString::number(ui->heightSlider->value()));
    });

    ui->widthSlider->setValue(5);
    ui->heightSlider->setValue(5);

    connect(ui->targetRedSlider, &QSlider::valueChanged, this, &Loader::TargetColorSettingsChanged);
    connect(ui->targetGreenSlider, &QSlider::valueChanged, this, &Loader::TargetColorSettingsChanged);
    connect(ui->targetBlueSlider, &QSlider::valueChanged, this, &Loader::TargetColorSettingsChanged);

    ui->targetRedSlider->setValue(255);
    ui->targetGreenSlider->setValue(255);
    ui->targetBlueSlider->setValue(255);

    connect(ui->hexColorInput, &QLineEdit::textChanged, this, &Loader::TargetColorSettingsHexInputChanged);

    const QScreen *screen = QGuiApplication::primaryScreen();
    _windowGeometry = screen->geometry();

    ui->screenResolutionLabel->setText(QString("%1x%2").arg(_windowGeometry.width()).arg(_windowGeometry.height()));

    connect(ui->startButton, &QPushButton::clicked, this, &Loader::StartTriggerProcess);

    connect(ui->extendValueSlider, &QSlider::valueChanged, this, [this]() {
        ui->extendValueLabel->setText(QString::number(ui->extendValueSlider->value()));
    });
}

Loader::~Loader()
{
    delete ui;
}

void Loader::StartTriggerProcess()
{
    if (_triggerBotProcess)
    {
        _triggerBotProcess->terminate();
        delete _triggerBotProcess;
    }

    _triggerBotProcess = new QProcess();

    QStringList args;

    QColor lColor;
    QColor rColor;

    if (ui->extendValueCheckbox->isChecked())
    {
        auto [first, second] = ColorUtils::GenerateColorRange(_selectionColor, ui->extendValueSlider->value());
        lColor = first;
        rColor = second;
    }
    else
    {
        lColor = _selectionColor;
        rColor = _selectionColor;
    }

    // -console 1 -colorl #00ff00 -colorr #ff00ff -wpx 1270 -wpy 710 -wpw 10 -wph 10
    args << "-console" << "1" << "colorl" << lColor.name() << "colorr" << rColor.name()
        << "-wpx" << QString::number(_windowGeometry.width() / 2 - ui->widthSlider->value() / 2)
        << "-wpy" << QString::number(_windowGeometry.height() / 2 - ui->heightSlider->value() / 2)
        << "-wpw" << QString::number(ui->widthSlider->value())
        << "-wph" << QString::number(ui->heightSlider->value());

    _triggerBotProcess->start(_triggerBotExeName, args);
}

void Loader::TargetColorSettingsHexInputChanged()
{
    const QColor color(ui->hexColorInput->text());

    if (color.isValid())
    {
        _selectionColor.setRed(color.red());
        _selectionColor.setGreen(color.green());
        _selectionColor.setBlue(color.blue());

        ui->targetRedSlider->blockSignals(true);
        ui->targetGreenSlider->blockSignals(true);
        ui->targetBlueSlider->blockSignals(true);

        ui->targetRedSlider->setValue(_selectionColor.red());
        ui->targetGreenSlider->setValue(_selectionColor.green());
        ui->targetBlueSlider->setValue(_selectionColor.blue());

        ui->colorSelectionDemo->setStyleSheet(
            QString("background-color: rgb(%1, %2, %3)").arg(_selectionColor.red()).arg(_selectionColor.green()).arg(_selectionColor.blue()));

        ui->targetRedSlider->blockSignals(false);
        ui->targetGreenSlider->blockSignals(false);
        ui->targetBlueSlider->blockSignals(false);
    }
}

void Loader::TargetColorSettingsChanged() {

    auto r = (byte)ui->targetRedSlider->value();
    auto g = (byte)ui->targetGreenSlider->value();
    auto b = (byte)ui->targetBlueSlider->value();

    _selectionColor.setRed(r);
    _selectionColor.setGreen(g);
    _selectionColor.setBlue(b);

    ui->colorSelectionDemo->setStyleSheet(
        QString("background-color: rgb(%1, %2, %3)").arg(r).arg(g).arg(b));

    ui->hexColorInput->setText(QColor(r, g, b).name(QColor::HexRgb));
}
