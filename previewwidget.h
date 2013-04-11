#ifndef PREVIEWWIDGET_H
#define PREVIEWWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QSize>

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

    int getCompkey() {return m_compkey;}

    void start(); // запускает процесс обновления
    void stop();


    QPoint getTopLeftPoint() const { return m_top_left; }    // возвращает левый верхний угол картинки
    QSize getImageSize() const {return m_img_size; } // размеры картинки

protected:
    void paintEvent(QPaintEvent *);
    void setTransparency(int value); // 0 <= value  <= 100
    int transparency() const { return m_transparency; }

public slots:
    void updatePreview();


private:
    int m_compkey;
    QImage* m_currentImage;
    ImageFitMode m_imageFitMode;
    QTimer *timer;
    QPoint m_top_left;
    QSize m_img_size;
    int   m_transparency;
};

#endif // PREVIEWWIDGET_H
