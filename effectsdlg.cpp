#include "effectsdlg.h"
#include "ui_effectsdlg.h"

#include <QDir>
#include <QStringList>
#include <QPushButton>
#include <QDebug>


CEffectsDlg::CEffectsDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CEffectsDlg)
{
    ui->setupUi(this);
    setWindowTitle(tr("Select effect"));
    ui->cleanButton->setText(tr("REMOVE EFFECT(ORIGINAL VIEW)"));
    fill();

 }

CEffectsDlg::~CEffectsDlg()
{
    delete ui;
}


void CEffectsDlg::onSelect()
{
    QPushButton* btn = (QPushButton*)sender();
    m_effect = btn->text();
    this->done(QDialog::Accepted);
}


void CEffectsDlg::fill()
{
    QStringList list;
    list << "agingtv"
         << "edgetv"
         << "shagadelictv"
         << "quarktv"
         << "radioactv";

    // TODO:  "rippletv"

    for (int i = 0; i < list.size(); i++){
        QPushButton* btn = new QPushButton(list[i]);
        btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        ui->verticalLayout->addWidget(btn);

        connect(btn, SIGNAL(clicked()), SLOT(onSelect()));
    }


}

void CEffectsDlg::on_cleanButton_clicked()
{
    m_effect = "CLEAN";
    this->done(QDialog::Accepted);
}
