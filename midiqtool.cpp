#include "midiqtool.h"
#include "ui_midiqtool.h"
#include "constants.h"
#include "iostream"

/* Note things that have been edited  on Midifile li b
 * MidiFile.cpp
 * - int MidiFile::read(const string& filename)
 *      -set rwstatus as 0 instead of 1 at start
 * - line 973 -> 975 commented out for single track split support
 * Add a new function called isVolume() (based off isController())
 * --Added 2 delte functions
 */
MidiQTool::MidiQTool(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MidiQTool)
{
    ui->setupUi(this);
    SetupValidators();
    midModifier = new AdjustMid();
}

MidiQTool::~MidiQTool()
{
    delete ui;
    delete midModifier;
    delete validatorMultiplier;
    delete validatorTempo;
    delete validatorIntHalfByte;
    delete validatorInt;
    delete validatorTime;
}
//--------------------File Options
//Open file
void MidiQTool::on_pushButtonOpenMidi_clicked()
{
    midiDir = QFileDialog::getOpenFileName(this,tr("Open Midi"),"","Midi file (*.mid *.midi)");
    if(midiDir == NULL)
        return;
    string sirLoc = midiDir.toStdString();
    midModifier->setMidi(sirLoc);
    //I hate this style option
    if(!midModifier->mid.status()){
        ui->statusBar->showMessage("Failed to read midi file.");
    }else{
        EnableGUI();
        ui->lineEditFile->setText(midiDir);
        outDir= midiDir.left(midiDir.lastIndexOf(".")) + " (modified).mid";
        ui->lineEditSaveLoc->setText(outDir);
        ui->statusBar->showMessage(QFileInfo(midiDir).fileName() + " opened successfully.");
    }
}
//Save file location
void MidiQTool::on_pushButtonSaveLocation_clicked()
{
    outDir = QFileDialog::getSaveFileName(this,tr("Save Midi"),"","Midi file (*.mid *.midi)");
    if(outDir != NULL)
        ui->lineEditSaveLoc->setText(outDir);
}
//Save file to location
void MidiQTool::on_pushButtonSave_clicked()
{
    string outLoc = outDir.toStdString();
    midModifier->mid.write(outLoc);
    ui->statusBar->showMessage("File saved");
}

//-----End file options


void MidiQTool::on_pushButtonTrimRest_clicked()
{
    midModifier->trimStart();
    ui->statusBar->showMessage("Start trimed");
}


//----------------Begin Tempo Note adujust methods

void MidiQTool::on_pushButtonTNUdjust_clicked()
{

    QString value = ui->lineEditNTMValue->text();
    float adjustment = 0;
    if(ui->radioButtonTempo->isChecked())
        adjustment = value.toFloat()/midModifier->firstTempo;
    else
        adjustment = value.toFloat();

    if(adjustment !=0){
        midModifier->adjustTempoAndNotes(adjustment);
        ui->statusBar->showMessage("Adjusted notes");
    }else{
        ui->statusBar->showMessage("Use a non 0 value");
    }
}

void MidiQTool::on_radioButtonMulti_toggled(bool checked)
{
    if(checked)
        ui->lineEditNTMValue->setValidator(validatorMultiplier);
    else
        ui->lineEditNTMValue->setValidator(validatorTempo);
    ui->lineEditNTMValue->setText("");
}
//----------------End Tempo Note adujust methods


//----------Note pitch shifiting
void MidiQTool::on_pushButtonOcUp_clicked()
{
    if(midModifier->adjustNotePitch(12))
        ui->statusBar->showMessage("Out of range.");
    else
        ui->statusBar->showMessage("Octave shifted up.");
}

void MidiQTool::on_pushButtonOctDown_clicked()
{
    if(midModifier->adjustNotePitch(-12))
        ui->statusBar->showMessage("Out of range.");
    else
        ui->statusBar->showMessage("Octave shifted down.");
}

void MidiQTool::on_pushButtonUpOne_clicked()
{
    if(midModifier->adjustNotePitch(1))
        ui->statusBar->showMessage("Out of range.");
    else
        ui->statusBar->showMessage("Notes shifted up.");
}

void MidiQTool::on_pushButtonDownOne_clicked()
{
     if(midModifier->adjustNotePitch(-1))
         ui->statusBar->showMessage("Out of range.");
     else
         ui->statusBar->showMessage("Notes shifted down.");

}
//-------End note Shifting
/*!
 * \brief MidiQTool::EnableGUI
 */
void MidiQTool::EnableGUI()
{
    for(QWidget *items: ui->centralWidget->findChildren<QWidget*>())
        items->setEnabled(true);
    if(midModifier->mid.getNumTracks() != 1)//If not type 0
        ui->pushButtonT0Split->setEnabled(false);
}


//--SETUP METHODS
/*!
 * \brief MidiQTool::SetupValidators
 */
void MidiQTool::SetupValidators()
{
    //Create
    validatorMultiplier =  new QDoubleValidator(0, 100, 2, this);
    validatorTempo = new QDoubleValidator(0, 512.0, 2, this); //No reason max
    validatorIntHalfByte =  new QIntValidator(1, 127,this);  //For note attacks
    validatorInt = new QIntValidator(0,INT_MAX,this);        //For cut
    validatorTime = new QDoubleValidator(0,INT_MAX, 2,this); //For cut, dont wanna go higher than a int can handle

    //set up format
    validatorMultiplier->setNotation(QDoubleValidator::StandardNotation);
    validatorTempo->setNotation(QDoubleValidator::StandardNotation);
    validatorTime->setNotation(QDoubleValidator::StandardNotation);

    //Setup
    //Note / length
    ui->lineEditNTMValue->setValidator(validatorMultiplier);
    // Volume options
    ui->lineEditVolume->setValidator(validatorIntHalfByte);
    ui->lineEditNoteAttacks->setValidator(validatorIntHalfByte);
    //Cut options
    ui->lineEditCutStart->setValidator(validatorInt);
    ui->lineEditCutFin->setValidator(validatorInt);
}
//--------End setup methods

void MidiQTool::on_pushButtonT0Split_clicked()
{
    midModifier->mid.splitTracksByChannelT0();
    cout << midModifier->mid.getNumTracks() << endl;
    ui->pushButtonT0Split->setEnabled(false);
    ui->statusBar->showMessage("Midi channels split into tracks.");
}
/*!
 * \brief MidiQTool::on_pushButtonCut_clicked
 */
void MidiQTool::on_pushButtonCut_clicked()
{
    QString cutSStart = ui->lineEditCutStart->text();
    QString cutSEnd = ui->lineEditCutFin->text();
    int cutStart =0;
    int cutEnd =0;
    if(ui->radioButtonCutTicks->isChecked()){
        cutStart = cutSStart.toInt();
        cutEnd = cutSEnd.toInt();
    }else{
       cutStart = midModifier->mid.getAbsoluteTickTime(cutSStart.toDouble());
       cutEnd = midModifier->mid.getAbsoluteTickTime(cutSEnd.toDouble());
    }

    if(cutEnd > cutStart || (cutStart && cutEnd == 0)){
        midModifier->cutMidi(cutStart,cutEnd);
        ui->statusBar->showMessage("Midi has been cut");
    }else{
       ui->statusBar->showMessage("Please enter correct times");
    }

}

void MidiQTool::on_radioButtonCutTicks_toggled(bool checked)
{
    if(checked){
        ui->lineEditCutStart->setValidator(validatorInt);
        ui->lineEditCutFin->setValidator(validatorInt);
    }else{
        ui->lineEditCutStart->setValidator(validatorTime);
        ui->lineEditCutFin->setValidator(validatorTime);
    }
    ui->lineEditCutStart->setText("");
    ui->lineEditCutFin->setText("");
}


/*!
 * \brief MidiQTool::on_pushButtonVolumeChan_clicked
 */
void MidiQTool::on_pushButtonVolumeChan_clicked()
{
    int value = ui->lineEditVolume->text().toInt();
    if(value){
        midModifier->removeAdditionalVolume(value);
        ui->statusBar->showMessage("Additional volumes removed");
    }else{
        ui->statusBar->showMessage("Input a value between 1-127");
    }
}

void MidiQTool::on_pushButtonNoteAttacks_clicked()
{
    int value = ui->lineEditNoteAttacks->text().toInt();
    if(value){
        midModifier->setNoteAttackVolume(value);
        ui->statusBar->showMessage("NoteAttacks set");
    }else{
        ui->statusBar->showMessage("Input a value between 1-127");
    }

}

void MidiQTool::on_pushButtonShortNotes_clicked()
{
   midModifier->removeShortNotes(5);
   ui->statusBar->showMessage("Short notes removed");
}

