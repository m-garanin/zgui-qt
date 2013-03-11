#include "airdialog.h"
#include "ui_airdialog.h"

AirDialog::AirDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AirDialog)
{
    ui->setupUi(this);
}

AirDialog::~AirDialog()
{
    delete ui;
}
