#include "Box.h"
#include "BindableBase.h"
#include "GraphicsThrowMacros.h"
#include "Cube.h"

Box::Box(Graphics& gfx,
	std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist,
	std::uniform_real_distribution<float>& bdist,
	DirectX::XMFLOAT3 material)
	:
	Object(gfx, rng, adist, ddist, odist, rdist)
{
	namespace dx = DirectX;
	if (!IsStaticInitialized())
	{
		struct Vertex
		{
			dx::XMFLOAT3 pos;
			dx::XMFLOAT3 n;
		};

		auto model = Cube::MakeIndependent<Vertex>();
		model.SetNormalsIndependentFlat();

		AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

		auto pvs = std::make_unique<VertexShader>(gfx, L"PhongVS.hlsl");
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(gfx, L"PhongPS.hlsl"));

		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind(std::make_unique<TransformCbuf>(gfx, *this));

	struct PSMaterialConstant
	{
		dx::XMFLOAT3 color;
		float padding[1];
	} colorConst;
	colorConst.color = material;
	AddBind(std::make_unique<PixelConstantBuffer<PSMaterialConstant>>(gfx, colorConst, 1u));

	dx::XMStoreFloat3x3(
		&mt,
		dx::XMMatrixScaling(1.0f, 1.0f, bdist(rng))
	);
}

DirectX::XMMATRIX Box::GetTransformXM() const noexcept
{
	namespace dx = DirectX;

	return dx::XMLoadFloat3x3(&mt) * Object::GetTransformXM();
}