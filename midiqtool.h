#ifndef MIDIQTOOL_H
#define MIDIQTOOL_H

#include <QMainWindow>
#include "midifile/MidiFile.h"
#include "adjustmid.h"
#include <QDoubleValidator>
#include <qfiledialog.h>
namespace Ui {
class MidiQTool;
}

class MidiQTool : public QMainWindow
{
    Q_OBJECT

public:
    explicit MidiQTool(QWidget *parent = 0);
    ~MidiQTool();

private slots:

    void on_pushButtonTrimRest_clicked();

    void on_pushButtonTNUdjust_clicked();

    void on_pushButtonOcUp_clicked();

    void on_pushButtonOctDown_clicked();

    void on_pushButtonUpOne_clicked();

    void on_pushButtonDownOne_clicked();

    void on_radioButtonMulti_toggled(bool checked);

    void on_pushButtonT0Split_clicked();

    void on_pushButtonCut_clicked();

    void on_pushButtonVolumeChan_clicked();

    void on_pushButtonNoteAttacks_clicked();

    void on_pushButtonShortNotes_clicked();

    void on_radioButtonCutTicks_toggled(bool checked);

    void on_pushButtonRepeat_clicked();

private:

    void EnableGUI();
    void SetupValidators();
    void SavedClicked();
    void SaveMidi();
    void OpenMidi();
    Ui::MidiQTool *ui;
    QString midiDir;
    QString outDir;
    AdjustMid *midModifier;
    QDoubleValidator *validatorMultiplier;
    QDoubleValidator *validatorTempo;
    QIntValidator *validatorIntHalfByte;
    QIntValidator *validatorInt;
    QDoubleValidator * validatorTime;
};

#endif // MIDIQTOOL_H
