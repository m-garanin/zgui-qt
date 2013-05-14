#include "bigairstat.h"
#include "ui_bigairstat.h"

Bigairstat::Bigairstat(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Bigairstat)
{
    ui->setupUi(this);    
}

Bigairstat::~Bigairstat()
{
    delete ui;
}

void Bigairstat::updateStat(int fps, int btr)
{
    ui->lcdFPS->display(fps);
    ui->lcdBTR->display(btr);

    if(fps < 24){
        ui->lcdFPS->setStyleSheet("color:red");
    }else{
        ui->lcdFPS->setStyleSheet("");
    }

}
