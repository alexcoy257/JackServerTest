#include "jackparameterform.h"
#include "ui_jackparameterform.h"

JackParameterForm::JackParameterForm(QWidget *parent) :
    QWidget(parent),
    jackServer(new JackInterface()),
    ui(new Ui::JackParameterForm)

{
    ui->setupUi(this);
    driverBox = ui->driverBox;
    m_fppChoice = ui->m_fppChoice;
    m_srChoice = ui->m_srChoice;
    iFaceBox = ui->iFaceBox;
    oFaceBox = ui->oFaceBox;
    iFaceLabel = ui->iFaceLabel;
    oFaceLabel = ui->oFaceLabel;

    ui->iFaceBox->setup(ui->driverBox, 1, "(default)" );
    //ui->iFaceBox->setCurrentText("(default)");
    ui->oFaceBox->setup(ui->driverBox, 2, "(default)" );
    //ui->oFaceBox->setCurrentText("(default)");

#ifdef __MAC_OSX__
    //These are useless with the current state of JackRouter. We just want the plain inputs!
    ui->iFaceBox->hide();
    ui->iFaceLabel->hide();
    ui->oFaceBox->hide();
    ui->oFaceLabel->hide();
#endif

    QObject::connect(ui->m_fppChoice, &QComboBox::currentTextChanged, this,
       &JackParameterForm::sendNewPeriod
    );

    QObject::connect(ui->m_srChoice, &QComboBox::currentTextChanged, this,
       &JackParameterForm::sendNewRate
    );



    QScopedPointer<QStringList> drivers(jackServer->getDrivers());
    for (QString s : *drivers){
        ui->driverBox->addItem(s);
    }

    QObject::connect(ui->driverBox, &QComboBox::currentTextChanged, this, &JackParameterForm::sendAllParameters);


    QObject::connect(jackServer, &JackInterface::paramsAvailable, this, [=](){
        QScopedPointer<QStringList> params(jackServer->getDriverParams());
     for (QString p : *params)
         qDebug() << p;
    });

    QObject::connect(ui->m_startJackButton, &QAbstractButton::released, jackServer, &JackInterface::start);
    QObject::connect(ui->m_stopJackButton, &QAbstractButton::released, jackServer, &JackInterface::stop);
    QObject::connect(jackServer, &JackInterface::jackStarted, this, [=](){emit jackStarted();
                                                                          ui->m_startJackButton->setDisabled(true);
                                                                          ui->m_startJackButton->clearFocus();});
    QObject::connect(jackServer, &JackInterface::jackStopped, this, [=](){emit jackStopped();
                                                                          ui->m_startJackButton->setEnabled(true);});

#ifndef __MAC_OSX__
    QObject::connect(ui->iFaceBox, &qjackctlInterfaceComboBox::currentTextChanged, this, [=](const QString & dev){
        QString tmp = "capture";
        QVariant tmpv = QVariant(dev);
        jackServer->setDriverParameter(tmp, tmpv);
    });

    QObject::connect(ui->oFaceBox, &qjackctlInterfaceComboBox::currentTextChanged, this, [=](const QString & dev){
        QString tmp = "playback";
        QVariant tmpv = QVariant(dev);
        jackServer->setDriverParameter(tmp, tmpv);
    });
#endif

    sendAllParameters();
    /*
    {QString arg = "verbose";
     QVariant v(true);
    jackServer->setServerParameter(arg, v);
    }*/
}

void JackParameterForm::sendAllParameters(){
    jackServer->setDriver(ui->driverBox->currentText());
    #ifdef WIN32
        
        printf("Try to set sync \n");    
        QString ts = "sync";
        QVariant tb = true;
        jackServer->setServerParameter(ts, tb);
        
        #endif
    sendNewRate(ui->m_srChoice->currentText());
    sendNewPeriod(ui->m_fppChoice->currentText());

    
}

void JackParameterForm::sendNewRate(const QString & value){
        QString tmp = "rate";
        QVariant tmpv = QVariant(value.toInt());
        jackServer->setDriverParameter(tmp, tmpv);
    }
void JackParameterForm::sendNewPeriod(const QString & value){
        QString tmp = "period";
        QVariant tmpv = QVariant(value.toInt());
        jackServer->setDriverParameter(tmp, tmpv);
    }

void JackParameterForm::loadSetup(QSettings &settings){
    settings.beginGroup("/Jack");
    driverBox->setCurrentText(settings.value("Driver","").toString());
    m_fppChoice->setCurrentText(settings.value("FPP","").toString());
    m_srChoice->setCurrentText(settings.value("SR","").toString());
    iFaceBox->setCurrentText(settings.value("iFace","").toString());
    oFaceBox->setCurrentText(settings.value("oFace","").toString());
    settings.endGroup();

    sendAllParameters();
}

void JackParameterForm::saveSetup(QSettings &settings){
    settings.beginGroup("/Jack");
    settings.setValue("Driver", driverBox->currentText());
    settings.setValue("FPP", m_fppChoice->currentText());
    settings.setValue("SR", m_srChoice->currentText());
    settings.setValue("iFace", iFaceBox->currentText());
    settings.setValue("oFace", oFaceBox->currentText());
    settings.endGroup();

    settings.sync();
}


JackParameterForm::~JackParameterForm()
{
    if (jackServer)
        delete jackServer;
    delete ui;
}
