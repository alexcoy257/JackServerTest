#include "jackinterface.h"

JackInterface::JackInterface():
#ifdef JACKCTL_H__2EEDAD78_DF4C_4B26_83B7_4FF1A446A47E__INCLUDED
    jackServer(jackctl_server_create(NULL, NULL))
#else
 jackServer(jackctl_server_create2(NULL, NULL, NULL))
#endif
{
    if(jackServer){
        qDebug() << "Jack server made successfully.";
        const JSList * drivers = jackctl_server_get_drivers_list(jackServer.data());

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
        updateParamStructure();
    }
}

QStringList * JackInterface::getDriverParams(){
    QStringList * toRet = new QStringList;
    if (toRet){

    for (QString s : jackDriverParams.keys()){
        toRet->append(s);
    }}
 return toRet;
}

void JackInterface::setParameter(QString & param, QVariant & value){
    jackctl_parameter * jparam = jackDriverParams[param];
    if(!jparam){
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
        size_t strl = qMin((int)ts.length(), JACK_PARAM_STRING_MAX)-1;
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
        jackctl_parameter_set_value(jparam, &jparamvalue);
        qDebug() <<"Set " <<param <<" to " <<value;
    }
    return;
}

void JackInterface::updateParamStructure(){
    jackDriverParams = {};
    const JSList * params = jackctl_driver_get_parameters(m_driver);
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
        jackDriverParams.insert(someParameter, (jackctl_parameter_t *)params->data);
        params = params->next;

    }
    emit paramsAvailable();
}

void JackInterface::start(){
    qDebug() <<"Try to start Jack...";
    if(jackctl_server_open(jackServer.data(), m_driver)){
        if(jackctl_server_start(jackServer.data()))
            qDebug() <<"Started Jack!";
        else
            qDebug() <<"Couldn't start Jack";
    }
    else
        qDebug () <<"Couldn't open server and driver";
}

void JackInterface::stop(){
    qDebug() <<"Try to stop Jack...";
    if(jackctl_server_stop(jackServer.data()))
            qDebug() <<"Server stopped successfully. ";
    else
        qDebug() <<"Server couldn't stop. ";
}
