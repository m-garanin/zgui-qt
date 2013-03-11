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

    explicit PreviewWidget(QWidget *parent = 0);
    void drawImage(QImage* img);
    void start(int compkey, int ms); // устанавливает ключ и запускает процесс обновления
    ImageFitMode imageFitMode() const { return m_imageFitMode; }
    void setImageFitMode(ImageFitMode mode);

    QImage image() const;

protected:
    void paintEvent(QPaintEvent *);
    bool event(QEvent *);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

//    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

public slots:
    void updatePreview();

private:
    QImage* m_currentImage;
    ImageFitMode m_imageFitMode;
    int m_compkey; // ???? ????????? ??? preview (????? ??? ????)
    bool _resizeBegin;
};

#endif // PREVIEWWIDGET_H
