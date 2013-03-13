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
    explicit CSceneWidget(qint32 compkey, QWidget *parent = 0);

    void showBox(int layer_id);
    QStringList apply();

protected:
    void dropEvent(QDropEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);

    void mousePressEvent(QMouseEvent *);

private:
    qint32 findPreviewWidget(const QPoint &);

private:
    QList<CBoxWidget*> _boxWidgetList;
    bool _enableDragAndDrop;
};

#endif // SCENEWIDGET_H
