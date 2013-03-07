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
    // ������ � �����-�����
    /*
    void hideLayer(int compkey);
    void showLayer(int compkey);
    void resizeLayer(int compkey, char* pos);

    void applyEffects(int compkey, char* efnames);
    void removeEffects(int compkey);

    void showLayerMax(int compkey);
    */
    // ��������������� �� ��� ������� � ����� ����
    // ����������� aspect-ratio � ��������������


    //////////////////////////////////////////////////////////////
    // �����-������
    /*
    bool addAudioSource(char* source_key); // false-���� �������� ��� ����
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
