#include "adjustmid.h"
#include "constants.h"
#include <iostream>
AdjustMid::AdjustMid()
{
    highPoint = Constants::MIN_MIDI_PITCH;
    lowPoint = Constants::MAX_MIDI_PITCH;
}
AdjustMid::SetMidi(string path)
{
    mid.read(path);
    AdjustMid::findHighLowPitch();
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

int AdjustMid::AdjustTempoAndNotes(float adjustment)
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
    return 0;
}
int AdjustMid::findHighLowPitch()
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
