#ifndef MVLCSTREAMPLAYER_H
#define MVLCSTREAMPLAYER_H

#include "MVLCPlayer.h"

class MVLCStreamPlayer : public MVLCPlayer
{
public:
    MVLCStreamPlayer();

    //url:要播放的流媒体地址
    bool setMedia(const char* url)override;
};

#endif // MVLCSTREAMPLAYER_H
