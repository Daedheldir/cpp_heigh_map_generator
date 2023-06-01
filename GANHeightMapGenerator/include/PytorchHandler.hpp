#pragma once
#include <string>
#include <torch/torch.h>
#include <opencv2/opencv.hpp>

class PytorchHandler {
public:
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

	struct DCGANGeneratorImpl : torch::nn::Module {
		DCGANGeneratorImpl(int kLatentDim)
			: conv1(torch::nn::ConvTranspose2dOptions(kLatentDim, 512, 4)
				.bias(false)),
			batch_norm1(512),
			conv2(torch::nn::ConvTranspose2dOptions(512, 256, 4)
				.stride(2)
				.padding(1)
				.bias(false)),
			batch_norm2(256),
			conv3(torch::nn::ConvTranspose2dOptions(256, 128, 4)
				.stride(2)
				.padding(1)
				.bias(false)),
			batch_norm3(128),
			conv4(torch::nn::ConvTranspose2dOptions(128, 64, 4)
				.stride(2)
				.padding(1)
				.bias(false)),
			batch_norm4(64),
			conv5(torch::nn::ConvTranspose2dOptions(64, 3, 4)
				.stride(2)
				.padding(1)
				.bias(false))
		{
			register_module("conv1", conv1);
			register_module("conv2", conv2);
			register_module("conv3", conv3);
			register_module("conv4", conv4);
			register_module("conv5", conv5);
			register_module("batch_norm1", batch_norm1);
			register_module("batch_norm2", batch_norm2);
			register_module("batch_norm3", batch_norm3);
			register_module("batch_norm4", batch_norm4);
		}

		torch::Tensor forward(torch::Tensor x) {
			x = torch::relu(batch_norm1(conv1(x)));
			x = torch::relu(batch_norm2(conv2(x)));
			x = torch::relu(batch_norm3(conv3(x)));
			x = torch::relu(batch_norm4(conv4(x)));
			x = torch::tanh(conv5(x));
			return x;
		}

		torch::nn::ConvTranspose2d conv1, conv2, conv3, conv4, conv5;
		torch::nn::BatchNorm2d batch_norm1, batch_norm2, batch_norm3, batch_norm4;
	};

	TORCH_MODULE(DCGANGenerator);

	PytorchHandler();

	std::string test();

	LabelsCsv ReadCsv(const std::string& location);

	void CreateModel(LabelsCsv& labelsCsv);
	void TrainModel();
	void GenerateResults();


	// Reshaped Image Size
	const int64_t kImageSize = 64;

	// The size of the noise vector fed to the generator.
	const int64_t kLatentDim = 100;

	// The batch size for training.
	const int64_t kBatchSize = 100;

	// Number of workers
	const int64_t kNumOfWorkers = 8;

	// Enforce ordering
	const bool kEnforceOrder = false;

	// The number of epochs to train.
	const int64_t kNumberOfEpochs = 30;

	// Where to find the CSV with file locations.
	const std::string kCsvFile = "./file_names.csv";

	// After how many batches to create a new checkpoint periodically.
	const int64_t kCheckpointEvery = 50;

	// How many images to sample at every checkpoint.
	const int64_t kNumberOfSamplesPerCheckpoint = 64;

	// After how many batches to log a new update with the loss value.
	const int64_t kLogInterval = 10;

	// Learning Rate
	const double kLr = 2e-4;

	// Beta1
	const double kBeta1 = 0.5;

	// Beta2
	const double kBeta2 = 0.999;
};