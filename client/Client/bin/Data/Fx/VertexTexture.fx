
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

cbuffer cbWorldCameraPosition : register(b4)
{
	float4 gWorldCameraPosition : packoffset(c0);
}

float4 gWorldLightPosition = float4(400.0f, 1000.0f, 600.0f, 0.0f);


struct Vertex_In
{
	float4 pos : POSITION;
	float4 normal : NORMAL;
	float2 tex2dcoord : TEXCOORD0;
};

struct Vertex_Out
{
	float4 pos : SV_POSITION;
	float4 normal : NORMAL;
	float2 tex2dcoord : TEXCOORD0;

	float3 mDiffuse:TEXCOORD1;
	float3 mViewDir:TEXCOORD2;
	float3 mReflection:TEXCOORD3;
};

Vertex_Out VS(Vertex_In input)
{
	Vertex_Out output = (Vertex_Out)0;

	
	matrix matAll = mul(gMatWorld, gMatView);
	matAll = mul(matAll, gMatProj);

	//output.pos = mul(input.pos, matAll);

	output.pos = mul(input.pos, gMatWorld);

	float3 lightDir = normalize(output.pos.xyz - gWorldLightPosition.xyz);
	output.mViewDir = normalize(output.pos.xyz - gWorldCameraPosition.xyz);

	//output.pos = mul(input.pos, gMatView);
	//output.pos = mul(input.pos, gMatProj);

	output.pos = mul(input.pos, matAll);


	float3 worldNormal = normalize(mul((float3)input.normal, (float3x3)gMatWorld));
	output.mDiffuse = dot(-lightDir, worldNormal);
	output.mReflection = reflect(lightDir, worldNormal);
	
	/*
	matrix matAll = mul(gMatWorld, gMatView);
	matAll = mul(matAll, gMatProj);

	output.pos = mul(input.pos, matAll);
	*/

	output.normal = input.normal;
	output.tex2dcoord = input.tex2dcoord;

	return output;
}

Texture2D gTexture : register(t0);
SamplerState gSamplerState : register(s0);

float4 PS(Vertex_Out input) : SV_Target
{
	
	float3 diffuse = saturate(input.mDiffuse);

	float3 reflection = normalize(input.mReflection);
	float3 viewDir = normalize(input.mViewDir);
	float3 specular = 0;
	if (diffuse.x > 0)
	{
		specular = saturate(dot(reflection, -viewDir));
		specular = pow(specular, 100.0f);
	}


	float3 ambient = float3(0.01f, 0.01f, 0.01f);

	float4 lightColor = float4(ambient + diffuse + specular, 0);
	float4 textureColor = gTexture.Sample(gSamplerState, input.tex2dcoord);

	return textureColor + lightColor;
	//return textureColor + float4(diffuse, 0);
	//return textureColor;
	
	//return gTexture.Sample(gSamplerState, input.tex2dcoord);
}