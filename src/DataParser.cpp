#include "DataParser.hpp"

DataParser::DataParser(const std::string pathToSpreadsheet)
	: m_pathToSpreadsheet{ pathToSpreadsheet }
{}

void DataParser::createDirectory() const
{
	try
	{
		std::filesystem::create_directory(m_csvOutputDirectory);
	}
	catch (std::filesystem::filesystem_error)
	{
		wxMessageDialog* directoryError = new wxMessageDialog(NULL,
			"Unable to create " + m_csvOutputDirectory + " directory!",
			"Directory Error", wxOK | wxICON_ERROR);
		directoryError->ShowModal();
	}
}

void DataParser::generateCSV() const
{
	if (!std::filesystem::exists(m_csvOutputDirectory))
	{
		createDirectory();
	}
	// Open File for Writing | Discard Existing Data
	std::ofstream csv{ m_csvPath, std::ios::out | std::ios::trunc };
	if (csv.is_open())
	{
		// Pass by value is intended, field strings are all < 16 characters.
		for (const std::string field : m_csvFields)
		{
			csv << field;
		}

		for (const CrimeData& data : spreadsheetData())
		{
			csv << data.latitude << ", "
				<< data.longitude << ", "
				<< data.date << ", "
				<< data.incidentNumber << ", "
				<< data.incidentType << ", "
				<< data.category << '\n';
		}
	}
	else
	{
		wxMessageDialog* fileError = new wxMessageDialog(NULL,
			"Unable to open " + m_csvPath + " file!",
			"File Error", wxOK | wxICON_ERROR);
		fileError->ShowModal();
	}
}

std::vector<OpenXLSX::XLWorksheet> DataParser::getWorksheets(const OpenXLSX::XLDocument& spreadsheet) const
{
	std::vector<OpenXLSX::XLWorksheet> worksheets;

	for (const std::string& sheetName : m_sheetNames)
	{
		worksheets.emplace_back(spreadsheet.workbook().worksheet(sheetName));
	}
	return worksheets;
}

uint32_t DataParser::findEmptyRow(const OpenXLSX::XLWorksheet& sheet) const
{
	Cells cell;
	uint32_t populatedRows{ 0 };

	// Entire range of a column, which column is 
	// irrelevant as long as said column contains data.
	OpenXLSX::XLCellRange range = sheet.range(
		OpenXLSX::XLCellReference(m_firstDataRow, cell.latitude),
		OpenXLSX::XLCellReference(OpenXLSX::MAX_ROWS, cell.latitude));

	for (const auto& cell : range)
	{
		if (cell.value().type() == OpenXLSX::XLValueType::Empty)
		{
			break;
		}
		++populatedRows;
	}
	return populatedRows + m_firstDataRow;
}

std::vector<DataParser::CrimeData> DataParser::spreadsheetData() const
{
	if (std::filesystem::exists(m_pathToSpreadsheet))
	{
		OpenXLSX::XLDocument spreadsheet{ m_pathToSpreadsheet };

		std::vector<OpenXLSX::XLWorksheet> worksheets(getWorksheets(spreadsheet));
		std::vector<DataParser::CrimeData> assembledData;

		if (spreadsheet.isOpen())
		{
			for (const OpenXLSX::XLWorksheet& sheet : worksheets)
			{
				const uint32_t emptyRow{ findEmptyRow(sheet) };

				for (uint32_t row = m_firstDataRow; row < emptyRow; ++row)
				{
					assembledData.emplace_back(readCells(sheet, row));
				}
			}
			return assembledData;
		}
		else
		{
			wxMessageDialog* error = new wxMessageDialog(NULL,
				"Spreadsheet was found at "
				+ m_pathToSpreadsheet + " but could not be opened",
				"Spreadsheet Error", wxICON_ERROR);
			error->ShowModal();
		}
	}
	else
	{
		wxMessageDialog* error = new wxMessageDialog(NULL,
			"Spreadsheet was not found!\n\n"
			"Expected in: " + m_pathToSpreadsheet,
			"Spreadsheet Error", wxICON_ERROR);
		error->ShowModal();
	}
	return {};
}

DataParser::CrimeData DataParser::readCells(const OpenXLSX::XLWorksheet& sheet, const uint32_t currentRow) const
{
	Cells cell(std::to_string(currentRow));

	CrimeData dataRow
		{
			sheet.cell(cell.latitude).value().get<std::string>(),
			sheet.cell(cell.longitude).value().get<std::string>(),
			sheet.cell(cell.date).value().get<std::string>(),
			sheet.cell(cell.incidentNumber).value().get<std::string>(),
			sheet.cell(cell.incidentType).value().get<std::string>(),
			sheet.name()
		};
	return dataRow;
}