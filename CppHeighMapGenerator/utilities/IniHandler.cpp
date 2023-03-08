#include "IniHandler.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "Configuration.hpp"
#include "StringOperations.hpp"

IniHandler::IniHandler(std::filesystem::path iniFilepath)
{
	if (iniFilepath.has_root_directory()) {
		this->iniFilepath = iniFilepath;
	}
	else {
		this->iniFilepath = std::filesystem::current_path().append(iniFilepath.string());
	}

	properties = readPropertyFile(this->iniFilepath);
}

std::string IniHandler::GetPropertyValue(const std::string& key)
{
	return GetPropertyValue(properties, key);
}

std::string IniHandler::GetPropertyValue(PropertyMap& propMap, const std::string& key)
{
	const Property* subprop = GetProperty(propMap, key);
	if (subprop != nullptr)
		return subprop->value;
	return std::string();
}

std::string IniHandler::GetPropertyValue(UnorderedPropertyMap& propMap, const std::string& key)
{
	const Property* subprop = GetProperty(propMap, key);
	if (subprop != nullptr)
		return subprop->value;
	return std::string();
}

IniHandler::Property* IniHandler::GetProperty(const std::string& key)
{
	return GetProperty(properties, key);
}

IniHandler::Property* IniHandler::GetProperty(PropertyMap& propMap, const std::string& key)
{
	//if current node has the key then return its value
	//else check subnodes
	for (auto& prop : propMap) {
		if (prop.first == key)
			return &prop.second;

		Property* subprop = GetProperty(prop.second.subproperties, key);
		if (subprop != nullptr)
			return subprop;
	}
	return nullptr;
}

IniHandler::Property* IniHandler::GetProperty(UnorderedPropertyMap& propMap, const std::string& key)
{
	//if current node has the key then return its value
	//else check subnodes
	for (auto& prop : propMap) {
		if (prop.first == key)
			return &prop.second;

		Property* subprop = GetProperty(prop.second.subproperties, key);
		if (subprop != nullptr)
			return subprop;
	}
	return nullptr;
}

IniHandler::PropertyMap IniHandler::readPropertyFile(std::filesystem::path filepath)
{
	if (!std::filesystem::exists(filepath)) {
		if (filepath.has_filename() && filepath.has_extension()) {
			std::ofstream file(filepath);
			file.close();
		}
		else {
			return IniHandler::PropertyMap();
		}
	}
	if (!std::filesystem::is_regular_file(filepath)) {
		return IniHandler::PropertyMap();
	}
	std::ifstream iniFile(filepath);
	PropertyMap propMap;

	if (!iniFile.good()) {
		return propMap;
	}

	std::string line;
	Property* lastAddedProp = nullptr;
	std::string currentCategoryKey = "";

	while (std::getline(iniFile, line, '\n')) {
		//Add subtree if line is category specifier
		if (line.find('[') != std::string::npos
			&& line.find(']') != std::string::npos)
		{
			auto RemoveSqBraces = [](std::string& s) {
				size_t start = s.find('[');
				size_t end = s.rfind(']');
				s = s.substr(start + 1, end - 1);
				dh::StringOps::trim(s);
			};
			RemoveSqBraces(line);

			//if category had double ([[ || ]]) square brackets, add it as subcategory to current subcategory
			if (line.find('[') != std::string::npos
				&& line.find(']') != std::string::npos)
			{
				RemoveSqBraces(line);

				lastAddedProp = this->GetProperty(propMap, currentCategoryKey);

				if (lastAddedProp != nullptr) {
					Property prop;
					prop.value = line;
					lastAddedProp->subproperties.insert(std::make_pair(line, prop));
					lastAddedProp = &lastAddedProp->subproperties.at(line);
				}
			}
			else {	//add a category
				Property prop;
				prop.value = line;
				propMap.insert(std::make_pair(line, prop));
				lastAddedProp = &propMap.at(line);
				currentCategoryKey = line;
			}
		}
		else if (line.find('=') != std::string::npos) {
			//if its an attribute add it to the last property added
			if (lastAddedProp != nullptr) {
				std::stringstream sstream(line);
				std::string key = "";

				std::getline(sstream, key, '=');
				dh::StringOps::trim(key);

				Property prop;
				std::getline(sstream, prop.value, '=');
				dh::StringOps::trim(prop.value);

				lastAddedProp->subproperties.insert(std::make_pair(key, prop));
			}
		}
	}

	iniFile.close();

	return propMap;
}

void IniHandler::writePropertyFile(std::filesystem::path filepath)
{
	if (!std::filesystem::exists(filepath)) {
		return;
	}
	if (!std::filesystem::is_regular_file(filepath)) {
		return;
	}
	std::ofstream iniFile;

	if (iniFile.is_open()) {
		std::string line = "";
	}
}