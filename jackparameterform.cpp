#include "jackparameterform.h"
#include "ui_jackparameterform.h"

JackParameterForm::JackParameterForm(QWidget *parent) :
    QWidget(parent),
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

    QObject::connect(ui->m_fppChoice, &QComboBox::currentTextChanged, this,
       &JackParameterForm::sendNewPeriod
    );

    QObject::connect(ui->m_srChoice, &QComboBox::currentTextChanged, this,
       &JackParameterForm::sendNewRate
    );
}

void JackParameterForm::sendAllParameters(){
    sendNewRate(ui->m_srChoice->currentText());
    sendNewPeriod(ui->m_fppChoice->currentText());
}

void JackParameterForm::sendNewRate(const QString & value){
        QString tmp = "rate";
        QVariant tmpv = QVariant(value.toInt());
        emit setParameter(tmp, tmpv);
    }
void JackParameterForm::sendNewPeriod(const QString & value){
        QString tmp = "period";
        QVariant tmpv = QVariant(value.toInt());
        emit setParameter(tmp, tmpv);
    }

JackParameterForm::~JackParameterForm()
{
    delete ui;
}
