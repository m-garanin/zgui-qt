#include <QDebug>
#include "IManager.h"
#include "mixer.h"

void g_toMix(void* pObj, char* pbuf, int x, int y, int w, int h, double alpha){
    Mixer* pm = (Mixer*)pObj;
    pm->toMix(pbuf, x, y, w, h, alpha);
}

Mixer::Mixer()
{
}

void Mixer::connectMixerToScene(int scene_id)
{
    m_result = nullptr;
    // установка микшер-функции
    global_manager->setMixFunction(scene_id, &g_toMix , this);// TODO

}

void Mixer::toMix(char *pbuf, int x, int y, int w, int h, double alpha)
{
    // слои поступают на микс в z-порядке,
    // когда pbuf == NULL, то считаем что микс закончен и требуется результат,
    // который записывается в первый буфер.
    // все буферы формата RGBA.

    // если это запрос о завершении микширования
    if(pbuf == nullptr){
        //qDebug() << "MIX END";
        m_result = nullptr;
        return;
    }

    // ловим первый буфер (фон)
    if(m_result == nullptr){
        m_result = pbuf;
        m_w = w;
        m_h = h;
    }



}
