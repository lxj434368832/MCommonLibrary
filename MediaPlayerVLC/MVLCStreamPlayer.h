#ifndef MVLCSTREAMPLAYER_H
#define MVLCSTREAMPLAYER_H

#include "MVLCPlayer.h"

class MVLCStreamPlayer : public MVLCPlayer
{
public:
    MVLCStreamPlayer();

    //url:Ҫ���ŵ���ý���ַ
    bool setMedia(const char* url)override;
};

#endif // MVLCSTREAMPLAYER_H
