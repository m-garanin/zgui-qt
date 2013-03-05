#ifndef PREVIEWWIDGET_H
#define PREVIEWWIDGET_H

#include <QWidget>

class PreviewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PreviewWidget(QWidget *parent = 0);
    void drawImage(QImage* img);
    void start(int compkey, int ms); // устанавливает ключ и запускает процесс обновления

protected:
    QImage* currentImage;
    int compkey; // ключ источника для preview (сцена или слой)

    void paintEvent(QPaintEvent *);
    void refresh();

signals:
    
public slots:

private slots:
    void updatePreview();

};

#endif // PREVIEWWIDGET_H
