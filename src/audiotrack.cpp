#include "audiotrack.h"

bool AudioTrack::isValid() const
{
    return !title.trimmed().isEmpty() && url.isValid();
}
