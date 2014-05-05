#include <QDebug>
#include "IManager.h"
#include "mixer.h"

static void blend(uint8* bkg, int bw, int bh, uint8* src, int sx, int sy,  int sw, int sh, int opacity);

static void g_toMix(void* pObj, uint8* pbuf, int x, int y, int w, int h, double alpha){
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
    global_manager->setMixFunction(scene_id, (void*)&g_toMix , this);
}

void Mixer::toMix(uint8 *pbuf, int x, int y, int w, int h, double alpha)
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
        return;
    }

    // накладываем поступивший слой сверху
    if(alpha > 0){
        blend(m_result, m_w, m_h, pbuf, x, y, w,  h, 1.0 - alpha);
    }

}

static void blend(uint8 *bkg, int bw, int bh, uint8 *src, int sx, int sy,  int sw, int sh, int opacity){    
    int step = 4;
    int i,j;
    int x, y;
    int k, alph;
    int step_i_sw;
    float opc;
    // бежим по накладываемой картинке (по логическим координатам)
    for(i=0;i<sh;i++){
        step_i_sw = step*i*sw;
        y = i + sy;

        // если логические Y-координаты точки вне зоны фона, то пропускаем ряд
        if(y <0 || y > bh-1){
            continue;
        }

        for(j=0;j<sw;j++){
            // логические координаты точки относительно фона
            x = j + sx;
            // если логические X-координаты точки вне зоны фона, то пропускаем точку
            if(x<0 || x > bw-1){
                continue;
            }
            // иначе берём точку с фона и накладываем на неё точку картинки
            k = y*bw + x; // логический индекс на фоне
            k = k*step; // реальный индекс

            alph = (&src[ step_i_sw+step*j])[3];
            if(alph == 255){
                memcpy(&bkg[k], &src[ step_i_sw+step*j], 3);
            }else
            {
                if(alph != 0){
                    opc = (1 - (float)(&src[ step_i_sw+step*j])[3]/255 );
                    ColorBlend_AlphaBuffer2(&bkg[k], &src[ step_i_sw+step*j], &bkg[k], ChannelBlend_Normal2, opc);
                }
            }
        }

    }
}

