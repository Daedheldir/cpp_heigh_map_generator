#pragma once
#include <vector>
#include <string>
class CsvHandler {
public:
	CsvHandler();

	void AddRowToCsv(const std::vector<std::string> & row );

	void WriteCsv(const std::string& filepath);

private:
	std::vector<std::vector<std::string>> csvFileArr;
};