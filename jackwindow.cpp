#include "jackwindow.h"
#include "ui_jackwindow.h"
#include "jackparameterform.h"
#include <QDebug>




JackWindow::JackWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::JackWindow)

{
    ui->setupUi(this);
}



JackWindow::~JackWindow()
{
    delete ui;
}

