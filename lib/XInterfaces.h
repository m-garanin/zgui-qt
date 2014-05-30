#ifndef XINTERFACES_H
#define XINTERFACES_H

#define AUDIO_TYPE 0
#define VIDEO_TYPE 1

#define VIDEO_CAPS_WITH_SIZE "video/x-raw,format=BGRA,width=%d,height=%d,framerate=25/1"
#define VIDEO_CAPS_SIMPLE "video/x-raw,format=BGRA" // TODO framerate .
#define AUDIO_CAPS "audio/x-raw,format=(string)S16LE,channels=2,rate=44100"

typedef unsigned long long uint64;

typedef void (*app_logger_callback)(char* buffer);
typedef void (*app_buffer_callback)(int type, char* buffer, uint64 ts, uint64 drt, int w, int h, int size, void* userdata);


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

    // rec
    virtual void startRec(char* fname, int width, int height, int vbr, int ar) = 0;
    virtual void stopRec() = 0;
    virtual void sendRecBuffer(int type, char* buf, int size, uint64 ts, uint64 drt) = 0;

};
#endif // XINTERFACES_H
