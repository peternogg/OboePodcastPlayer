#include "OboeWindow.h"
#include "ui_OboeWindow.h"

OboeWindow::OboeWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OboeWindow)
{
    ui->setupUi(this);
}

OboeWindow::~OboeWindow()
{
    delete ui;
}
