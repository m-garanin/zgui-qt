#ifndef IMANAGER_H
#define IMANAGER_H

typedef unsigned long long uint64;

class IManager
{
public:
    // стартует видео-микшер со сценой размером width x height
    virtual void startPipeline(int width, int height) = 0;
    virtual void stopPipeline() = 0;
    virtual void setWorksize(int w, int h) = 0;
    virtual void setBackground(int pattern) = 0;
    virtual int getLayersCount(int scene_id) = 0;
    virtual int getProxySceneId(int layer_id) = 0;
    ////////////////////////////////////////////////
    // работа с видео
    virtual int addScene(int zorder) = 0;

    // возвращает превью-картинку слоя (или сцены)
    virtual void getLastImage(int layer_id, char** ppbuf, int* pw, int* ph) = 0;

    // добавляет слой в сцену, возвращает полный ключ слоя. при этом добавление - отложенное.
    virtual int addLayer(int scene_id, char* type, char* source_key, int zorder) = 0;

    //////////////////////////////////////////////////////////////
    // работа с видео-слоем
    virtual void hideLayer(int layer_id) = 0;
    virtual void showLayer(int layer_id) = 0;
    virtual void repositionLayer(int layer_id, double x, double y, double w, double h) = 0;
    virtual void applyEffects(int layer_id, char* efnames) = 0;
    virtual void removeEffects(int layer_id) = 0;

    // масштабирование на всю область и показ слоя
    // сохранением aspect-ratio и центрированием
    virtual void showLayerMax(int layer_id) = 0;


    //////////////////////////////////////////////////////////////
    // аудио-методы    
    virtual bool addAudioSource(char* source_key) = 0; // false-если источник уже есть
    virtual void mute(char* srcname) = 0;
    virtual void unmute(char* srcname) = 0;
    virtual void setVolume(char* srcname, double vol) = 0;
    virtual void getAudioLevels(double* values, int* size) = 0;

    //
    virtual void startAir(int ch_id, char* pwd,
                          char* param_fname, char* server_fname, char* log_fname,
                          int width, int height, int bitrate, char tarif, char quality, int acc,
                          int test) = 0;

    virtual void stopAir() = 0;
    virtual void getAirStat(uint64* total_bytes, uint64* total_frames) = 0;

    // rec
    virtual void startRec(char* fname, int width, int height, int vbr, int ar) = 0;
    virtual void stopRec() = 0;
    virtual void getRecStat(uint64* total_bytes, uint64* total_frames) = 0;

    //    
    virtual void free_memory(void*) = 0;
};

extern IManager* global_manager;
#endif // IMANAGER_H
