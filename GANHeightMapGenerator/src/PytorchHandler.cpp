#include "PytorchHandler.hpp"
#include "PytorchHandler.hpp"
#include "GeneratorModule.hpp"
#include <filesystem>

PytorchHandler::PytorchHandler()
{
}

std::string PytorchHandler::test()
{
	return "Test";
}

PytorchHandler::LabelsCsv PytorchHandler::ReadCsv(const std::string& location)
{
	std::fstream in(location, std::ios::in);
	std::string line;
	std::string name;
	std::string label;
	LabelsCsv csv;

	while (getline(in, line)) {
		std::stringstream s(line);
		getline(s, name, ',');
		getline(s, label, ',');

		csv.push_back(std::make_tuple("../" + name, stoi(label)));
	}
	return csv;
}
auto ToCvImage(at::Tensor tensor)
{
	int width = tensor.sizes()[0];
	int height = tensor.sizes()[1];
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
void PytorchHandler::CreateModel(LabelsCsv& labelsCsv)
{
	using namespace torch;

	manual_seed(42);

	torch::Device device(torch::kCPU);
	//if (torch::cuda::is_available()) {
	//	std::cout << "CUDA available! Training on GPU." << std::endl;
	//	device = torch::Device(torch::kCUDA);
	//}

	DCGANGenerator generator(kLatentDim);
	generator->to(device);

	nn::Sequential discriminator(
		// Layer 1
		nn::Conv2d(
			nn::Conv2dOptions(3, 64, 4).stride(2).padding(1).bias(false)),
		nn::LeakyReLU(nn::LeakyReLUOptions().negative_slope(0.2)),
		// Layer 2
		nn::Conv2d(
			nn::Conv2dOptions(64, 128, 4).stride(2).padding(1).bias(false)),
		nn::BatchNorm2d(128),
		nn::LeakyReLU(nn::LeakyReLUOptions().negative_slope(0.2)),
		// Layer 3
		nn::Conv2d(
			nn::Conv2dOptions(128, 256, 4).stride(2).padding(1).bias(false)),
		nn::BatchNorm2d(256),
		nn::LeakyReLU(nn::LeakyReLUOptions().negative_slope(0.2)),
		// Layer 4
		nn::Conv2d(
			nn::Conv2dOptions(256, 512, 4).stride(2).padding(1).bias(false)),
		nn::BatchNorm2d(512),
		nn::LeakyReLU(nn::LeakyReLUOptions().negative_slope(0.2)),
		// Layer 5
		nn::Conv2d(
			nn::Conv2dOptions(512, 1, 4).stride(1).padding(0).bias(false)),
		nn::Sigmoid());
	discriminator->to(device);

	auto dataset = Dataset(labelsCsv, kImageSize)
		.map(data::transforms::Normalize<>(0.5, 0.5))
		.map(data::transforms::Stack<>());

	const int64_t batches_per_epoch =
		std::ceil(dataset.size().value() / static_cast<double>(kBatchSize));

	auto data_loader = data::make_data_loader<data::samplers::RandomSampler>(
		dataset,
		data::DataLoaderOptions().workers(kNumOfWorkers).batch_size(kBatchSize).enforce_ordering(kEnforceOrder));

	optim::Adam generator_optimizer(
		generator->parameters(), optim::AdamOptions(kLr).betas(std::make_tuple(kBeta1, kBeta2)));
	optim::Adam discriminator_optimizer(
		discriminator->parameters(), optim::AdamOptions(kLr).betas(std::make_tuple(kBeta1, kBeta2)));

	int64_t checkpoint_counter = 1;
	std::cout << "Epoch/NumberOfEpochs, batch_index/batches_per_epoch, d_loss, g_loss" << std::endl;

	for (int64_t epoch = 1; epoch <= kNumberOfEpochs; ++epoch) {
		int64_t batch_index = 0;
		for (auto& batch : *data_loader) {
			// Train discriminator with real images.
			discriminator->zero_grad();
			Tensor real_images = batch.data.to(device);
			Tensor real_labels =
				torch::empty(batch.data.size(0), device).uniform_(0.8, 1.0);
			Tensor real_output = discriminator->forward(real_images);
			Tensor d_loss_real =
				binary_cross_entropy(real_output, real_labels); //UNHANDLED EXCEPTION HERE
			d_loss_real.backward();

			// Train discriminator with fake images.
			Tensor noise =
				torch::randn({ batch.data.size(0), kLatentDim, 1, 1 }, device);
			Tensor fake_images = generator->forward(noise);
			Tensor fake_labels = torch::zeros(batch.data.size(0), device);
			Tensor fake_output = discriminator->forward(fake_images.detach());
			Tensor d_loss_fake =
				torch::binary_cross_entropy(fake_output, fake_labels);
			d_loss_fake.backward();

			Tensor d_loss = d_loss_real + d_loss_fake;
			discriminator_optimizer.step();

			// Train generator.
			generator->zero_grad();
			fake_labels.fill_(1);
			fake_output = discriminator->forward(fake_images);
			Tensor g_loss =
				torch::binary_cross_entropy(fake_output, fake_labels);
			g_loss.backward();
			generator_optimizer.step();
			batch_index++;
			if (batch_index % kLogInterval == 0) {
				std::printf(
					"\r[%2ld/%2ld][%3ld/%3ld] D_loss: %.4f | G_loss: %.4f\n",
					epoch,
					kNumberOfEpochs,
					batch_index,
					batches_per_epoch,
					d_loss.item<float>(),
					g_loss.item<float>());
			}
			if (batch_index % kCheckpointEvery == 0) {
				if (!std::filesystem::exists("./output")) {
					std::filesystem::create_directories("./output");
				}
				else {
					std::filesystem::remove_all("./output/");
				}

				// Checkpoint the model and optimizer state.
				torch::save(generator, "./output/generator-checkpoint.pt");
				torch::save(generator_optimizer, "./output/generator-optimizer-checkpoint.pt");
				torch::save(discriminator, "./output/discriminator-checkpoint.pt");
				torch::save(
					discriminator_optimizer, "./output/discriminator-optimizer-checkpoint.pt");
				// Sample the generator and save the images.
				Tensor samples = generator->forward(torch::randn(
					{ kNumberOfSamplesPerCheckpoint, kLatentDim, 1, 1 }, device));
				//auto cvImage = ToCvImage(samples);

				//cv::imwrite(
				//	torch::str("./output/dcgan-sample-", checkpoint_counter, ".png"),
				//	cvImage);
				torch::save(
					(samples + 1.0) / 2.0,
					torch::str("./output/dcgan-sample-", checkpoint_counter, ".pt"));
				std::cout << "\n-> checkpoint " << ++checkpoint_counter << '\n';
			}
		}
	}

	std::cout << "Training complete!" << std::endl;
}

void PytorchHandler::testing()
{
	DCGANGenerator dcgan;
}