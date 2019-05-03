#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <stdio.h>

#include <multfly_ref.h>
#include <multfly_device.cuh>

#define CUDA_CHECK(code) do { \
	if (code != cudaSuccess) { \
		throw std::runtime_error(__FILE__ ":" + std::to_string(__LINE__) + " Cuda error" + std::to_string(code)); \
	} \
} while (0)

#define TEST_KEY_NAME "multfly_test"

__global__ void init_key(multfly_key *key, uint64_t global_seed, uint64_t global_ctr) {
	multfly_device_init_by_literal(key, TEST_KEY_NAME, global_seed, global_ctr);
}

__global__ void generate_u32(const multfly_key *key, uint32_t *result0, uint32_t *result1) {
	uint64_t id = blockIdx.x * blockDim.x + threadIdx.x;
	result0[id] = multfly_device_gen32(key, id >> 2, 0);
	result1[id] = multfly_device_gen32(key, id >> 2, 1);
}

int main() {
	int ng = 0;

	CUDA_CHECK(cudaSetDevice(0));

	int global_seed = 0;
	int global_ctr = 0;

	multfly_key *d_key;
	CUDA_CHECK(cudaMalloc(&d_key, sizeof(multfly_key)));
	init_key<<<1, 4>>>(d_key, global_seed, global_ctr);
	CUDA_CHECK(cudaDeviceSynchronize());

	multfly_key h_key, h_key_test;
	CUDA_CHECK(cudaMemcpy(&h_key, d_key, sizeof(multfly_key), cudaMemcpyDeviceToHost));
	h_key_test = multfly_init_by_literal(TEST_KEY_NAME, global_seed, global_ctr);
	for (int i = 0; i < 8; i++) {
		if (h_key.v_[i] != h_key_test.v_[i]) {
			ng = 1;
			std::cout << "init fail" << std::endl;
			break;
		}
	}

	int len = 8192;
	int n_threads = 128;
	int n_blocks = len / n_threads;

	uint32_t *d_result0;
	uint32_t *d_result1;
	CUDA_CHECK(cudaMalloc(&d_result0, sizeof(uint32_t) * len));
	CUDA_CHECK(cudaMalloc(&d_result1, sizeof(uint32_t) * len));
	generate_u32<<<n_blocks, n_threads>>>(d_key, d_result0, d_result1);
	CUDA_CHECK(cudaDeviceSynchronize());

	std::vector<uint32_t> h_result0(len);
	std::vector<uint32_t> h_result1(len);
	uint32_t h_result_test0[4];
	uint32_t h_result_test1[4];
	CUDA_CHECK(cudaMemcpy(h_result0.data(), d_result0, sizeof(uint32_t) * len, cudaMemcpyDeviceToHost));
	CUDA_CHECK(cudaMemcpy(h_result1.data(), d_result1, sizeof(uint32_t) * len, cudaMemcpyDeviceToHost));
	for (int i = 0; i < len; i += 4) {
		multfly_gen32(&h_key_test, i >> 2, 0, h_result_test0);
		multfly_gen32(&h_key_test, i >> 2, 1, h_result_test1);
		for (int j = 0; j < 4; j++) {
			if (h_result0[i + j] != h_result_test0[j]) {
				ng = 1;
				std::cout << "uint32 result0 fail" << std::endl;
				i = len;
				break;
			}
			if (h_result1[i + j] != h_result_test1[j]) {
				ng = 1;
				std::cout << "uint32 result1 fail" << std::endl;
				i = len;
				break;
			}
		}
	}

	if (!ng) {
		std::cout << "ok" << std::endl;
	}
	return ng;
}
