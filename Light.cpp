#include "Light.h"
using namespace DirectX;

ID3D12Device* Light::device = nullptr;

void Light::StaticInitialize(ID3D12Device* device)
{
	//再初期化チェック
	assert(!Light::device);
	//nullptrチェック
	assert(device);
	//静的メンバ変数のセット
	Light::device = device;
}

void Light::Initialize()
{
	//ヒープ設定

	cbHeapProp_.Type = D3D12_HEAP_TYPE_UPLOAD;

	//リソース設定
	cbResourceDesc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc_.Width = (sizeof(ConstBufferData) + 0xff) & ~0xff;
	cbResourceDesc_.Height = 1;
	cbResourceDesc_.DepthOrArraySize = 1;
	cbResourceDesc_.MipLevels = 1;
	cbResourceDesc_.SampleDesc.Count = 1;
	cbResourceDesc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	CreateConstBuffer();

	//定数バッファへデータ転送
	TransferConstBuffer();
}

void Light::CreateConstBuffer()
{
	HRESULT result;
	result = device->CreateCommittedResource(
		&cbHeapProp_, //ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc_, //リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff_));
	assert(SUCCEEDED(result));
}

void Light::TransferConstBuffer()
{
	HRESULT result;

	//定数バッファへデータ転送
	ConstBufferData* constMap = nullptr;
	result = constBuff_->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->lightv = -lightdir_;
		constMap->lihtcolor = lightcolor_;
		constBuff_->Unmap(0, nullptr);

	}
}

void Light::SetLightDir(const XMVECTOR& lightdir)
{
	//正規化してセット
	this->lightdir_ = XMVector3Normalize(lightdir);
	dirty_ = true;

}

void Light::SetLightColor(const XMFLOAT3& lightcolor)
{
	this->lightcolor_ = lightcolor;
}

void Light::Update()
{
	//値の更新があった時だけ定数バッファに転送する
	if (dirty_) {
		TransferConstBuffer();
		dirty_ = false;
	}
}

void Light::Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex)
{
	//定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(rootParameterIndex, constBuff_->GetGPUVirtualAddress());
}

Light* Light::Create()
{
	//3Dオブジェクトのインスタンスを生成
	Light* instance = new Light();
	//初期化
	instance->Initialize();
	//生成したインスタンスを返す
	return instance;
}