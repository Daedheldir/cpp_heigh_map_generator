#pragma once
#include "torch/torch.h"

using LabelsCsv = std::vector<std::tuple<std::string /*file location*/, int64_t /*label*/>>;
struct Dataset : torch::data::Dataset<Dataset> {
	const int64_t kImageSize = 512;
	LabelsCsv csv_;

	Dataset(LabelsCsv& file_names_csv, const int64_t kImageSize) :
		kImageSize{ kImageSize },
		csv_{ file_names_csv } {
	};

	// Override the get method to load custom data.
	torch::data::Example<> get(size_t index) override {
		std::string file_location = std::get<0>(csv_[index]);
		int64_t label = std::get<1>(csv_[index]);

		// Load image with OpenCV.
		cv::Mat img = cv::imread(file_location);

		// Resize to 64*64
		cv::resize(img, img, cv::Size(kImageSize, kImageSize));

		// Convert the image and label to a tensor.
		// Here we need to clone the data
		torch::Tensor img_tensor = torch::from_blob(img.data, { img.rows, img.cols, 3 }, torch::kByte).clone();
		img_tensor = img_tensor.permute({ 2, 0, 1 }); // convert to CxHxW
		torch::Tensor label_tensor = torch::full({ 1 }, label);

		return { img_tensor, label_tensor };
	};

	// Override the size method to infer the size of the data set.
	torch::optional<size_t> size() const override {
		return csv_.size();
	};
};
