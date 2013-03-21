#ifndef IRENDERER_H
#define IRENDERER_H

#include <QImage>
class QSize;

class IRenderer
{
public:
    virtual void setSize(const QSize&) = 0;
    virtual QImage render() = 0;
    virtual ~IRenderer() {}
};



#endif // IRENDERER_H
