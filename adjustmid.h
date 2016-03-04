#ifndef ADJUSTMID_H
#define ADJUSTMID_H

#include "midifile/MidiFile.h"

class AdjustMid
{
public:
    //Methods
    AdjustMid();
    ~AdjustMid();
    void SetMidi(string path);
    int TrimStart ();
    int AdjustTempoAndNotes(double adjustment);
    int AdjustNotePitch(int amount);
    int FindHighLowPoints();
    int FindFirstTempo();
    void CutMidi(int startTick, int endTick);
    //Varibles
    double firstTempo;
    int highPoint;
    int lowPoint;

    MidiFile mid;
};



#endif // ADJUSTMID_H
