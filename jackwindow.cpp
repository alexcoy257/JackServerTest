#include "jackwindow.h"
#include "ui_jackwindow.h"
#include "portaudio.h"
#include "jackparameterform.h"
#include <QDebug>




JackWindow::JackWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::JackWindow)

{
    ui->setupUi(this);
    ui->jpf->iFaceBox->setup(ui->jpf->driverBox, 1, "(default)" );
    ui->jpf->iFaceBox->setCurrentText("(default)");
    ui->jpf->oFaceBox->setup(ui->jpf->driverBox, 2, "(default)" );
    ui->jpf->oFaceBox->setCurrentText("(default)");
    QScopedPointer<QStringList> drivers(jackServer.getDrivers());
    for (QString s : *drivers){
        ui->jpf->driverBox->addItem(s);
    }

    QObject::connect(ui->jpf->driverBox, &QComboBox::currentTextChanged, &jackServer, &JackInterface::setDriver);
    QObject::connect(&jackServer, &JackInterface::paramsAvailable, this, [=](){
        QScopedPointer<QStringList> params(jackServer.getDriverParams());
     for (QString p : *params)
         qDebug() << p;
    });

    QObject::connect(ui->jpf, &JackParameterForm::setParameter, &jackServer, &JackInterface::setParameter);

    QObject::connect(ui->startJackButton, &QAbstractButton::released, &jackServer, &JackInterface::start);
    QObject::connect(ui->stopJackButton, &QAbstractButton::released, &jackServer, &JackInterface::stop);

    QObject::connect(ui->jpf->iFaceBox, &qjackctlInterfaceComboBox::setSubdevice, this, [=](const QString & dev){
        QString tmp = "capture";
        QVariant tmpv = QVariant(dev);
        jackServer.setParameter(tmp, tmpv);
    });

    QObject::connect(ui->jpf->oFaceBox, &qjackctlInterfaceComboBox::setSubdevice, this, [=](const QString & dev){
        QString tmp = "playback";
        QVariant tmpv = QVariant(dev);
        jackServer.setParameter(tmp, tmpv);
    });

    ui->jpf->sendAllParameters();
}



JackWindow::~JackWindow()
{
    delete ui;
}

