#pragma once

#include <OpenXLSX.hpp>

#include <wx/msgdlg.h>
#include <wx/wx.h>

#include <array>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

class DataParser
{
public:
	DataParser(const std::string filePath);
	
	void generateCSV() const;

private:
	struct Cells // Spreadsheet data cells.
	{
		Cells(const std::string row = "")
		  : latitude{ "A" + row },
			longitude{ "B" + row },
			date{ "C" + row },
			incidentNumber{ "D" + row },
			incidentType{ "E" + row }
		{}

		const std::string latitude;
		const std::string longitude;
		const std::string date;
		const std::string incidentNumber;
		const std::string incidentType;
	};

	struct CrimeData // Spreadsheet data.
	{
		const std::string latitude;
		const std::string longitude;
		const std::string date;
		const std::string incidentNumber;
		const std::string incidentType;
		const std::string category;
	};

	const std::array<std::string, 6> m_csvFields
	{
		"Latitude,",
		"Longitude,",
		"Date,",
		"IncidentNumber,",
		"IncidentType,",
		"Category\n"
	};

	const std::string m_csvOutputDirectory{ "../Data" };
	const std::string m_csvPath{ "../Data/Crime Data.csv" };
	const uint32_t m_firstDataRow{ 7 }; // Row 7 is where the actual data begins.
	const std::string m_pathToSpreadsheet;

	const std::vector<std::string> m_sheetNames
	{
		// "Animal Incidents" at the top so they're easier to review.
		"Animal Incidents",
		"Crimes Against Persons",
		"Crimes Against Property",
		"Crimes Against Society",
		"Other Calls for Service",
		"Traffic Incidents",
		"Miscellaneous Investigations"
	};

	void createDirectory() const;
	uint32_t findEmptyRow(const OpenXLSX::XLWorksheet& sheet) const;
	std::vector<OpenXLSX::XLWorksheet> getWorksheets(const OpenXLSX::XLDocument& spreadsheet) const;
	DataParser::CrimeData readCells(const OpenXLSX::XLWorksheet& sheet, const uint32_t) const;
	std::vector<DataParser::CrimeData> spreadsheetData() const;
};