#ifndef ADJUSTMID_H
#define ADJUSTMID_H

#include "midifile/MidiFile.h"

class AdjustMid
{
public:
    AdjustMid();
    SetMidi(string path);
    int TrimStart ();
    int AdjustTempoAndNotes(float adjustment);
    int AdjustNotePitch(int amount);
    int findHighLowPitch();
    MidiFile mid;
    int highPoint;
    int lowPoint;
};



#endif // ADJUSTMID_H
