#pragma once
#include <DirectXMath.h>
#include <d3d12.h>
#include <wrl.h>

class Light
{
private: //エイリアンス
	//Microsoft::WRL::を省略
	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: //サブクラス
	//定数バッファ用データ構造体
	struct ConstBufferDate
	{
		XMVECTOR lightv; //ライトへの方向を表すベクトル
		XMFLOAT3 lightcolor; //色
	};

private: //静的メンバ変数
	//デバイス
	static ID3D12Device* device;

	static Light* Create();

public: //静的メンバ関数

	static void StaticInitialize(ID3D12Device* device);
	
	void Initialize();

	void TransferConstBuffer();

	void SetLightDir(const XMVECTOR& lightdir);

	void SetLightColor(const XMFLOAT3& lightcolor);

	void Update();

	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex);


private: //メンバ変数
	//定数バッファ
	ComPtr<ID3D12Resource>constBuff;
	//ライト光線方向(単位ベクトル)
	XMVECTOR lightdir = { 1,0,0,0 };
	//ライト色
	XMFLOAT3 lightcolor = { 1,1,1 };
	//ダーディフラグ
	bool dirty = false;

};

