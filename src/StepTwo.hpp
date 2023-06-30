#pragma once

#include "CSVVideoFrame.hpp"

#include <wx/wx.h>
#include <wx/wizard.h>

#include <filesystem>
#include <string>


class StepTwo : public wxWizardPageSimple
{
public:
    StepTwo(wxWizard* parent);

private:
    void openCSV(wxCommandEvent& evt) const;
    void showVideo(wxCommandEvent& evt) const;
};