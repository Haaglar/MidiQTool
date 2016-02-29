#include "midiqtool.h"
#include "ui_midiqtool.h"
#include "adjustmid.h"
#include "constants.h"
#include <qfiledialog.h>
MidiQTool::MidiQTool(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MidiQTool)
{
    ui->setupUi(this);
    ui->lineEditNTMValue->setValidator( new QDoubleValidator(0, 100, 2, this));
    midModifier = new AdjustMid();
}

MidiQTool::~MidiQTool()
{
    delete ui;
    delete midModifier;
}

//Open file
void MidiQTool::on_pushButtonOpenMidi_clicked()
{
    midiDir = QFileDialog::getOpenFileName(this,tr("Open Midi"),"","Midi file (*.mid *.midi)");
    if(midiDir == NULL)
        return;
    string sirLoc = midiDir.toStdString();
    midModifier->SetMidi(sirLoc);
    //I hate this style option
    if(!midi.status()){
        ui->statusBar->showMessage("Failed to read midi file.");
    }else{
        midi.linkNotePairs(); //So we can easily modify note onoff combos
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
    ui->lineEditSaveLoc->setText(outDir);
}
//Save file to location
void MidiQTool::on_pushButtonSave_clicked()
{
    string outLoc = outDir.toStdString();
    midi.write(outLoc);
}

//-----End file options


void MidiQTool::on_pushButtonTrimRest_clicked()
{
    midModifier->TrimStart();
}

void MidiQTool::on_pushButtonTNUdjust_clicked()
{

    QString value = ui->lineEditNTMValue->text();
    float adjustment = value.toFloat();
    if(adjustment !=0)
        midModifier->AdjustTempoAndNotes(adjustment);
    else
        ui->statusBar->showMessage("Use a non 0 value");
}


//----------Note shifiting
void MidiQTool::on_pushButtonOcUp_clicked()
{
    if(midModifier->AdjustNotePitch(16))
        ui->statusBar->showMessage("Out of range.");
    else
        ui->statusBar->showMessage("Octave shifted up.");
}

void MidiQTool::on_pushButtonOctDown_clicked()
{
    if(midModifier->AdjustNotePitch(-16))
        ui->statusBar->showMessage("Out of range.");
    else
        ui->statusBar->showMessage("Octave shifted down.");
}

void MidiQTool::on_pushButtonUpOne_clicked()
{
    if(midModifier->AdjustNotePitch(1))
        ui->statusBar->showMessage("Out of range.");
    else
        ui->statusBar->showMessage("Notes shifted up.");
}

void MidiQTool::on_pushButtonDownOne_clicked()
{
     if(midModifier->AdjustNotePitch(-1))
         ui->statusBar->showMessage("Out of range.");
     else
         ui->statusBar->showMessage("Notes shifted down.");

}
//-------End note Shifting
