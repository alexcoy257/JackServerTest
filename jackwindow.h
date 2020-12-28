#ifndef JACKWINDOW_H
#define JACKWINDOW_H

#include <QMainWindow>
#include <QDebug>

#include "jackinterface.h"

QT_BEGIN_NAMESPACE
namespace Ui { class JackWindow; }
QT_END_NAMESPACE

class JackWindow : public QMainWindow
{
    Q_OBJECT

public:
    JackWindow(QWidget *parent = nullptr);
    ~JackWindow();

private:
    Ui::JackWindow *ui;
    JackInterface jackServer;

};
#endif // JACKWINDOW_H
