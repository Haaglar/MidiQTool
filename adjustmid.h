#ifndef ADJUSTMID_H
#define ADJUSTMID_H

#include "midifile/MidiFile.h"

class AdjustMid
{
public:
    AdjustMid();
    ~AdjustMid();
    void SetMidi(string path);
    int TrimStart ();
    int AdjustTempoAndNotes(float adjustment);
    int AdjustNotePitch(int amount);
    int FindHighLowPoints();
    int FindFirstTempo();

    int firstTempo;
    int highPoint;
    int lowPoint;

    MidiFile mid;
};



#endif // ADJUSTMID_H
