#ifndef MANAGER_H
#define MANAGER_H

#include <QStringList>
#include <QMap>

#include "../lib/IManager.h"


class IRenderer;

class Manager:public IManager
{
public:
    Manager();
    ~Manager(void);

    //////////////////////////////////////////////
    // IManager implementations
    void startPipeline(int width, int height);
    void stopPipeline();

    int addScene(int zorder);
    void getLastImage(int layer_id, char** ppbuf, int* pw, int* ph);
    int addLayer(int scene_id, char* source_key, int zorder);

    void hideLayer(int layer_id);
    void showLayer(int layer_id);    
    void repositionLayer(int layer_id, double x, double y, double w, double h);
    void applyEffects(int layer_id, char* efnames);
    void removeEffects(int layer_id);
    void showLayerMax(int layer_id);


    bool addAudioSource(char* source_key); // false-если источник уже есть
    void toggleMute(char* srcname);
    void setVolume(char* srcname, double vol);


    void startAir(int ch_id, char* pwd,
                  char* param_fname, char* server_fname, char* log_fname,
                  int width, int height, int bitrate, char tarif, char quality, int acc,
                  int test);
    void stopAir();
    //////////////////////////////////////////////

private:
    int width, height;
    int scene_count;
    int layer_count;
    QStringList audios;
    QMap<int, IRenderer*> m_renderers;
};

#endif // MANAGER_H
