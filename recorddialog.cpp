#include "recorddialog.h"
#include "ui_recorddialog.h"

RecordDialog::RecordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RecordDialog)
{
    ui->setupUi(this);
    ui->comboBox->addItem("100");
    ui->comboBox->addItem("200");
    ui->comboBox->addItem("300");
}

RecordDialog::~RecordDialog()
{
    delete ui;
}
