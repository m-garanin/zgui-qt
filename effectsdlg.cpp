#include "effectsdlg.h"
#include "ui_effectsdlg.h"

#include <QDir>
#include <QStringList>
#include <QDebug>


CEffectsDlg::CEffectsDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CEffectsDlg),
    selected_effect(QString()),
    column_count(5)
{
    ui->setupUi(this);

    connect(ui->pbOk, SIGNAL(clicked()), SLOT(onPbOkClicked()));
    connect(ui->pbCancel, SIGNAL(clicked()), SLOT(onPbCancelClicked()));

    loadEffectsImg();
}

CEffectsDlg::~CEffectsDlg()
{
    delete ui;
}

void CEffectsDlg::setColuntCount(quint32 count)
{
    column_count = count;
    ui->tableWidget->clear();
    loadEffectsImg();
}

void CEffectsDlg::loadEffectsImg()
{
    QString defaultPthToEffects("effects/");

    QDir dir(defaultPthToEffects);
    QStringList files = dir.entryList(QDir::Files);

    ui->tableWidget->setFocusPolicy(Qt::NoFocus);
    ui->tableWidget->setRowCount(files.count()/column_count + files.count()%column_count);
    ui->tableWidget->setColumnCount(column_count);

    QString btn_name, cust_style;

    for(int i = 0; i < files.count(); ++i)
    {
        int row = i/column_count;
        int col = i%column_count;
        btn_name = QString("btEffect%1").arg(i);
        cust_style = QString("#%1 { border-image: url(%2/%3); background-color: transparent; max-width: 150px; max-height: 150px; margin-top: 7px; margin-left: 4px; margin-right: 4px;}").arg(btn_name).arg(dir.absolutePath()).arg(files.at(i));

        ui->tableWidget->setIndexWidget(ui->tableWidget->model()->index(row, col), new QPushButton());
        ui->tableWidget->indexWidget(ui->tableWidget->model()->index(row, col))->setObjectName(btn_name);
        ui->tableWidget->indexWidget(ui->tableWidget->model()->index(row, col))->setStyleSheet(cust_style);
        ui->tableWidget->indexWidget(ui->tableWidget->model()->index(row, col))->setProperty("file_name", files.at(i));

        connect(ui->tableWidget->indexWidget(ui->tableWidget->model()->index(row, col)), SIGNAL(clicked()), SLOT(onPbEffectClicked()));
    }
}

void CEffectsDlg::onPbEffectClicked()
{
    selected_effect = sender()->property("file_name").toString();
}

QString CEffectsDlg::selectedEffectName() const
{
    return selected_effect;
}

void CEffectsDlg::onPbOkClicked()
{
    accept();
}

void CEffectsDlg::onPbCancelClicked()
{
    selected_effect.clear();
    reject();
}
