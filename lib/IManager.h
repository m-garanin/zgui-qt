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
    virtual void getDualDevices(char** pb, int* pcount) = 0;
    ////////////////////////////////////////////////
    // работа с видео
    virtual int addScene() = 0;

    // возвращает превью-картинку слоя (или сцены)
    virtual void getPreview(int layer_id, char** ppbuf, int* psize, int* pw, int* ph, int* pnum) = 0;
    virtual void unrefPreview(int layer_id) = 0;


    // добавляет слой в сцену, возвращает полный ключ слоя. при этом добавление - отложенное.
    virtual int addLayer(int scene_id, char* type, char* source_key) = 0;

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
    virtual void sendExternalFrame(char* source_key, char* buf, int size, int width, int height) = 0;

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

    // получение инфы о слое (в процентах)
    virtual void getLayerPosition(int layer_id, double* px, double* py, double* pw, double* ph, int* pz) = 0;


};

extern IManager* global_manager;
#endif // IMANAGER_H
