#ifndef MANAGER_H
#define MANAGER_H

#include "IManager.h"

class Manager:public IManager
{
public:
    Manager();
    ~Manager(void);

    void startPipeline(int width, int height);

    int addLayer(int scene_key, char* source_key, int zorder);

    //////////////////////////////////////////////////////////////
    // работа с видео-слоем
    /*
    void hideLayer(int compkey);
    void showLayer(int compkey);
    void resizeLayer(int compkey, char* pos);

    void applyEffects(int compkey, char* efnames);
    void removeEffects(int compkey);

    void showLayerMax(int compkey);
    */
    // масштабирование на всю область и показ слоя
    // сохранением aspect-ratio и центрированием


    //////////////////////////////////////////////////////////////
    // аудио-методы
    /*
    bool addAudioSource(char* source_key); // false-если источник уже есть
    void toggleMute(char* srcname);
    void setVolume(char* srcname, double vol);
    */


    void getLastImage(int compkey, char** ppbuf, int* pw, int* ph);

    int addScene();
    void addSource(char* key);

    int test();


private:
    int width, height;


};

#endif // MANAGER_H
