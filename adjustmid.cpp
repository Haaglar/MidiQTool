#include "adjustmid.h"
#include "constants.h"
#include <iostream>
AdjustMid::AdjustMid(void)
{
    highPoint = Constants::MIN_MIDI_PITCH;
    lowPoint = Constants::MAX_MIDI_PITCH;
}
AdjustMid::~AdjustMid(void)
{
}
void AdjustMid::SetMidi(string path)
{
    mid.read(path);
    AdjustMid::FindHighLowPoints();
    AdjustMid::FindFirstTempo();
}

int AdjustMid::TrimStart()
{
    int iTicks = 999999;
    //Find
    for (int track = 0; track < mid.getTrackCount(); track++)
    {
        for (int eventNo = 0; eventNo < mid[track].size(); eventNo++)
        {
            if (mid[track][eventNo].isNoteOn()) //Find first note on for track
            {
                iTicks = min(mid[track][eventNo].tick, iTicks);
                break;
            }
        }
    }
    if (iTicks > 0 && iTicks != 999999)
    {
        //iterate over it all
        for (int track = 0; track < mid.getTrackCount(); track++)
        {
            for (int eventNo = 0; eventNo < mid[track].size(); eventNo++)
            {
                if (mid[track][eventNo].tick >= iTicks)
                    mid[track][eventNo].tick -= iTicks;
                else
                    mid[track][eventNo].tick = 0;
            }
        }
        cout << "Complete" << endl;
        return(0);
    }
    cout << "File can't be rest trimed." << endl;
    return(1);
}

int AdjustMid::AdjustTempoAndNotes(double adjustment)
{
    for (int track = 0; track < mid.getTrackCount(); track++)
    {
        for (int eventNo = 0; eventNo < mid[track].size(); eventNo++)
        {
            mid[track][eventNo].tick = (int)(mid[track][eventNo].tick * adjustment);
            if (mid[track][eventNo].isTempo())
                mid[track][eventNo].setTempo(mid[track][eventNo].getTempoBPM() * adjustment);
        }
    }
    firstTempo *= adjustment;
    return 0;
}
int AdjustMid::AdjustNotePitch(int amount)
{
    if( amount > 0 && (highPoint + amount) > Constants::MAX_MIDI_PITCH)
        return 1;
    else if (amount < 0 && (lowPoint + amount) < Constants::MIN_MIDI_PITCH)
        return 1;
    for (int track = 0; track < mid.getTrackCount(); track++)
    {
        for (int eventNo = 0; eventNo < mid[track].size(); eventNo++)
        {
            if (mid[track][eventNo].isNote())
                mid[track][eventNo][1]+= amount;
        }
    }
    highPoint += amount;
    return 0;
}
int AdjustMid::FindHighLowPoints()
{
    for (int track = 0; track < mid.getTrackCount(); track++)
    {
        for (int eventNo = 0; eventNo < mid[track].size(); eventNo++)
        {
            if (mid[track][eventNo].isNote())
            {
                lowPoint = min(mid[track][eventNo].getKeyNumber(),lowPoint);
                highPoint = max(mid[track][eventNo].getKeyNumber(),highPoint);
            }
        }
    }
    return 0;
}

int AdjustMid::FindFirstTempo()
{
    for (int eventNo = 0; eventNo < mid[0].size(); eventNo++)
    {
        if(mid[0][eventNo].isTempo())
        {
            firstTempo = mid[0][eventNo].getTempoBPM();
            return firstTempo;
        }
    }
    return 0;
}

void AdjustMid::CutMidi(int startTick, int endTick)
{
    for (int track = 0; track < mid.getTrackCount(); track++)
    {
        for (int eventNo = 0; eventNo < mid[track].size(); eventNo++)
        {
            if(mid[track][eventNo].tick >= startTick)
                break;
            if (mid[track][eventNo].isNote())
            {
                mid[track].remove(eventNo);
                eventNo--;
            }
        }
    }
    AdjustMid::TrimStart();
}
