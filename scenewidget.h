#ifndef SCENEWIDGET_H
#define SCENEWIDGET_H

#include <QDropEvent>
#include <QMenu>

#include "previewwidget.h"
#include "boxwidget.h"

class CSceneWidget : public PreviewWidget
{
    Q_OBJECT
public:
    explicit CSceneWidget(QWidget *parent = 0);

    void showBox(int layer_id);

protected:
    void dropEvent(QDropEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);

    void mousePressEvent(QMouseEvent *);

private:
    qint32 findPreviewWidget(const QPoint &);

private:
    QList<CBoxWidget*> _boxWidgetList;
};

#endif // SCENEWIDGET_H
