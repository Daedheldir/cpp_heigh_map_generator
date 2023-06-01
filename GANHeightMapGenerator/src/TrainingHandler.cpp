#include "TrainingHandler.hpp"
#include "GeneratorModule.hpp"
#include "DiscriminatorModule.hpp"
#include <opencv2/opencv.hpp>
#include <vector>

TrainingHandler::TrainingHandler()
{
}

TrainingHandler::~TrainingHandler()
{
}
auto ToCvImage(at::Tensor tensor)
{
	int width = tensor.sizes()[0];
	int height = tensor.sizes()[1];
	tensor.toType(c10::kFloat);
	try
	{
		cv::Mat output_mat(cv::Size{ height, width }, CV_8UC3, tensor.data_ptr<uchar>());
		return output_mat.clone();
	}
	catch (const c10::Error& e)
	{
		std::cout << "an error has occured : " << e.msg() << std::endl;
	}
	return cv::Mat(height, width, CV_8UC3);
}

void TrainingHandler::TrainModel()
{
	using namespace torch;
	manual_seed(42);

	torch::Device device(torch::kCPU);
	if (torch::cuda::is_available()) {
		std::cout << "CUDA available! Training on GPU." << std::endl;
		device = torch::Device(torch::kCUDA);
	}

	int kLatentDim = 100;
	int kImageSize = 32;
	DCGANGenerator dcgan(kLatentDim, kImageSize);

	dcgan->to(device);
	Tensor in_tensor = torch::randn({ 1, kLatentDim, 1, 1 }, device);
	std::cout << "In_tensor: " << in_tensor.sizes() << std::endl;
	Tensor out_tensor = dcgan->forward(in_tensor);

	//torch::save(
	//	(out_tensor + 1.0) / 2.0,
	//	torch::str("./output/dcgan-sample-", 0, ".pt"));
	//out_tensor = out_tensor.detach();
	//std::cout << "Out_tensor: " << out_tensor.sizes() << std::endl;
	////out_tensor = out_tensor.permute({ 1,1,1,1 });
	//out_tensor = out_tensor.mul(255).clamp(0, 255).to(torch::kU8);
	//out_tensor = out_tensor.to(torch::kCPU);
	//cv::Mat resultImg(kImageSize, kImageSize, CV_8UC3);
	//std::memcpy((void*)resultImg.data, out_tensor.data_ptr(), sizeof(torch::kU8) * out_tensor.numel());
	//std::cout << "Tensor converetd to Image!!" << std::endl;
	//cv::imwrite("./output/new_img.jpg", resultImg);

	//std::vector<float> output_vector = out_tensor.data<float>();
	//Mat m = Mat(512, 1024, CV_32F);
	//memcpy(m.data, output_vector.data(), output_vector.size() * sizeof(float));
}