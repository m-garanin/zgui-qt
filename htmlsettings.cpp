#include "htmlsettings.h"
#include "ui_htmlsettings.h"


HTMLSettings::HTMLSettings(QWidget *parent) :
    QDialog(parent, Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint),
    ui(new Ui::HTMLSettings)
{
    ui->setupUi(this);
    m_view = new QWebView(this);
    //m_view->setGeometry(0,0,320,240);
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

