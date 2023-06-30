#include "Application.hpp"

wxIMPLEMENT_APP(Launcher); // int main()

bool Launcher::OnInit()
{
	CrimeApp* appWindow = new CrimeApp();
    appWindow->Show();
    return true;
}

CrimeApp::CrimeApp()
    : wxFrame(nullptr, wxID_ANY, "", wxPoint(wxDefaultPosition),
        wxSize(wxDefaultSize), wxICONIZE | wxFRAME_NO_TASKBAR)
{
	CrimeWizard wizard(this);
    wizard.RunWizard(wizard.GetFirstPage());
	wizard.Destroy();
    Close();
}

CrimeWizard::CrimeWizard(wxFrame* parent)
{
	SetExtraStyle(wxWIZARD_EX_HELPBUTTON);
	SetPageSize(wxSize(800, 300));

	Create(parent, wxID_ANY, "Crime Map Update Wizard",
		wxNullBitmap, wxDefaultPosition);

	m_pageOne = new StepOne(this);
	m_pageTwo = new StepTwo(this);
	m_pageThree = new StepThree(this);

	m_pageOne->Chain(m_pageTwo).Chain(m_pageThree);
}

void CrimeWizard::help(wxWizardEvent& event)
{
	if (wxTheClipboard->Open())
	{
		wxTheClipboard->SetData(new wxTextDataObject("abachurski@kalispell.com"));
		wxTheClipboard->Close();
		wxLogStatus("'abachurski@kalispell.com' copied to clipboard.");
	}

	wxMessageDialog* showHelp = new wxMessageDialog(NULL,
		"If you need help, please contact Austin Bachurski at\n"
		"406-758-7943 or abachurski@kalispell.com\n"
		"The email address has been copied to your clipboard.",
		"Help!", wxOK | wxICON_QUESTION);
	showHelp->ShowModal();
}

int CrimeWizard::uploadGISdata()
{
	const std::string pythonPath{"python \""
		+ std::filesystem::absolute("Geoprocessing/upload.py").string()
		+ '"' };

	wxGenericProgressDialog progress(
		"Uploading Data to Web Map",
		"Performing Geoprocessing Tasks...",
		10,
		this,
		wxPD_APP_MODAL | wxPD_AUTO_HIDE | wxPD_ELAPSED_TIME);
	progress.Show();

	std::promise<int> promise;
	std::future<int> future = promise.get_future();

	std::thread geoprocessing{
		[this, promise = std::move(promise), &pythonPath]() mutable
		{
			callPython(std::move(promise), pythonPath);
		} };
	geoprocessing.detach();

	while (future.wait_for(std::chrono::milliseconds(50)) != std::future_status::ready)
	{
		wxYield();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		progress.Pulse();
	}
	int result{ future.get() };
	progress.Hide();

	return result;
}

void CrimeWizard::callPython(std::promise<int> promise, const std::string pythonPath) const
{
	int32_t uploadResult{ std::system((pythonPath).c_str()) };
	promise.set_value(uploadResult);
}

void CrimeWizard::nextClicked(wxWizardEvent& event)
{
	if (GetCurrentPage() == m_pageOne && event.GetDirection())
	{
		// Parses the DSS Data spreadsheet and generates the Crime Data csv.
		DataParser parser{ "../Data/DSS Data.xlsx" };
		parser.generateCSV();
	}
	else if (GetCurrentPage() == m_pageTwo && event.GetDirection())
	{
		// Calls 3 Python scripts to convert the table to GIS points,
		// delete the old data, and upload the new.
		int returnCode{ uploadGISdata() };
		wxString message{ "Geoprocessing Result:\n\n" + translate(returnCode)};

		wxMessageDialog* displayResults = new wxMessageDialog(NULL,
			message,
			"Geoprocessing Complete",
			wxOK | wxICON_INFORMATION);
		displayResults->ShowModal();
	}
}

std::string CrimeWizard::translate(int returnCode) const
{
	const std::string notify{"\n\nPlease contact Austin Bachurski via the 'help' button."};

	switch (returnCode)
	{
	case ReturnCode::Success:
		return "Success!\n\nClick 'OK' to continue.";
		break;
	case ReturnCode::SignInFailed:
		return "ERROR: Sign in to ArcGIS Online Failed!" + notify;
		break;
	case ReturnCode::CSVnotFound:
		return "ERROR: Crime Data.csv could not be located!" + notify;
		break;
	case ReturnCode::URLerrorDeleteFeatures:
		return "ERROR: URL to feature layer is invalid! (Delete Features)" + notify;
		break;
	case ReturnCode::URLerrorAppendFeatures:
		return "ERROR: URL to feature layer is invalid! (Append Features)" + notify;
		break;
	case ReturnCode::UserClosedWindow:
		return "ERROR: Geoprocessing window was closed prematurely!";
		break;
	default:
		return "ERROR: Unexpected return code - received: " + std::to_string(returnCode) + notify;
		break;
	}
}

wxBEGIN_EVENT_TABLE(CrimeWizard, wxWizard)
EVT_WIZARD_HELP(wxID_ANY, CrimeWizard::help)
EVT_WIZARD_BEFORE_PAGE_CHANGED(wxID_ANY, CrimeWizard::nextClicked)
wxEND_EVENT_TABLE()