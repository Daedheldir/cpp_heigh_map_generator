#pragma once
#include "torch/torch.h"

struct DCGANDiscriminatorImpl : torch::nn::Module {
	DCGANDiscriminatorImpl(int kNoiseSize, int imageSize = 64) :
		conv1(torch::nn::Conv2dOptions(3, imageSize, 4)
			.stride(2)
			.padding(1)
			.bias(false)),
		conv2(torch::nn::Conv2dOptions(imageSize, imageSize * 2, 4)
			.stride(2)
			.padding(1)
			.bias(false)),
		conv3(torch::nn::Conv2dOptions(imageSize * 2, imageSize * 4, 4)
			.stride(2)
			.padding(1)
			.bias(false)),
		conv4(torch::nn::Conv2dOptions(imageSize * 4, imageSize * 8, 4)
			.stride(2)
			.padding(1)
			.bias(false)),
		conv5(torch::nn::Conv2dOptions(imageSize * 8, 1, 4)
			.stride(2)
			.padding(1)
			.bias(false)),
		batch_norm2(imageSize * 2),
		batch_norm3(imageSize * 4),
		batch_norm4(imageSize * 8)
	{
		register_module("conv1", conv1);
		register_module("conv2", conv2);
		register_module("conv3", conv3);
		register_module("conv4", conv4);
		register_module("conv5", conv5);
		register_module("batch_norm2", batch_norm2);
		register_module("batch_norm3", batch_norm3);
		register_module("batch_norm4", batch_norm4);
	}

	torch::Tensor forward(torch::Tensor x) {
		x = torch::leaky_relu(conv1(x), 0.2);
		x = torch::leaky_relu(batch_norm2(conv2(x)), 0.2);
		x = torch::leaky_relu(batch_norm3(conv3(x)), 0.2);
		x = torch::leaky_relu(batch_norm4(conv4(x)), 0.2);
		x = torch::sigmoid(conv5(x));

		return x;
	}

	torch::nn::Conv2d conv1, conv2, conv3, conv4, conv5;
	torch::nn::BatchNorm2d batch_norm2, batch_norm3, batch_norm4;
};
TORCH_MODULE(DCGANDiscriminator);

//nn::Sequential discriminator(
//	// Layer 1
//	nn::Conv2d(
//		nn::Conv2dOptions(3, 64, 4).stride(2).padding(1).bias(false)),
//	nn::LeakyReLU(nn::LeakyReLUOptions().negative_slope(0.2)),
//	// Layer 2
//	nn::Conv2d(
//		nn::Conv2dOptions(64, 128, 4).stride(2).padding(1).bias(false)),
//	nn::BatchNorm2d(128),
//	nn::LeakyReLU(nn::LeakyReLUOptions().negative_slope(0.2)),
//	// Layer 3
//	nn::Conv2d(
//		nn::Conv2dOptions(128, 256, 4).stride(2).padding(1).bias(false)),
//	nn::BatchNorm2d(256),
//	nn::LeakyReLU(nn::LeakyReLUOptions().negative_slope(0.2)),
//	// Layer 4
//	nn::Conv2d(
//		nn::Conv2dOptions(256, 512, 4).stride(2).padding(1).bias(false)),
//	nn::BatchNorm2d(512),
//	nn::LeakyReLU(nn::LeakyReLUOptions().negative_slope(0.2)),
//	// Layer 5
//	nn::Conv2d(
//		nn::Conv2dOptions(512, 1, 4).stride(1).padding(0).bias(false)),
//	nn::Sigmoid());
//discriminator->to(device);