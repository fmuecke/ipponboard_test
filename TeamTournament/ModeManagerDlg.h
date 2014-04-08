///////////////////////////////////////////////////////////////////////////////
// $Id$
///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2010-2014 Florian Muecke - (ipponboardinfo at googlemail dot com)
// All rights reserved.  Web: http://www.ipponboard.info
//
// This file is part of the Ipponboard project.
//
// It may not be distributed to or shared with the public in any form!
///////////////////////////////////////////////////////////////////////////////


#ifndef MODEMANAGERDLG_H
#define MODEMANAGERDLG_H

#include "../core/TournamentMode.h"
#include "../util/DialogResult.h"
#include <QDialog>
#include <memory>

class QStringList;

namespace Ui {
class ModeManagerDlg;
}

class ModeManagerDlg : public QDialog, public FMU::DialogResult<Ipponboard::TournamentMode::List>
{
    Q_OBJECT

public:
    explicit ModeManagerDlg(
            Ipponboard::TournamentMode::List const& modes,
            QStringList const& templates,
            QWidget *parent = 0);
    ~ModeManagerDlg();

private slots:
        // comboBoxes
        void on_comboBox_mode_currentIndexChanged(QString const& s);
        void on_comboBox_template_currentIndexChanged(QString const& s);
        // checkBoxes
        void on_checkBox_timeOverrides_toggled(bool checked);
        void on_checkBox_doubleWeights_toggled(bool checked);
        void on_checkBox_2013Rules_toggled(bool checked);
        void on_checkBox_autoIncrement_toggled(bool checked);
        void on_checkBox_allSubscoresCount_toggled(bool checked);
        // buttons
        void on_toolButton_add_clicked();
        void on_toolButton_remove_clicked();
        // spin controls
        void on_spinBox_rounds_valueChanged(int i);
        void on_spinBox_fightTime_valueChanged(int i);
        // line edits
        void on_lineEdit_weights_textChanged(QString const& s);
        void on_lineEdit_title_textChanged(QString const& s);
        void on_lineEdit_subtitle_textChanged(QString const& s);
        void on_lineEdit_timeOverrides_textChanged(QString const& s);

private:
    Ipponboard::TournamentMode& GetMode(QString const& s);

    std::shared_ptr<Ui::ModeManagerDlg> m_pUi;  //TODO: use unique_ptr
    Ipponboard::TournamentMode::List m_modes;
};

#endif // MODEMANAGERDLG_H