#ifndef IMANAGER_H
#define IMANAGER_H

#include <QDebug>

class IManager
{
public:
    virtual void startPipeline(int width, int height) = 0;
    virtual int test() = 0;
    //void getLayersJSON(int scene_key);

    // ��������� ���� � �����, ���������� ������ ���� ����. ��� ���� ���������� - ����������.
    virtual int addLayer(int scene_key, char* source_key, int zorder) = 0;

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

    
    virtual void getLastImage(int compkey, char** ppbuf, int* pw, int* ph) = 0;

    virtual int addScene() = 0;
    virtual void addSource(char* key) = 0;
};

extern IManager* global_manager;
extern void init_core();
#endif // IMANAGER_H
