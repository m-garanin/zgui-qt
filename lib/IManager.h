#ifndef IMANAGER_H
#define IMANAGER_H

#include <QDebug>

class IManager
{
public:
    // стартует видео-микшер со сценой размером width x height
    virtual void startPipeline(int width, int height) = 0;

    ////////////////////////////////////////////////
    // работа с видео
    virtual int addScene() = 0;

    // возвращает превью-картинку слоя (или сцены)
    virtual void getLastImage(int layer_id, char** ppbuf, int* pw, int* ph) = 0;

    // добавляет слой в сцену, возвращает полный ключ слоя. при этом добавление - отложенное.
    virtual int addLayer(int scene_id, char* source_key, int zorder) = 0;

    //////////////////////////////////////////////////////////////
    // работа с видео-слоем
    virtual void hideLayer(int layer_id) = 0;
    virtual void showLayer(int layer_id) = 0;
    virtual void resizeLayer(int layer_id, char* pos) = 0;
    virtual void applyEffects(int layer_id, char* efnames) = 0;
    virtual void removeEffects(int layer_id) = 0;

    // масштабирование на всю область и показ слоя
    // сохранением aspect-ratio и центрированием
    virtual void showLayerMax(int layer_id) = 0;


    //////////////////////////////////////////////////////////////
    // аудио-методы    
    virtual bool addAudioSource(char* source_key) = 0; // false-если источник уже есть
    virtual void toggleMute(char* srcname) = 0;
    virtual void setVolume(char* srcname, double vol) = 0;

    //
    virtual void startAir(int ch_id, char* pwd,
                          char* param_fname, char* server_fname, char* log_fname,
                          int width, int height, int bitrate, char tarif, char quality, int acc,
                          int test) = 0;

    virtual void stopAir() = 0;

};

extern IManager* global_manager;
extern void init_core();
#endif // IMANAGER_H
