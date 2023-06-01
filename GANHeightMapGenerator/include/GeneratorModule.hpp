#pragma once
#include "torch/torch.h"

struct DCGANGeneratorImpl : torch::nn::Module {
	DCGANGeneratorImpl(int kNoiseSize, int imageSize = 64)
		: conv1(torch::nn::ConvTranspose2dOptions(kNoiseSize, imageSize * 4, 4)
			.bias(false)),
		batch_norm1(imageSize * 4),
		conv2(torch::nn::ConvTranspose2dOptions(imageSize * 4, imageSize * 2, 3)
			.stride(2)
			.padding(1)
			.bias(false)),
		batch_norm2(imageSize * 2),
		conv3(torch::nn::ConvTranspose2dOptions(imageSize * 2, imageSize, 4)
			.stride(2)
			.padding(1)
			.bias(false)),
		batch_norm3(imageSize),
		conv4(torch::nn::ConvTranspose2dOptions(imageSize, 1, 4)
			.stride(2)
			.padding(1)
			.bias(false))
	{
		// register_module() is needed if we want to use the parameters() method later on
		register_module("conv1", conv1);
		register_module("conv2", conv2);
		register_module("conv3", conv3);
		register_module("conv4", conv4);
		register_module("batch_norm1", batch_norm1);
		register_module("batch_norm2", batch_norm2);
		register_module("batch_norm3", batch_norm3);
	}

	torch::Tensor forward(torch::Tensor x) {
		x = torch::relu(batch_norm1(conv1(x)));
		x = torch::relu(batch_norm2(conv2(x)));
		x = torch::relu(batch_norm3(conv3(x)));
		x = torch::tanh(conv4(x));
		return x;
	}

	torch::nn::ConvTranspose2d conv1, conv2, conv3, conv4;
	torch::nn::BatchNorm2d batch_norm1, batch_norm2, batch_norm3;
};
TORCH_MODULE(DCGANGenerator);