#include "DSSVideoFrame.hpp"

DSSVideoFrame::DSSVideoFrame()
	: wxFrame(nullptr, wxID_ANY, "How-to Video", wxPoint(wxDefaultPosition),
	  wxSize(wxDefaultSize), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX | wxMINIMIZE_BOX))
{
	wxPanel* panel = new wxPanel(this);
	
	wxMediaCtrl* video = new wxMediaCtrl(panel, wxID_ANY);
	video->Load("./Media/DSS Instructions.mp4");
	video->SetSize(800, 600);
	video->ShowPlayerControls();
}