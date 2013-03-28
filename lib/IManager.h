#ifndef IMANAGER_H
#define IMANAGER_H

#include <QDebug>

class IManager
{
public:
    // �������� �����-������ �� ������ �������� width x height
    virtual void startPipeline(int width, int height) = 0;

    ////////////////////////////////////////////////
    // ������ � �����
    virtual int addScene() = 0;

    // ���������� ������-�������� ���� (��� �����)
    virtual void getLastImage(int layer_id, char** ppbuf, int* pw, int* ph) = 0;

    // ��������� ���� � �����, ���������� ������ ���� ����. ��� ���� ���������� - ����������.
    virtual int addLayer(int scene_id, char* source_key, int zorder) = 0;

    //////////////////////////////////////////////////////////////
    // ������ � �����-�����
    virtual void hideLayer(int layer_id) = 0;
    virtual void showLayer(int layer_id) = 0;
    virtual void resizeLayer(int layer_id, char* pos) = 0;
    virtual void applyEffects(int layer_id, char* efnames) = 0;
    virtual void removeEffects(int layer_id) = 0;

    // ��������������� �� ��� ������� � ����� ����
    // ����������� aspect-ratio � ��������������
    virtual void showLayerMax(int layer_id) = 0;


    //////////////////////////////////////////////////////////////
    // �����-������    
    virtual bool addAudioSource(char* source_key) = 0; // false-���� �������� ��� ����
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
