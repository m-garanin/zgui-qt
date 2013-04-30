#ifndef ZCORE_H
#define ZCORE_H

#ifdef ZCORE_EXPORTS
#define ZCORE_API __declspec(dllexport)
#else
#define ZCORE_API __declspec(dllimport)
#endif

ZCORE_API int testZCore(void);
ZCORE_API void getGlobalManager(void** ppManager);


class Zcore
{
public:
    Zcore();
};

#endif // ZCORE_H
