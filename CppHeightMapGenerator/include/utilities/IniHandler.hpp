#pragma once
#include <filesystem>
#include <map>
#include <unordered_map>
class IniHandler {
public:

	struct Property {
		std::string value;
		std::unordered_map<std::string, Property> subproperties;
	};
	//map of <Category, properties>
	using PropertyMap = std::map<std::string, Property>;
	using UnorderedPropertyMap = std::unordered_map<std::string, Property>;

	IniHandler(std::filesystem::path iniFilepath);

	std::string GetPropertyValue(const std::string& key);
	std::string GetPropertyValue(PropertyMap& propMap, const std::string& key);
	std::string GetPropertyValue(UnorderedPropertyMap& propMap, const std::string& key);
	Property* GetProperty(const std::string& key);
	Property* GetProperty(PropertyMap& propMap, const std::string& key);
	Property* GetProperty(UnorderedPropertyMap& propMap, const std::string& key);
private:

	PropertyMap readPropertyFile(std::filesystem::path filepath);
	void writePropertyFile(std::filesystem::path filepath);
public:
	PropertyMap properties;
private:
	std::filesystem::path iniFilepath;
};