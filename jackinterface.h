#ifndef JACKINTERFACE_H
#define JACKINTERFACE_H

#include <QDebug>
#include <QObject>
#include <QHash>
#include <QMap>
#undef signals
#include <jack/jack.h>
#include <jack/control.h>
#define signals public

#include "lrnetjackservertest.h"

class LRNETJACKSERVERTEST_EXPORT JackInterface : public QObject
{
    Q_OBJECT
    typedef QMap<QString,jackctl_parameter_t *> paramStruct_t;
    struct JackServerDeleter
    {
        static inline void cleanup(jackctl_server_t *pointer)
        {
            jackctl_server_destroy(pointer);
            qDebug() <<"Jack server deleted okay.";
        }
    };
    class JackParamValue{
        jackctl_param_type_t m_type;
        union jackctl_parameter_value m_value;
    public:
        JackParamValue(jackctl_param_type_t type, union jackctl_parameter_value value):
        m_type(type), m_value(value){}
        void reset(jackctl_param_type_t type, union jackctl_parameter_value value){
            m_type = type;
            m_value = value;
        }
        JackParamValue(QVariant value){

            if (value.canConvert(QVariant::Int)){
                m_value.i = value.toInt();
                m_type = JackParamInt;
            }

            else if (value.canConvert(QVariant::UInt)){
                m_value.ui = value.toUInt();
                m_type = JackParamUInt;
            }

            else if (value.canConvert(QVariant::Char)){
                m_value.c = value.toChar().toLatin1();
                m_type = JackParamChar;
            }

            else if (value.canConvert(QVariant::Bool)){
                m_value.b = value.toBool();
                m_type = JackParamBool;
            }

            else if (value.canConvert(QVariant::String)){
                std::string ts = value.toString().toStdString();
                size_t strl = qMin((int)ts.length(), JACK_PARAM_STRING_MAX)-1;
                memcpy(m_value.str, ts.data(), strl);
                m_value.str[strl] = 0;
                m_type = JackParamString;
            }

        }
        JackParamValue():
        m_type((jackctl_param_type_t)-1),
        m_value({.i=0}){};
        ~JackParamValue(){};
        QVariant value(){
            switch(m_type){
            case JackParamInt:
                return QVariant(m_value.i);
                break;
            case JackParamUInt:
                return QVariant(m_value.ui);
                break;
            case JackParamString:
                return QVariant(m_value.str);
                break;
            case JackParamBool:
                return QVariant(m_value.b);
                break;
            case JackParamChar:
                return QVariant(m_value.c);
                break;
            }
            return QVariant();
        }
    };
public:
    JackInterface();
    virtual ~JackInterface();
    QStringList * getDrivers();
    void setDriver(const QString & driver);


public slots:
    void setDriverParameter(QString & name, QVariant & value);
    QStringList * getDriverParams(){
        return getParams(jackDriverParams);
    }
    QStringList * getServerParams(){
        return getParams(jackServerParams);
    }
    void setServerParameter(QString & name, QVariant & value);
    int start();
    int stop();

signals:
    void paramsAvailable();
    void serverParamsAvailable();
    void jackStarted();
    void jackStopped();

    //jackctl_driver_get_parameters(m_driver)
    //jackctl_server_get_parameters();
private:
    QScopedPointer<jackctl_server_t, JackServerDeleter> jackServer;
    jackctl_driver_t * jackDriver;
    QMap<QString,jackctl_driver_t *> jackDrivers;
    paramStruct_t jackDriverParams;
    paramStruct_t jackServerParams;
    QMap<QString,JackParamValue> jackDriverDefaultValues;
    jackctl_driver_t * m_driver;

    bool m_jackRunning = false;

    void updateParamStructure(paramStruct_t & paramStruct, const JSList * params);
    void updateDriverParams(JSList * params){
        updateParamStructure(jackDriverParams, params);
        emit paramsAvailable();}
    void updateServerParams(JSList * params){
        updateParamStructure(jackDriverParams, params);
        emit paramsAvailable();}
    QStringList * getParams(paramStruct_t & paramStruct);
    void setParameter(jackctl_parameter_t *param ,QString & name, QVariant & value);
};


#endif // JACKINTERFACE_H
