#pragma once
#include <DirectXMath.h>
#include <d3d12.h>
#include <wrl.h>

class Light
{
private: //�G�C���A���X
	//Microsoft::WRL::���ȗ�
	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: //�T�u�N���X
	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferDate
	{
		XMVECTOR lightv; //���C�g�ւ̕�����\���x�N�g��
		XMFLOAT3 lightcolor; //�F
	};

private: //�ÓI�����o�ϐ�
	//�f�o�C�X
	static ID3D12Device* device;

	static Light* Create();

public: //�ÓI�����o�֐�

	static void StaticInitialize(ID3D12Device* device);
	
	void Initialize();

	void TransferConstBuffer();

	void SetLightDir(const XMVECTOR& lightdir);

	void SetLightColor(const XMFLOAT3& lightcolor);

	void Update();

	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex);


private: //�����o�ϐ�
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource>constBuff;
	//���C�g��������(�P�ʃx�N�g��)
	XMVECTOR lightdir = { 1,0,0,0 };
	//���C�g�F
	XMFLOAT3 lightcolor = { 1,1,1 };
	//�_�[�f�B�t���O
	bool dirty = false;

};

