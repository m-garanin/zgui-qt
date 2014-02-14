#ifndef MIXER_H
#define MIXER_H

class Mixer
{
public:
    Mixer();

    void connectMixerToScene(int scene_id);
    void toMix(char* pbuf, int x, int y, int w, int h, double alpha);
private:
    char* m_result;
    int m_w, m_h; // width & height result
};

#endif // MIXER_H
