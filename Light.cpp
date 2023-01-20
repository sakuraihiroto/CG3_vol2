#include "Light.h"
using namespace DirectX;

ID3D12Device* Light::device = nullptr;

void Light::StaticInitialize(ID3D12Device* device)
{
	//�ď������`�F�b�N
	assert(!Light::device);
	//nullptr�`�F�b�N
	assert(device);
	//�ÓI�����o�ϐ��̃Z�b�g
	Light::device = device;
}

void Light::Initialize()
{
	//�q�[�v�ݒ�

	cbHeapProp_.Type = D3D12_HEAP_TYPE_UPLOAD;

	//���\�[�X�ݒ�
	cbResourceDesc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc_.Width = (sizeof(ConstBufferData) + 0xff) & ~0xff;
	cbResourceDesc_.Height = 1;
	cbResourceDesc_.DepthOrArraySize = 1;
	cbResourceDesc_.MipLevels = 1;
	cbResourceDesc_.SampleDesc.Count = 1;
	cbResourceDesc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	CreateConstBuffer();

	//�萔�o�b�t�@�փf�[�^�]��
	TransferConstBuffer();
}

void Light::CreateConstBuffer()
{
	HRESULT result;
	result = device->CreateCommittedResource(
		&cbHeapProp_, //�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc_, //���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff_));
	assert(SUCCEEDED(result));
}

void Light::TransferConstBuffer()
{
	HRESULT result;

	//�萔�o�b�t�@�փf�[�^�]��
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
	//���K�����ăZ�b�g
	this->lightdir_ = XMVector3Normalize(lightdir);
	dirty_ = true;

}

void Light::SetLightColor(const XMFLOAT3& lightcolor)
{
	this->lightcolor_ = lightcolor;
}

void Light::Update()
{
	//�l�̍X�V���������������萔�o�b�t�@�ɓ]������
	if (dirty_) {
		TransferConstBuffer();
		dirty_ = false;
	}
}

void Light::Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex)
{
	//�萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(rootParameterIndex, constBuff_->GetGPUVirtualAddress());
}

Light* Light::Create()
{
	//3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	Light* instance = new Light();
	//������
	instance->Initialize();
	//���������C���X�^���X��Ԃ�
	return instance;
}