#ifndef XINTERFACES_H
#define XINTERFACES_H

typedef void (*app_logger_callback)(char* buffer);
typedef void (*app_buffer_callback)(int type, char* buffer, int w, int h, int size, void* userdata);

class IPlaybackSource
{
public:
    virtual void play() = 0;
    virtual void pause() = 0;

    // sec
    virtual int getDuration() = 0;
    virtual int getPosition() = 0;
    virtual void seek(int sec) = 0;
};


class IXManager
{
public:
    virtual void init(app_buffer_callback func, app_logger_callback logger) = 0;

    virtual void createCamSource(char* name, int pref_w, int pref_h, void* userdata) = 0;

    virtual void createNetSource(char* uri, void* userdata) = 0;
    virtual void createVideoFileSource(char* fname, IPlaybackSource** ppOut, void* userdata) = 0;

};
#endif // XINTERFACES_H
