

cbuffer cbViewMatrix : register(b0)
{
	matrix gMatView : packoffset(c0);
}

cbuffer cbProjMatrix : register(b1)
{
	matrix gMatProj : packoffset(c0);
}

cbuffer cbWorldMatrix : register(b2)
{
	matrix gMatWorld : packoffset(c0);
}

cbuffer cbPixelBuffer
{
	float4 gPixelColor : packoffset(c0);
};

struct Vertex_In
{
	float4 pos : POSITION;
	float2 tex2dcoord : TEXCOORD0;
};

struct Vertex_Out
{
	float4 pos : SV_POSITION;
	float2 tex2dcoord : TEXCOORD0;
};

Vertex_Out VS(Vertex_In input)
{
	Vertex_Out output = (Vertex_Out)0;

	input.pos.w = 1.f;

	matrix matAll = mul(gMatWorld, gMatView);
	matAll = mul(matAll, gMatProj);

	output.pos = mul(input.pos, matAll);
	output.tex2dcoord = input.tex2dcoord;

	return output;
}

Texture2D gTexture : register(t0);
SamplerState gSamplerState : register(s0);

float4 PS(Vertex_Out input) : SV_Target
{
	float4	color = gTexture.Sample(gSamplerState, input.tex2dcoord);

	if (0.0f == color.r)
		color.a = 0.0f;
	else
	{
		color.a = 1.0f;
		color = color * gPixelColor;
	}
	
	return color;
}