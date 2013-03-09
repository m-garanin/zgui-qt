#include "previewwidgettester.h"
#include "ui_previewwidgettester.h"
#include "previewwidget.h"

#include <QVariant>
#include <QFileDialog>
#include <QImage>

PreviewWidgetTester::PreviewWidgetTester(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PreviewWidgetTester)
{
    ui->setupUi(this);
    ui->comboBox->addItem("Fit", qVariantFromValue((int)PreviewWidget::ImageFit) );
    ui->comboBox->addItem("Stretch", qVariantFromValue((int)PreviewWidget::ImageStretch) );

}

PreviewWidgetTester::~PreviewWidgetTester()
{
    delete ui;
}

void PreviewWidgetTester::on_comboBox_currentIndexChanged(int index)
{
    int imageMode = ui->comboBox->itemData(index).toInt();
    ui->previewWidget->setImageFitMode(PreviewWidget::ImageFitMode(imageMode));
}

void PreviewWidgetTester::on_toolButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select image"), "",
                                                    "Images (*.png *.gif *.jpg)");
    if (!fileName.isEmpty()) {
        QImage * img = new QImage();
        img->load(fileName);
        ui->previewWidget->drawImage(img);
    }
}
