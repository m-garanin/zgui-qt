#include "htmlsettings.h"
#include "ui_htmlsettings.h"
#include <QWebElement>
#include <QDebug>

HTMLSettings::HTMLSettings(QWidget *parent) :
    QDialog(parent, Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint),
    ui(new Ui::HTMLSettings)
{
    ui->setupUi(this);
    m_view = new QWebView(this);

    ui->verticalLayout->addWidget(m_view);

    m_lbl = new QLabel();
    ui->verticalLayout->addWidget(m_lbl);

}


HTMLSettings::~HTMLSettings()
{
    delete ui;
}

void HTMLSettings::openURL(QUrl url)
{
    m_view->load(url);
}


void HTMLSettings::showFrame(QImage img)
{
    m_lbl->setPixmap(QPixmap::fromImage(img));
}


void HTMLSettings::on_btnApply_clicked()
{
    // pack_params
    QVariant params;
    params = m_view->page()->mainFrame()->documentElement().evaluateJavaScript("Z_PACK_PARAMS()");
    emit change_params(params.toString());
    if(!ui->chkDontClose->isChecked()) hide();

}
