#pragma once
#include "DirectX/PSO.h"
#include "AllInclude/AllD3D12Include.h"
#include "../DirectX/DescriptorHeap.h"
#include <algorithm> 
#include<array>

using Microsoft::WRL::ComPtr;

class TextureManager;
class DX12Context;

class RenderTexture
{

public:

	RenderTexture() = default;
	~RenderTexture() = default;


	void CreateSceneRtv(
		ID3D12Device* device, 
		TextureManager* textureManager,
		int32_t width, int32_t height);

	void SetRenderTarget(ComPtr<ID3D12GraphicsCommandList> commandList);
	void ClearRenderTarget(ComPtr<ID3D12GraphicsCommandList> commandList);

private:
	ComPtr<ID3D12DescriptorHeap> mSceneRtvDescriptorHeap;
	ComPtr<ID3D12Resource> mSceneColorResource;
	D3D12_CPU_DESCRIPTOR_HANDLE mSceneRtvHandle{};
	D3D12_CPU_DESCRIPTOR_HANDLE mSceneSrvCpuHandle{};
	D3D12_GPU_DESCRIPTOR_HANDLE mSceneSrvGpuHandle{};

	int mSlot = -1;

};

