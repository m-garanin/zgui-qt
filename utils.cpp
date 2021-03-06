#include "utils.h"

#include <QAudioDeviceInfo>
#include <QCamera>
#include <QString>
#include <QDebug>


#ifdef Q_OS_WIN32
#include <comdef.h>
#include <comutil.h>
#include <dshow.h>


QStringList getCaptureDevices(GUID catGuid);
#endif


QStringList getAudioCaptureDevicesQt()
{
    QStringList result = QStringList() << "AS-A" << "AS-B";
    QList<QAudioDeviceInfo> l = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    foreach (QAudioDeviceInfo devInfo, l) {
        result << devInfo.deviceName();
    }
    return result;
}

QStringList getVideoCaptureDevicesQt()
{
    QStringList result = QStringList() << "VS-A" << "VS-B";
    QList<QByteArray> l = QCamera::availableDevices();
    foreach (QByteArray devInfo, l) {
        result << QCamera::deviceDescription(devInfo); // returns device name like "USB2.0 Camera" under Ubuntu 12.04 (Qt5)
        //result << devInfo; // returns device filename under Ubuntu 12.04 (Qt5), like /dev/videoX
    }
    return result;
}

// возвращает список устройств видео-захвата в формате json
QStringList getVideoCaptureDevices()
{

#ifdef Q_OS_WIN32
    return getCaptureDevices(CLSID_VideoInputDeviceCategory);
#elif defined Q_OS_UNIX
    return getVideoCaptureDevicesQt();
#endif

//    return getVideoCaptureDevicesQt();
}

// возвращает список устройств аудио-захвата в формате json
QStringList getAudioCaptureDevices()
{
    //return getCaptureDevices(CLSID_AudioInputDeviceCategory);
    return getAudioCaptureDevicesQt();
}


#ifdef Q_OS_WIN32
QStringList getCaptureDevices(GUID catGuid) //, QList<IMoniker*>& monList)
{
    // Create the System Device Enumerator.
    QStringList list;
    HRESULT hr;
    ICreateDevEnum *pDevEnum = NULL;
    IEnumMoniker *pEnum = NULL;

    // тест-источники
    if( catGuid == CLSID_VideoInputDeviceCategory){
        list.append("VS-A");
        list.append("VS-B");
    }else{
        list.append("AS-A");
        list.append("AS-B");
    }

    //

    hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,
                          IID_ICreateDevEnum, (void **)&pDevEnum);
    //CHECK(hr, "create SystemDeviceEnum");

    // заполнение списка видео-источников
    hr = pDevEnum->CreateClassEnumerator(catGuid,&pEnum, 0);
    //CHECK(hr, "create ClassEnumerator");

    if(pEnum == NULL)
        qDebug() << "bad pEnum";

    IMoniker *pMoniker = NULL;
    while ( (pEnum != NULL) && pEnum->Next(1, &pMoniker, NULL) == S_OK)
    {
        IPropertyBag *pPropBag;
        hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pPropBag);
        if (SUCCEEDED(hr))
        {
                    // To retrieve the filter's friendly name, do the following:
            VARIANT varName;
            VariantInit(&varName);
            hr = pPropBag->Read(L"FriendlyName", &varName, 0);
            if (SUCCEEDED(hr))
            {
                char* pN = _com_util::ConvertBSTRToString(varName.bstrVal);
                QString txt = QString::fromLocal8Bit(pN); // иначе кракозябры вместо кириллицы

                list.append(txt);
            }

            VariantClear(&varName);

            pPropBag->Release();
        }
        pMoniker->Release();
    }

    if(pEnum != NULL)
        pEnum->Release();

    if(pDevEnum != NULL)
        pDevEnum->Release();

    return list;
}
#endif

