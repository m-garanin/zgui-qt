#ifndef PREVIEWWIDGET_H
#define PREVIEWWIDGET_H

#include <QWidget>

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

protected:
    void paintEvent(QPaintEvent *);

protected:
    QImage* m_currentImage;
    int m_compkey; // ключ источника для preview (сцена или слой)

private slots:
    void updatePreview();

private:
    ImageFitMode m_imageFitMode;
};

#endif // PREVIEWWIDGET_H
