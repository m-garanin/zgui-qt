#ifndef PREVIEWWIDGET_H
#define PREVIEWWIDGET_H

#include <QWidget>
#include <QMouseEvent>

class PreviewWidget : public QWidget
{
    Q_OBJECT
public:
    enum ImageFitMode {
        ImageFit,
        ImageStretch
    };

    explicit PreviewWidget(qint32 compkey, QWidget *parent = 0);
    void drawImage(QImage* img);    
    ImageFitMode imageFitMode() const { return m_imageFitMode; }
    void setImageFitMode(ImageFitMode mode);

    QImage image();

    int getCompkey() {return m_compkey;}

protected:
    void paintEvent(QPaintEvent *);

public slots:
    void updatePreview();

private:
    int m_compkey;
    QImage* m_currentImage;
    ImageFitMode m_imageFitMode;

    void start(); // запускает процесс обновления
};

#endif // PREVIEWWIDGET_H
