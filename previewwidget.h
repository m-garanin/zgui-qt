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

    explicit PreviewWidget(qint32 compkey, bool need_quality, QWidget *parent);

    ImageFitMode imageFitMode() const { return m_imageFitMode; }
    void setImageFitMode(ImageFitMode mode);

    int getCompkey() {return m_compkey;}

    void start(); // запускает процесс обновления
    void stop();


    QPoint getTopLeftPoint() const { return m_rec.topLeft(); }    // возвращает левый верхний угол картинки
    QSize getOriginalImageSize() const {return m_orig_size; } // размеры оригинальной картинки
    QSize getImageSize() const {return m_img_size; } // размеры текущей картинки с учётом растяжения

protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent * event);
    void setTransparency(int value); // 0 <= value  <= 100
    int transparency() const { return m_transparency; }
    void recalcPosition();
    void drawImage(QImage* img);

public slots:
    void updatePreview();


private:
    int m_compkey;
    int m_prv_num;
    bool m_need_quality;
    QImage* m_currentImage;
    ImageFitMode m_imageFitMode;
    QTimer *timer;
    QPoint m_top_left;

    QRect m_rec; // область, в которой отображать превью-картинку
    QSize m_img_size;
    QSize m_orig_size;

    int   m_transparency;
    int m_temp_count;
};

#endif // PREVIEWWIDGET_H
