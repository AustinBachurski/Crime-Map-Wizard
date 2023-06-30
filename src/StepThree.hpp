#pragma once

#include <filesystem>
#include <fstream>
#include <string>
#include <wx/wx.h>
#include <wx/wizard.h>


class StepThree : public wxWizardPageSimple
{
public:
    StepThree(wxWizard* parent);

private:
    void displayWebPage(wxCommandEvent& evt) const;
    std::string readURL() const;
};