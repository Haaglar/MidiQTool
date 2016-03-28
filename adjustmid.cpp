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
void AdjustMid::setMidi(string path)
{
    mid.read(path);
    AdjustMid::findHighLowPoints();
    AdjustMid::findFirstTempo();
}

int AdjustMid::trimStart()
{
    int iTicks = 999999; //random number
    //Find
    for (int track = 0; track < mid.getTrackCount(); track++)
    {
        for (int eventNo = 0; eventNo < mid[track].size(); eventNo++)
        {
            if (mid[track][eventNo].isNoteOn()){ //Find first note on for track
                iTicks = min(mid[track][eventNo].tick, iTicks);
                break;
            }
        }
    }
    if (iTicks > 0 && iTicks != 999999){
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
        return(0);
    }
    cout << "File can't be rest trimed." << endl;
    return(1);
}

int AdjustMid::adjustTempoAndNotes(double adjustment)
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
int AdjustMid::adjustNotePitch(int amount)
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
int AdjustMid::findHighLowPoints()
{
    for (int track = 0; track < mid.getTrackCount(); track++)
    {
        for (int eventNo = 0; eventNo < mid[track].size(); eventNo++)
        {
            if (mid[track][eventNo].isNote()){
                lowPoint = min(mid[track][eventNo].getKeyNumber(),lowPoint);
                highPoint = max(mid[track][eventNo].getKeyNumber(),highPoint);
            }
        }
    }
    return 0;
}

int AdjustMid::findFirstTempo()
{
    for (int eventNo = 0; eventNo < mid[0].size(); eventNo++)
    {
        if(mid[0][eventNo].isTempo()){
            firstTempo = mid[0][eventNo].getTempoBPM();
            return firstTempo;
        }
    }
    return 0;
}

void AdjustMid::cutMidi(int startTick, int endTick)
{
    //first join
    mid.linkNotePairs();
    vector<vector<int>> toDelete( mid.getTrackCount());
    //Find the notes to delete
    for (int track = 0; track < mid.getTrackCount(); track++)
    {
        for (int eventNo = 0; eventNo < mid[track].size(); eventNo++)
        {
            if(startTick && mid[track][eventNo].tick < startTick ){ //Before the start point
                if (mid[track][eventNo].isPitchbend()){
                    toDelete[track].push_back(eventNo);
                    continue;
                }
                if (mid[track][eventNo].isNote()){
                    if(mid[track][eventNo].isNoteOff()){
                        toDelete[track].push_back(eventNo);
                        continue;
                    }
                    MidiEvent * linked = mid[track][eventNo].getLinkedEvent();
                    if(linked->tick < startTick){          //Check the linked note is is before the cut time
                     toDelete[track].push_back(eventNo);
                    }else{
                     mid[track][eventNo].tick = startTick; //Adjust it so its at the start of the midi
                    }
                }
            }else if (endTick){ //If specified an end tick
                if(mid[track][eventNo].tick <= endTick){ //Middle Cases
                    if (mid[track][eventNo].isNoteOn()){
                         MidiEvent * linked = mid[track][eventNo].getLinkedEvent();
                         if(linked->tick > endTick)
                            linked->tick = endTick;
                    }
                }else{
                     toDelete[track].push_back(eventNo);
                }
            }//End specified endtick block
        }//End event loop-
    }//End track loop
    for(int track = 0; track < mid.getTrackCount(); track++)
    {
        mid[track].removeList(toDelete[track]);
    }
    AdjustMid::trimStart(); //Shift everything down
    //Since the midihas changedwe need to check again
    AdjustMid::findHighLowPoints();

}
/*!
 * \brief AdjustMid::RemoveAdditionalVolume
 * \param volume
 */
void AdjustMid::removeAdditionalVolume(int volume)
{
    for (int track = 0; track < mid.getTrackCount(); track++)
    {
        int volumeFound = 0;
        for (int eventNo = 0; eventNo < mid[track].size(); eventNo++)
        {
            if(mid[track][eventNo].isVolume()){
                if(volumeFound == 0 && mid[track][eventNo][2] !=0){ //Dont want to set to 0
                    volumeFound++;
                    if(volume != 0)
                        mid[track][eventNo][2] = (volume & 0xff);
                }else{
                    mid[track].remove(eventNo);
                    eventNo--;
                }
            }
        }
    }
}
/*!
 * \brief AdjustMid::SetNoteAttackVolume
 * \param volume
 */
void AdjustMid::setNoteAttackVolume(int volume)
{
    for (int track = 0; track < mid.getTrackCount(); track++)
    {
        for (int eventNo = 0; eventNo < mid[track].size(); eventNo++)
        {
            if(mid[track][eventNo].isNote())
                mid[track][eventNo][2] = (volume & 0xff);
        }
    }
}

void AdjustMid::removeShortNotes(int length)
{
    //first join
    mid.linkNotePairs();
    vector<vector<int>> toDelete( mid.getTrackCount());
    //Find the notes to delete
    for (int track = 0; track < mid.getTrackCount(); track++)
    {
        for (int eventNo = 0; eventNo < mid[track].size(); eventNo++)
        {
            if(mid[track][eventNo].isNoteOn()){
                MidiEvent * linked = mid[track][eventNo].getLinkedEvent();
                int eventNoPairEnd = eventNo + 1;
                while(eventNoPairEnd < mid[track].size())
                {
                    if(linked == &mid[track][eventNoPairEnd]){
                        if((mid[track][eventNoPairEnd].tick - mid[track][eventNo].tick)< length){
                            toDelete[track].push_back(eventNo);
                            toDelete[track].push_back(eventNoPairEnd);
                        }
                        break;
                    }
                    eventNoPairEnd++;
                }//End pair note find
            }//End note test
        }//End current track
    }//End note find
    for(int track = 0; track < mid.getTrackCount(); track++)
    {
        mid[track].removeList(toDelete[track]);
    }
}
