#include "netsourcedlg.h"
#include "ui_netsourcedlg.h"

NetSourceDlg::NetSourceDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NetSourceDlg)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Add network source"));
}

NetSourceDlg::~NetSourceDlg()
{
    delete ui;
}

QString NetSourceDlg::getURI()
{
    return ui->txtURI->text();
}
