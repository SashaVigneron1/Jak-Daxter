//INCOMPLETE!

float4x4 gTransform : WorldViewProjection;
Texture2D gSpriteTexture;
float2 gTextureSize;

SamplerState samPoint
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = WRAP;
	AddressV = WRAP;
};

BlendState EnableBlending
{
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
	DestBlend = INV_SRC_ALPHA;
};

BlendState AlphaBlending
{
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
	DestBlend = INV_SRC_ALPHA;
	BlendOp = ADD;
	SrcBlendAlpha = ONE;
	DestBlendAlpha = ZERO;
	BlendOpAlpha = ADD;
	RenderTargetWriteMask[0] = 0x0f;
};

DepthStencilState DisableDepthWriting
{
	//Enable Depth Rendering
	DepthEnable = TRUE;
	//Disable Depth Writing
	DepthWriteMask = ZERO;
};

DepthStencilState NoDepth
{
	DepthEnable = FALSE;
};

RasterizerState BackCulling
{
	CullMode = BACK;
};

//SHADER STRUCTS
//**************
struct VS_DATA
{
	uint TextureId : TEXCOORD0;
	float4 TransformData : POSITION; //PosX, PosY, Depth (PosZ), Rotation
	float4 TransformData2 : POSITION1; //PivotX, PivotY, ScaleX, ScaleY
	float4 Color : COLOR;
};

struct GS_DATA
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
	float2 TexCoord : TEXCOORD0;
};

//VERTEX SHADER
//*************
VS_DATA MainVS(VS_DATA input)
{
	return input;
}

//GEOMETRY SHADER
//***************
void CreateVertex(inout TriangleStream<GS_DATA> triStream, float3 vertexPos, float3 pivotPos, float4 col, float2 texCoord, float2 rotCosSin, float2 pivotOffset)
{
	float3 newPos = float3(0, 0, 0);

	vertexPos.x += pivotOffset.x;
	vertexPos.y += pivotOffset.y;

	newPos.x = (vertexPos.x * rotCosSin.x) - (vertexPos.y * rotCosSin.y);
	newPos.y = (vertexPos.y * rotCosSin.x) + (vertexPos.x * rotCosSin.y);

	/*newPos.x -= pivotOffset.x;
	newPos.y -= pivotOffset.y;*/

	newPos += pivotPos;

	//Geometry Vertex Output
	GS_DATA geomData = (GS_DATA)0;
	geomData.Position = mul(float4(newPos, 1.0f), gTransform);
	geomData.Color = col;
	geomData.TexCoord = texCoord;
	triStream.Append(geomData);
}

[maxvertexcount(4)]
void MainGS(point VS_DATA vertex[1], inout TriangleStream<GS_DATA> triStream)
{
	//Given Data (Vertex Data)
	float3 position = vertex[0].TransformData.xyz; //Extract the position data from the VS_DATA vertex struct
	float rotation = vertex[0].TransformData.w; //Extract the rotation data from the VS_DATA vertex struct
	float2 pivot = vertex[0].TransformData2.xy; //Extract the pivot data from the VS_DATA vertex struct
	float2 scale = vertex[0].TransformData2.zw; //Extract the scale data from the VS_DATA vertex struct

	pivot.x = -(pivot.x * gTextureSize.x * scale.x) + ((gTextureSize.x / 2) * scale.x);
	pivot.y = -(pivot.y * gTextureSize.y * scale.y) + ((gTextureSize.y / 2) * scale.y);

	// LT----------RT //TriangleStrip (LT > RT > LB, LB > RB > RT)
	// |          / |
	// |       /    |
	// |    /       |
	// | /          |
	// LB----------RB

	// Variables
	float2 rotCosSin = float2(cos(rotation), sin(rotation));

	//VERTEX 1 [LT]
	float3 vertexPos = float3((-gTextureSize.x / 2) * scale.x, (gTextureSize.y / 2) * scale.y, 0);
	CreateVertex(triStream, vertexPos, position, vertex[0].Color, float2(0, 1), rotCosSin, pivot);

	//VERTEX 3 [LB]
	vertexPos = float3((-gTextureSize.x / 2) * scale.x, (-gTextureSize.y / 2) * scale.y, 0);
	CreateVertex(triStream, vertexPos, position, vertex[0].Color, float2(0, 0), rotCosSin, pivot);

	//VERTEX 2 [RT]
	vertexPos = float3((gTextureSize.x / 2) * scale.x, (gTextureSize.y / 2) * scale.y, 0);
	CreateVertex(triStream, vertexPos, position, vertex[0].Color, float2(1, 1), rotCosSin, pivot);

	//VERTEX 4 [RB]
	vertexPos = float3((gTextureSize.x / 2) * scale.x, (-gTextureSize.y / 2) * scale.y, 0);
	CreateVertex(triStream, vertexPos, position, vertex[0].Color, float2(1, 0), rotCosSin, pivot);
}

//PIXEL SHADER
//************
float4 MainPS(GS_DATA input) : SV_TARGET
{
	return gSpriteTexture.Sample(samPoint, input.TexCoord) * input.Color;
}

// Default Technique
technique11 Default
{
	pass p0
	{
		SetRasterizerState(BackCulling);
		SetBlendState(EnableBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetDepthStencilState(DisableDepthWriting,0);
		SetVertexShader(CompileShader(vs_4_0, MainVS()));
		SetGeometryShader(CompileShader(gs_4_0, MainGS()));
		SetPixelShader(CompileShader(ps_4_0, MainPS()));
	}
}
