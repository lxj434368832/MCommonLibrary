#ifndef MEDIAPLAYEVENT_H
#define MEDIAPLAYEVENT_H

enum class EMediaPlayEvent
{
    empe_Unknown,
    empe_MediaLengthChaged,
    empe_PositionChanged,
    empe_VolumnChanged,
};

struct SMediaPlayEvent
{
    int           type;
    void        *p_obj;

    union
    {
       unsigned unMediaLength;
       unsigned uPosition;
       unsigned uVolumn;
    };
};

typedef void (*MediaPlayerCallback) (const SMediaPlayEvent *);

#endif // MEDIAPLAYEVENT_H
