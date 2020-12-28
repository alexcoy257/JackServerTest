#ifndef JACKPARAMETERFORM_H
#define JACKPARAMETERFORM_H

#include <QWidget>
#include <QComboBox>

#include "qjackctlinterfacecombobox.h"
#include "ui_jackparameterform.h"

namespace Ui {
class JackParameterForm;
}

class JackParameterForm : public QWidget
{
    Q_OBJECT

     Ui::JackParameterForm *ui;
public:
    explicit JackParameterForm(QWidget *parent = nullptr);
    ~JackParameterForm();

     QComboBox * driverBox;
     QComboBox * m_fppChoice;
     QComboBox * m_srChoice;
     qjackctlInterfaceComboBox * iFaceBox;
     qjackctlInterfaceComboBox * oFaceBox;
     void sendAllParameters();

signals:
     void setParameter(QString & name, QVariant & value);

private:
     void sendNewRate(const QString & rate);
     void sendNewPeriod(const QString & period);


};

#endif // JACKPARAMETERFORM_H
