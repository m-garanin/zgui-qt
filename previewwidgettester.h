#ifndef PREVIEWWIDGETTESTER_H
#define PREVIEWWIDGETTESTER_H

#include <QWidget>

namespace Ui {
class PreviewWidgetTester;
}

class PreviewWidgetTester : public QWidget
{
    Q_OBJECT
    
public:
    explicit PreviewWidgetTester(QWidget *parent = 0);
    ~PreviewWidgetTester();
    
private slots:
    void on_comboBox_currentIndexChanged(int index);

    void on_toolButton_clicked();

private:
    Ui::PreviewWidgetTester *ui;
};

#endif // PREVIEWWIDGETTESTER_H
