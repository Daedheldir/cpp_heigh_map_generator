#include "utilities/CsvHandler.hpp"
#include <filesystem>
#include <fstream>

CsvHandler::CsvHandler()
{
}

void CsvHandler::AddRowToCsv(const std::vector<std::string>& row)
{
	csvFileArr.push_back(row);
}

void CsvHandler::WriteCsv(const std::string& filepath)
{
	std::filesystem::path csvPath{ filepath };

	if (csvPath.has_root_directory()) {
		if (!std::filesystem::exists(csvPath.root_directory())) {
			std::filesystem::create_directory(csvPath.root_directory());
		}
	}

	if (!csvPath.has_filename())
		return;

	if (!csvPath.has_extension()) {
		csvPath += ".csv";
	}

	std::ofstream csvFile(csvPath, std::ios::trunc);

	if (!csvFile.good())
		return;

	for (const auto& row : csvFileArr) {
		for (const auto& item : row) {
			csvFile << item << ',';
		}
		csvFile << std::endl;
	}
}
