#include "StepOne.hpp"

StepOne::StepOne(wxWizard* parent)
	: wxWizardPageSimple(parent)
{
	wxPanel* panel = new wxPanel(this);
	wxBoxSizer* fitToWindow = new wxBoxSizer(wxVERTICAL);
	fitToWindow->Add(panel, 1, wxEXPAND);
	SetSizerAndFit(fitToWindow);

	// Window text.
	wxStaticText* welcome = new wxStaticText(
		panel, wxID_ANY, ("Welcome to the Crime Map Update Wizard"),
		wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);
	wxFont welcomeFont = welcome->GetFont();
	welcomeFont.SetPointSize(22);
	welcome->SetFont(welcomeFont);

	wxStaticText* displayWeekNumber = new wxStaticText(
		panel, wxID_ANY,
		std::format("It is currently ISO week number: {:%V}",
			std::chrono::system_clock::now()), // Current week number in ISO format.
		wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);
	wxFont weekNumberFont = displayWeekNumber->GetFont();
	weekNumberFont.SetPointSize(20);
	displayWeekNumber->SetFont(weekNumberFont);

	wxStaticText* selectPrompt = new wxStaticText(panel, wxID_ANY,
		"Please open the 'DSS Data.xlsx' spreadsheet and refresh the data.\n"
		"Then, select the appropriate week, and save the spreadsheet.\n"
		"The buttons below will show a how to video, or open the spreadsheet for you.\n",
		wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);
	wxFont selectPromptFont = selectPrompt->GetFont();
	selectPromptFont.SetPointSize(14);
	selectPrompt->SetFont(selectPromptFont);

	wxStaticText* whenDone = new wxStaticText(panel, wxID_ANY,
		"Click 'Next >' to update the .csv file.",
		wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);
	wxFont whenDoneFont = whenDone->GetFont();
	whenDoneFont.SetPointSize(10);
	whenDone->SetFont(whenDoneFont);

	// Buttons.
	// Creation order determines the 'TAB' traversal order
	// wanted excel by default so it's created first.
	wxBitmapButton* openExcelButton = new wxBitmapButton(panel,	wxID_ANY,
		wxBitmap("./Media/icon_excel.bmp", wxBITMAP_TYPE_BMP),
		wxDefaultPosition, wxSize(70, 70));
	openExcelButton->Bind(wxEVT_BUTTON,
		[this](wxCommandEvent& event) { openSpreadsheet(event); });

	wxBitmapButton* showMeHowButton = new wxBitmapButton(panel, wxID_ANY,
		wxBitmap("./Media/icon_movie.bmp", wxBITMAP_TYPE_BMP),
		wxDefaultPosition, wxSize(70, 70));
	showMeHowButton->Bind(wxEVT_BUTTON,
		[this](wxCommandEvent& event) { showVideo(event); });

	// Sizers.
	wxBoxSizer* stackSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	buttonSizer->Add(showMeHowButton, 0, wxLEFT);
	buttonSizer->Add(25, 0);
	buttonSizer->Add(openExcelButton, 0, wxRIGHT);

	stackSizer->Add(welcome, wxSizerFlags().Center());
	stackSizer->Add(0, 5);
	stackSizer->Add(displayWeekNumber, wxSizerFlags().Center());
	stackSizer->Add(0, 15);
	stackSizer->Add(selectPrompt, wxSizerFlags().Center());
	stackSizer->Add(buttonSizer, wxSizerFlags().Center());
	stackSizer->Add(0, 10);
	stackSizer->Add(whenDone, wxSizerFlags().Right().Border(wxRIGHT, 35));
	sizer->Add(stackSizer, 1, wxEXPAND);
	panel->SetSizerAndFit(sizer);
}

void StepOne::openSpreadsheet(wxCommandEvent& evt) const
{
	const std::string path{std::filesystem::absolute("../Data/DSS Data.xlsx").string()};

	// Convert path to wide string for SHELLEXECUTE.
	std::wstring spreadsheetPath(path.length(), L' ');
	std::copy(path.begin(), path.end(), spreadsheetPath.begin());

	[[maybe_unused]] HRESULT _unused_{ CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED) };
	SHELLEXECUTEINFOW execute = { sizeof execute };
	execute.lpVerb = L"open";
	execute.lpFile = spreadsheetPath.c_str();
	ShellExecuteExW(&execute);
}

void StepOne::showVideo(wxCommandEvent& evt) const
{
	DSSVideoFrame* videoFrame = new DSSVideoFrame();
	videoFrame->SetClientSize(800, 600);
	videoFrame->Show();
}