#ifndef IMANAGER_H
#define IMANAGER_H
#include "scene.h"
#include "layer.h"
#include <QVariant>

typedef void (*app_logger_callback)(char* buffer);
typedef unsigned long long uint64;

class IExternalSource
{
public:
    virtual void sendFrame(char* buf, int size, int width, int height) = 0;
};

class IManager
{
public:
    virtual void start(int width, int height) = 0;
    virtual void stop() = 0;
    virtual void logger(char* buf) = 0;



    virtual Scene* getScene() = 0;

    virtual QObject* addSource(QString type, QString sourcename, QVariant ainfo) = 0;

    virtual void setWorksize(int w, int h) = 0;
    virtual void setBackground(QString fname) = 0;

    // rec
    virtual void startRec(char* fname, int width, int height, int vbr, int ar) = 0;
    virtual void stopRec() = 0;

    // air
    virtual void startAir(int ch_id, char* pwd,
                          char* param_fname, char* server, char* log_fname,
                          int width, int height, int bitrate, char tarif, char quality, int acc,
                          int test) = 0;

    virtual void stopAir() = 0;

};

class IManagerOLD
{
public:
    virtual int checkKey(char* key) = 0; // проверяет лицензионный ключ. 0-ключ невалиден; 1-ключ валиден; 2-ключ валиден, но следует обновить.

    // стартует видео-микшер со сценой размером width x height
    virtual void startPipeline(int width, int height, app_logger_callback plogger) = 0;
    virtual void stopPipeline() = 0;
    virtual void setWorksize(int w, int h) = 0;
    virtual void setBackground(int pattern) = 0;
    virtual int getLayersCount(int scene_id) = 0;
    virtual int getProxySceneId(int layer_id) = 0;
    virtual void getDualDevices(char** pb, int* pcount) = 0;
    ////////////////////////////////////////////////
    // работа с видео
    virtual int addScene() = 0;
    virtual void setMixFunction(int scene_id, void* pFunction, void* pData);

    // возвращает превью-картинку слоя (или сцены)
    virtual void getPreview(int layer_id, char** ppbuf, int* psize, int* pw, int* ph, int* pnum) = 0;
    virtual void unrefPreview(int layer_id) = 0;


    // добавляет слой в сцену, возвращает полный ключ слоя. при этом добавление - отложенное.
    virtual int addLayer(int scene_id, char* type, char* source_key, char* ainfo) = 0;

    //////////////////////////////////////////////////////////////
    // работа с видео-слоем
    virtual void deleteLayer(int layer_id) = 0;
    virtual void hideLayer(int layer_id) = 0;
    virtual void showLayer(int layer_id) = 0;
    virtual void repositionLayer(int layer_id, double x, double y, double w, double h) = 0;
    virtual void applyEffects(int layer_id, char* efnames) = 0;
    virtual void removeEffects(int layer_id) = 0;
    virtual void setZOrder(int layer_id, int zorder) = 0;

    // масштабирование на всю область и показ слоя
    // сохранением aspect-ratio и центрированием
    virtual void setLayerOptimalSize(int layer_id) = 0;
    virtual void setLayerFullSize(int layer_id) = 0;


    // работа с external-источником
    //virtual void sendExternalFrame(char* source_key, char* buf, int size, int width, int height) = 0;
    virtual void queryIExternalSource(char* source_key, IExternalSource** ppOut) = 0;
    //virtual void queryIPlaybackSource(int layer_id, IPlaybackSource** ppOut) = 0;

    //////////////////////////////////////////////////////////////
    // аудио-методы    
    virtual bool addAudioSource(char* source_key) = 0; // false-если источник уже есть
    virtual void deleteAudioSource(char* source_key) = 0;
    virtual void mute(char* srcname) = 0;
    virtual void unmute(char* srcname) = 0;
    virtual void setVolume(char* srcname, double vol) = 0;
    virtual void getAudioLevels(double* values, int* size) = 0;

    //
    virtual void startAir(int ch_id, char* pwd,
                          char* param_fname, char* server, char* log_fname,
                          int width, int height, int bitrate, char tarif, char quality, int acc,
                          int test) = 0;

    virtual void stopAir() = 0;
    virtual void getAirStat(uint64* total_bytes, uint64* total_frames) = 0;

    // rec
    virtual void startRec(char* fname, int width, int height, int vbr, int ar) = 0;
    virtual void stopRec() = 0;
    virtual void getRecStat(uint64* total_bytes, uint64* total_frames) = 0;

    // получение инфы о слое (в процентах)
    virtual void getLayerPosition(int layer_id, double* px, double* py, double* pw, double* ph, int* pz) = 0;

};

extern IManager* global_manager;
#endif // IMANAGER_H
