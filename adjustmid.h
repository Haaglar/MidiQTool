#ifndef ADJUSTMID_H
#define ADJUSTMID_H

#include "midifile/MidiFile.h"
#include <algorithm>
class AdjustMid
{
public:
    //Methods
    AdjustMid();
    ~AdjustMid();
    void setMidi(string path);
    int trimStart ();
    int adjustTempoAndNotes(double adjustment);
    int adjustNotePitch(int amount);
    int findHighLowPoints();
    int findFirstTempo();
    void cutMidi(int startTick, int endTick);
    void removeAdditionalVolume(int volume);
    void setNoteAttackVolume(int volume);
    void removeShortNotes(int length);
    int getLastTick();
    //Varibles
    double firstTempo;
    int highPoint;
    int lowPoint;

    MidiFile mid;
};



#endif // ADJUSTMID_H
