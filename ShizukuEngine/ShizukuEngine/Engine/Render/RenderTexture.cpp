#include "RenderTexture.h"
#include "TextureManager/TextureManager.h"

void RenderTexture::CreateSceneRtv(
	ID3D12Device* device,
	TextureManager* textureManager,
	int32_t width,
	int32_t height
) {

	D3D12_RESOURCE_DESC desc{};
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.DepthOrArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	desc.SampleDesc.Count = 1;
	desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	D3D12_CLEAR_VALUE clearValue{};
	clearValue.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	clearValue.Color[0] = 0.05f;
	clearValue.Color[1] = 0.12f;
	clearValue.Color[2] = 0.25f;
	clearValue.Color[3] = 1.0f;

	mSceneRtvDescriptorHeap.Reset();
	HRESULT hr{};
	hr = device->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&clearValue,
		IID_PPV_ARGS(mSceneColorResource.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(hr));

	mSceneRtvDescriptorHeap = CreateDescriptorHeap(device, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 1, false);
	mSceneRtvHandle = mSceneRtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	device->CreateRenderTargetView(mSceneColorResource.Get(), nullptr, mSceneRtvHandle);

	//SRV作成
	if (mSlot < 0) {
		mSlot = textureManager->AllocateSrv();
	}
	assert(mSlot >= 0);

	mSceneSrvCpuHandle = textureManager->GetCPUHandle(mSlot);
	mSceneSrvGpuHandle = textureManager->GetGPUHandle(mSlot);
	_ASSERT_EXPR(mSceneSrvCpuHandle.ptr != 0, L"SRVのCPUハンドルが取得できませんでした");

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Texture2D.MipLevels = 1;
	device->CreateShaderResourceView(mSceneColorResource.Get(), &srvDesc, mSceneSrvCpuHandle);

}