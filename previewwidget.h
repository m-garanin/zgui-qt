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

public slots:
    void updatePreview();

private:
    QImage* m_currentImage;
    ImageFitMode m_imageFitMode;
    int m_compkey; // ???? ????????? ??? preview (????? ??? ????)
};

#endif // PREVIEWWIDGET_H
