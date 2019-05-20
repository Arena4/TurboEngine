#include "model.h"

ZModel::ZModel()
{
	m_indexCount = 0;
	m_vertexCount = 0;
	m_Texture = 0;
}

ZModel::ZModel(const ZModel &)
{
}

ZModel::~ZModel()
{
}

bool ZModel::Initialize(ID3D11Device* device, WCHAR* name)
{
	bool result = InitializeBuffers(device);

	if (!result) return false;

	result = LoadTexture(device, name);
	if (!result) return false;

	return result;
}

void ZModel::Shutdown()
{
	ReleaseTexture();
	ShutdownBuffers();
}

void ZModel::Render(ID3D11DeviceContext* context)
{
	RenderBuffers(context);
}

int ZModel::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* ZModel::GetTexture()
{
	return m_Texture->GetTexture();
}

bool ZModel::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	m_vertexCount = 3;
	m_indexCount = 3;
	
	vertices = new VertexType[m_vertexCount];
	indices = new unsigned long[m_indexCount];

	//init vertex & index data.
	vertices[0].position =  XMFLOAT3(-1.f, -1.f, 0.f);
	vertices[0].texture = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertices[0].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertices[1].texture = XMFLOAT3(0.5f, 0.0f, 0.0f);
	vertices[1].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);
	vertices[2].texture = XMFLOAT3(1.0f, 1.0f, 0.0f);
	vertices[2].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	//Setup the description of the static vertex buffer
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	vertexData.pSysMem = vertices;

	//Create vertext buffer
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if(FAILED(result))
		return false;

	//Setup the description of the static index buffer
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	indexData.pSysMem = indices;
	
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
		return false;

	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void ZModel::ShutdownBuffers()
{
	if(m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}

void ZModel::RenderBuffers(ID3D11DeviceContext* context)
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool ZModel::LoadTexture(ID3D11Device* device, WCHAR* name)
{
	bool result;

	m_Texture = new ZTexture;
	if (!m_Texture) return false;

	result = m_Texture->Initialize(device, name);
	if (!result) return false;

	return true;
}

void ZModel::ReleaseTexture()
{
	if(m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}
}
