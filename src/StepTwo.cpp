#include "StepTwo.hpp"

StepTwo::StepTwo(wxWizard* parent)
    : wxWizardPageSimple(parent)
{
	wxPanel* panel = new wxPanel(this);
	wxBoxSizer* fitToWindow = new wxBoxSizer(wxVERTICAL);
	fitToWindow->Add(panel, 1, wxEXPAND);
	SetSizerAndFit(fitToWindow);

	wxStaticText* instructions = new wxStaticText(panel, wxID_ANY,
		"The CSV file has been updated, please review the 'Animal Welfare' calls in\n"
		"New World and change the incident type to 'Animal Cruelty' as needed.\n"
		"The buttons below will show a how to video, or open the csv file for you.\n",
		wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);
	wxFont instructionsFont = instructions->GetFont();
	instructionsFont.SetPointSize(16);
	instructions->SetFont(instructionsFont);

	wxStaticText* notice = new wxStaticText(panel, wxID_ANY,
		"Due to constraints in the ArcGIS API, a separate console window must be opened to run geoprocessing tasks.\n"
		"Please allow these tasks to finish, the console window will provide you with status updates in text format,\n"
		"and the window will close automatically when the geoprocessing tasks complete.",
		wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);
	wxFont noticeFont = notice->GetFont();
	noticeFont.SetPointSize(12);
	notice->SetFont(noticeFont);

	wxStaticText* whenDone = new wxStaticText(panel, wxID_ANY,
		"Click 'Next >' to upload the data to the web map.",
		wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);
	wxFont whenDoneFont = whenDone->GetFont();
	whenDoneFont.SetPointSize(10);
	whenDone->SetFont(whenDoneFont);

	// Buttons.
	// Creation order determines the 'TAB' traversal order
	// wanted csv by default so it's created first.
	wxBitmapButton* openExcelButton = new wxBitmapButton(panel, wxID_ANY,
		wxBitmap("./Media/icon_csv.bmp", wxBITMAP_TYPE_BMP),
		wxDefaultPosition, wxSize(70, 70));
	openExcelButton->Bind(wxEVT_BUTTON,
		[this](wxCommandEvent& event) { openCSV(event); });

	wxBitmapButton* showMeHowButton = new wxBitmapButton(panel, wxID_ANY,
		wxBitmap("./Media/icon_movie.bmp", wxBITMAP_TYPE_BMP),
		wxDefaultPosition, wxSize(70, 70));
	showMeHowButton->Bind(wxEVT_BUTTON,
		[this](wxCommandEvent& event) { showVideo(event); });

	wxBoxSizer* stackSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	buttonSizer->Add(showMeHowButton, 0, wxLEFT);
	buttonSizer->Add(15, 0);
	buttonSizer->Add(openExcelButton, 0, wxRIGHT);

	stackSizer->Add(instructions, wxSizerFlags().Center());
	stackSizer->Add(buttonSizer, wxSizerFlags().Center());
	stackSizer->Add(0, 15);
	stackSizer->Add(notice, wxSizerFlags().Center());
	stackSizer->Add(0, 10);
	stackSizer->Add(whenDone, wxSizerFlags().Right().Border(wxRIGHT, 35));
	sizer->Add(stackSizer, 1, wxEXPAND);
	panel->SetSizerAndFit(sizer);
}

void StepTwo::openCSV(wxCommandEvent& evt) const
{
	const std::string path{std::filesystem::absolute("../Data/Crime Data.csv").string()};
	
	// Convert path to wide string for SHELLEXECUTE.
	std::wstring csvPath(path.length(), L' ');
	std::copy(path.begin(), path.end(), csvPath.begin());

	[[maybe_unused]] HRESULT _unused_{ CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED) };
	SHELLEXECUTEINFOW execute = { sizeof execute };
	execute.lpVerb = L"open";
	execute.lpFile = csvPath.c_str();
	ShellExecuteExW(&execute);
}

void StepTwo::showVideo(wxCommandEvent& evt) const
{
	CSVVideoFrame* videoFrame = new CSVVideoFrame();
	videoFrame->SetClientSize(800, 600);
	videoFrame->Show();
}