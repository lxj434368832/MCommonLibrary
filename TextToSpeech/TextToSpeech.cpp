/*
#define _ATL_APARTMENT_THREADED
#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override something,
//but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>*/

#include <sapi.h>

#pragma comment(lib, "SAPI.lib")

int main(int argc, char* argv[])
{
    ISpVoice * pVoice = NULL;

    if (FAILED(::CoInitialize(NULL)))
        return -1;

    HRESULT hr = ::CoCreateInstance(CLSID_SpVoice, nullptr, CLSCTX_ALL, IID_ISpVoice, (void **)&pVoice);
    if( SUCCEEDED( hr ) )
    {
        hr = pVoice->Speak(L"ÎÒ°®Äã123", 0, NULL);
        pVoice->Release();
        pVoice = NULL;
    }

    ::CoUninitialize();
    return 0;
}
