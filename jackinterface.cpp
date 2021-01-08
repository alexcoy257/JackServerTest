#include "jackinterface.h"

#ifdef DBUS
#include <cstdlib>
#include <dbus/dbus.h>
#include "audio_reserve.h"
#endif
#include <cstdio>

#if defined(JACK_DBUS) && defined(__linux__)
#include <cstdlib>
#include <dbus/dbus.h>
#include "audio_reserve.h"
#endif


/*
Q_DECL_IMPORT bool audio_acquire(const char * device_name);
Q_DECL_IMPORT void audio_release(const char * device_name);
Q_DECL_IMPORT void audio_reserve_loop();
*/
/*
extern "C" bool audio_acquire(const char * device_name);
extern "C" void audio_release(const char * device_name);
extern "C" void audio_reserve_loop(const char * device_name);
*/

#undef JACKCTL_H__2EEDAD78_DF4C_4B26_83B7_4FF1A446A47E__INCLUDED
JackInterface::JackInterface():
#ifdef LINUX_KLUDGE
    jackServer(jackctl_server_create(NULL, NULL))
#else
jackServer(jackctl_server_create2(audio_acquire, audio_release, audio_reserve_loop))
 //jackServer(jackctl_server_create2(NULL, NULL, NULL))
#endif
{
    const char * dev = "Audio0";
    audio_acquire(dev);
    audio_release(dev);
    if(jackServer){
        qDebug() << "Jack server made successfully.";
        const JSList * drivers = jackctl_server_get_drivers_list(jackServer.data());
        updateParamStructure(jackDriverParams, jackctl_server_get_parameters(jackServer.data()));
        while (drivers){
            QString someDriver = QString::fromStdString(jackctl_driver_get_name((jackctl_driver_t *)drivers->data));
            //qDebug() <<someDriver;
            jackDrivers.insert(someDriver, (jackctl_driver_t *)drivers->data);
            drivers = drivers->next;
        }

    }
}

QStringList * JackInterface::getDrivers(){
    QStringList * toRet = new QStringList;
    if (toRet){

    for (QString s : jackDrivers.keys()){
        toRet->append(s);
    }}
 return toRet;
}

void JackInterface::setDriver(const QString & driver){
    m_driver = jackDrivers[driver];
    if (m_driver){
        updateParamStructure(jackDriverParams, jackctl_driver_get_parameters(m_driver));
    }
}


QStringList * JackInterface::getParams(paramStruct_t & paramStruct ){
    QStringList * toRet = new QStringList;
    if (toRet){

    for (QString s : paramStruct.keys()){
        toRet->append(s);
    }}
 return toRet;
}

void JackInterface::setServerParameter(QString & param, QVariant & value){
    jackctl_parameter * sparam = jackServerParams[param];
    setParameter(sparam, param, value);
}

void JackInterface::setDriverParameter(QString & param, QVariant & value){
    jackctl_parameter * sparam = jackDriverParams[param];
    setParameter(sparam, param, value);
}

void JackInterface::setParameter(jackctl_parameter_t * jparam, QString & param, QVariant & value){
    if(!jparam){
        qDebug() <<"Null parameter " <<param <<"sent";
        return; //Failed
    }
    jackctl_param_type_t jparamtype = jackctl_parameter_get_type(jparam);
    jackctl_parameter_value jparamvalue = jackctl_parameter_get_value(jparam);
    bool okay;
    switch (jparamtype){
    case JackParamInt:
        okay = value.canConvert(QVariant::Int);
        jparamvalue.i = value.toInt();
        break;
    case JackParamBool:
        okay = value.canConvert(QVariant::Bool);
        jparamvalue.b = value.toBool();
        break;
    case JackParamString:{
        QString tqs = value.toString();
        if (tqs.isNull()){
            okay = false;
            break;
        }
        okay=true;
        std::string ts = tqs.toStdString();
        size_t strl = qMin((int)ts.length(), JACK_PARAM_STRING_MAX-1);
        memcpy(jparamvalue.str, ts.data(), strl);
        jparamvalue.str[strl] = 0;
        break;
    }
    case JackParamUInt:
        jparamvalue.ui = value.toUInt(&okay);
        break;
    case JackParamChar:
        jparamvalue.i = value.toInt(&okay);
        break;
    }
    if(okay){
        if(jackctl_parameter_set_value(jparam, &jparamvalue)){
        qDebug() <<"Set " <<param <<" to " <<value;
        }else{
            qDebug() <<"Failed to set " <<param <<" to " <<value;
        }
    }
    return;
}

void JackInterface::updateParamStructure(paramStruct_t & paramStruct, const JSList * params){
    paramStruct = {};
    while (params){
        jackctl_parameter_t * cParam = (jackctl_parameter_t *)params->data;
        QString someParameter = QString::fromStdString(jackctl_parameter_get_name(cParam));
        qDebug() << "Looking at: " <<someParameter;
        jackctl_param_type_t cParamType = jackctl_parameter_get_type(cParam);
        if(jackctl_parameter_has_enum_constraint(cParam)){
            uint32_t numConstraints = jackctl_parameter_get_enum_constraints_count(cParam);
            QVector<JackParamValue> cParamConstraints(numConstraints);
            for (uint32_t i=0; i<numConstraints; i++){
                cParamConstraints[i].reset(cParamType,
                            jackctl_parameter_get_enum_constraint_value(cParam, i));
            }

            for (JackParamValue v: cParamConstraints){
                qDebug() <<"Constraint: " <<v.value();
            }
        }
        paramStruct.insert(someParameter, (jackctl_parameter_t *)params->data);
        params = params->next;

    }
}

int JackInterface::start(){
    qDebug() <<"Try to start Jack...";
    if(m_jackRunning){
        qDebug() <<"Jack already running!";
        emit jackStarted();
        return 1;
    }
    if(jackctl_server_open(jackServer.data(), m_driver)){
        if(jackctl_server_start(jackServer.data())){
            qDebug() <<"Started Jack!";
            m_jackRunning = true;
            emit jackStarted();
            return 0;
        }
        else
            qDebug() <<"Couldn't start Jack";
    }
    else
        qDebug () <<"Couldn't open server and driver";
    return 1;
}

int JackInterface::stop(){
    qDebug() <<"Try to stop Jack...";
    if (m_jackRunning){
    if(jackctl_server_stop(jackServer.data())){
        qDebug() <<"Server stopped successfully. ";
        if(jackctl_server_close(jackServer.data()))
        qDebug() <<"Server closed succesfully";
        m_jackRunning = false;
        emit jackStopped();
        return 0;       
    }
    else{
        qDebug() <<"Server couldn't stop. ";
        return 1;
    }
    }
    emit jackStopped();
    return 0;

}
