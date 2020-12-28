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

class JackInterface : public QObject
{
    Q_OBJECT
    struct JackServerDeleter
    {
        static inline void cleanup(jackctl_server_t *pointer)
        {
            jackctl_server_destroy(pointer);
            qDebug() <<"Jack server deleted okay.";
        }
    };
public:
    JackInterface();
    QStringList * getDrivers();
    void setDriver(const QString & driver);
    QStringList * getDriverParams();

public slots:
    void setParameter(QString & name, QVariant & value);
    void start();
    void stop();

signals:
    void paramsAvailable();

private:
    QScopedPointer<jackctl_server_t, JackServerDeleter> jackServer;
    jackctl_driver_t * jackDriver;
    QMap<QString,jackctl_driver_t *> jackDrivers;
    QMap<QString,jackctl_parameter_t *> jackDriverParams;
    jackctl_driver_t * m_driver;
};


#endif // JACKINTERFACE_H
