#pragma once

#include "DSSVideoFrame.hpp"
#include "DataParser.hpp"

#include <wx/wx.h>
#include <wx/wizard.h>

#include <filesystem>
#include <string>

class StepOne : public wxWizardPageSimple
{
public:
    StepOne(wxWizard* parent);

private:
    void openSpreadsheet(wxCommandEvent& evt) const;
    void showVideo(wxCommandEvent& evt) const;
};
