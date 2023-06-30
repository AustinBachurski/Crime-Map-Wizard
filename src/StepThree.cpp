#include "StepThree.hpp"

StepThree::StepThree(wxWizard* parent)
    : wxWizardPageSimple(parent)
{
	wxPanel* panel = new wxPanel(this);
	wxBoxSizer* fitToWindow = new wxBoxSizer(wxVERTICAL);
	fitToWindow->Add(panel, 1, wxEXPAND);
	SetSizerAndFit(fitToWindow);

	wxStaticText* done = new wxStaticText(panel, wxID_ANY, 
		"Assuming you received \"Success!\" from\nupdloading the data, you're all done!",
		wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);
	wxFont doneFont = done->GetFont();
	doneFont.SetPointSize(20);
	done->SetFont(doneFont);

	wxStaticText* assuming = new wxStaticText(panel, wxID_ANY,
		"If you received an error, please let Austin Bachurski know by clicking the\n"
		"'Help' button below so that the error can be corrected.",
		wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);
	wxFont assumingFont = assuming->GetFont();
	assumingFont.SetPointSize(10);
	assuming->SetFont(assumingFont);

	wxStaticText* information = new wxStaticText(panel, wxID_ANY, (
		"If you want to check your work you can click the button below\n"
		"to open a web browser to the Kalispell Crime Map."),
		wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);
	wxFont informationFont = information->GetFont();
	informationFont.SetPointSize(16);
	information->SetFont(informationFont);

	wxStaticText* whenDone = new wxStaticText(panel, wxID_ANY, (
		"Click 'Finish' to exit the wizard."),
		wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);
	wxFont whenDoneFont = whenDone->GetFont();
	whenDoneFont.SetPointSize(10);
	whenDone->SetFont(whenDoneFont);

	wxBitmapButton* openWebPageButton = new wxBitmapButton(panel, wxID_ANY,
		wxBitmap("./Media/icon_edge.bmp", wxBITMAP_TYPE_BMP),
		wxDefaultPosition, wxSize(70, 70));
	openWebPageButton->Bind(wxEVT_BUTTON,
		[this](wxCommandEvent& event) { displayWebPage(event); });

	wxBoxSizer* stackSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	buttonSizer->Add(openWebPageButton, wxSizerFlags().Center());

	stackSizer->Add(done, wxSizerFlags().Center());
	stackSizer->Add(0, 10);
	stackSizer->Add(assuming, wxSizerFlags().Center());
	stackSizer->Add(0, 10);
	stackSizer->Add(information, wxSizerFlags().Center());
	stackSizer->Add(0, 15);
	stackSizer->Add(buttonSizer, wxSizerFlags().Center());
	stackSizer->Add(0, 3);
	stackSizer->Add(whenDone, wxSizerFlags().Right().Border(wxRIGHT, 35));
	sizer->Add(stackSizer, 1, wxEXPAND);
	panel->SetSizerAndFit(sizer);
}

std::string StepThree::readURL() const
{
	const std::filesystem::path urlFilePath{ std::filesystem::absolute("URL/URL.ini") };

	if (std::filesystem::exists(urlFilePath))
	{
		std::string url;

		std::ifstream urlFile(urlFilePath, std::ios::in); // Open file for reading.
		if (urlFile.is_open())
		{
			std::getline(urlFile, url);
			return url;
		}
	}
	return {};
}

void StepThree::displayWebPage(wxCommandEvent& evt) const
{
	const std::string url{ readURL() };

	if (!url.empty())
	{
		// Convert path to wide string for SHELLEXECUTE.
		std::wstring mapURL(url.length(), L' ');
		std::copy(url.begin(), url.end(), mapURL.begin());

		[[maybe_unused]] HRESULT _unused_{ CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED) };
		SHELLEXECUTEINFOW execute = { sizeof execute };
		execute.lpVerb = L"open";
		execute.lpFile = mapURL.c_str();
		ShellExecuteExW(&execute);
	}
	else
	{
		wxMessageDialog* noURL = new wxMessageDialog(NULL,
			"No URL read from url.ini file.\n\n"
			"Please inform Austin Bachurski of the error, his contact info\n"
			"can be found by clicking the 'Help' button below.",
			"URL Not Found",
			wxOK | wxICON_INFORMATION);
		noURL->ShowModal();
	}
}