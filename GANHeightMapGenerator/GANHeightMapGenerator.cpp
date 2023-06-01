#include "GANHeightMapGenerator.hpp"
#include <iostream>
#include "TrainingHandler.hpp"
#include <filesystem>

int main() {
	std::cout << "GANHeightMapGenerator" << std::endl;
	std::cout << "Current dir: " << std::filesystem::current_path().string() << std::endl;

	//Create config file if it does not exist
	std::filesystem::path datasetPath("./assets/dataset");
	std::cout << "Assets directory: " << std::filesystem::absolute(datasetPath.string()) << std::endl;

	if (!std::filesystem::exists(datasetPath)) {
		std::cout << "Directory " << std::filesystem::absolute(datasetPath.string()) << " doesn't exist!" << std::endl;
		std::filesystem::create_directories(datasetPath);
	}
	//PytorchHandler::LabelsCsv labelsCsv;

	//for (int i = 1; i <= 5000; ++i) {
	//	std::ostringstream fileStr;
	//	if (i < 10) fileStr << "000";
	//	else if (i < 100) fileStr << "00";
	//	else if (i < 1000) fileStr << "0";
	//	fileStr << i << "_h.png";

	//	std::string filepath = datasetPath.string() + "/" + fileStr.str();
	//	cv::Mat img = cv::imread(filepath); //WHY THIS DOESNT LOAD IMAGES!!!!!!
	//	if (img.empty()) continue;

	//	labelsCsv.push_back(std::make_tuple(filepath, 0));
	//}

	//PytorchHandler pyh;
	//pyh.CreateModel(labelsCsv);

	TrainingHandler trainingHandler;
	trainingHandler.TrainModel();

	return 0;
}