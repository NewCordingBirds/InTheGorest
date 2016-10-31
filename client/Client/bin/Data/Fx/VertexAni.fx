#define MAX		128


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

cbuffer cbBoneWorldMatrix : register(b3)
{
	row_major matrix gMtxBone[MAX] : packoffset(c0);
};

// ���� ���õ� �ּ�����
// ���� ���� ���� �� �� �ּ� Ǯ�� �غ�����.

/*
cbuffer cbTargetCameraPostion : register(b4)
{
	float4 gCameraPos : packoffset(c0);
};
// ī�޶� ��ġ ��� ���ۿ� ��Ƽ� �����;���
// ������ �ͼ� gCameraPostionForTest �� ���� �Ÿ�
// �� ������ ��� �ٲٸ� �ٷ� ���� �� �̴ϴ�.

float4 gCameraPostionForTest = float4(500.0f, 1000.0f, 500.0f, 1.0f) ;
// ���� �ӽ÷� �׽�Ʈ �غ���� �س��� ��
float4 gDirectionLightPostionForTest = float4(500.0f, 1000.0f, 500.0f, 1.0f);
// ���� ���� ForTest �̱� �ѵ� �׳� �̰� ������ �ᵵ �ǰ�
// ���� ���� ��ġ���� ��� ���ۿ� ��Ƽ� �����͵� �ǰ� ������� �ϼ���.
// �׷��� �׳� ���⿡ �����ؼ� ����ϴ� �� ���� ��
*/


struct Vertex_In
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 tex2dcoord : TEXCOORD0;
	uint4 Bones1 : BONES0;
	uint4 Bones2 : BONES1;
	float4 Weights1 : WEIGHTS0;
	float4 Weights2 : WEIGHTS1;
};

struct Vertex_Out
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float2 tex2dcoord : TEXCOORD0;

	/*
	float diffuse : TEXCOORD1;
	float dirView : TEXCOORD2;
	float dirReflection : TEXCOORD3;
	*/
};

Vertex_Out VS(Vertex_In input)
{
	Vertex_Out output = (Vertex_Out)0;


	float4 Pos = float4(input.position, 1);

	uint iBone0 = input.Bones1.r;
	uint iBone1 = input.Bones1.g;
	uint iBone2 = input.Bones1.b;
	uint iBone3 = input.Bones1.a;
	uint iBone4 = input.Bones2.r;
	uint iBone5 = input.Bones2.g;
	uint iBone6 = input.Bones2.b;
	uint iBone7 = input.Bones2.a;

	float fWeight0 = input.Weights1.r;
	float fWeight1 = input.Weights1.g;
	float fWeight2 = input.Weights1.b;
	float fWeight3 = input.Weights1.a;
	float fWeight4 = input.Weights2.r;
	float fWeight5 = input.Weights2.g;
	float fWeight6 = input.Weights2.b;
	float fWeight7 = input.Weights2.a;

	matrix m0 = gMtxBone[iBone0];
	matrix m1 = gMtxBone[iBone1];
	matrix m2 = gMtxBone[iBone2];
	matrix m3 = gMtxBone[iBone3];
	matrix m4 = gMtxBone[iBone4];
	matrix m5 = gMtxBone[iBone5];
	matrix m6 = gMtxBone[iBone6];
	matrix m7 = gMtxBone[iBone7];

	if (fWeight0 > 0) output.position += fWeight0 * mul(Pos, m0);
	if (fWeight1 > 0) output.position += fWeight1 * mul(Pos, m1);
	if (fWeight2 > 0) output.position += fWeight2 * mul(Pos, m2);
	if (fWeight3 > 0) output.position += fWeight3 * mul(Pos, m3);
	if (fWeight4 > 0) output.position += fWeight4 * mul(Pos, m4);
	if (fWeight5 > 0) output.position += fWeight5 * mul(Pos, m5);
	if (fWeight6 > 0) output.position += fWeight6 * mul(Pos, m6);
	if (fWeight7 > 0) output.position += fWeight7 * mul(Pos, m7);


	// ���� �ּ�Ǯ� ���� �ް� �Ҷ��
	// �Ʒ� �� �� �ּ� ���� ���ּž� �մϴ�.
	// (�ֳ��ϸ� ���� ��ġ�� ���� ����� �ִ� �Ŵϱ�
	// ����� �� ���Ǵ� ��ġ���� ����� �ִ� ������
	// ��� �Ǿ�� �ϱ� ������!!!)
	matrix matAll = mul(gMatWorld, gMatView);
	matAll = mul(matAll, gMatProj);
	output.position = mul(output.position, matAll);
	
	/*
	output.position = mul(output.position, gMatWorld);

	float3 dirLight = normalize(output.position.xyz - gDirectionLightPostionForTest.xyz);
	output.dirView = normalize(output.position.xyz - gCameraPostionForTest.xyz);

	output.position = mul(output.position, gMatView);
	output.position = mul(output.position, gMatProj);
	*/
	
	output.normal = mul(input.normal, (float3x3)gMatWorld);
	output.normal = normalize(output.normal); // �븻������ �ϴ� �� �����߾ �߰� ����
	
	output.tex2dcoord = input.tex2dcoord;
	/*
	output.diffuse = dot(-dirLight, output.normal);
	output.dirReflection = reflect(dirLight, output.normal);
	// reflect �Լ� �� ������ ������ ���ϼ�
	*/

	return output;
}

Texture2D gTexture : register(t0);
SamplerState gSamplerState : register(s0);

float4 PS(Vertex_Out input) : SV_Target
{
	/*
	float3 diffuse = saturate(input.diffuse);
	float3 dirReflection = normalize(input.dirReflection);
	float3 dirView = normalize(input.dirView);
	float3 specular = 0;

	if (diffuse.x > 0)
	{
		specular = saturate(dot(dirReflection, -dirView));
		specular = pow(specular, 20.0f);
	}

	float3 ambient = float3(0.1f, 0.1f, 0.1f);

	float4 light = float4(ambient + diffuse + specular, 1);
	float4 textrueColor = gTexture.Sample(gSamplerState, input.tex2dcoord);

	return textrueColor + light;
	*/
	
	return gTexture.Sample(gSamplerState, input.tex2dcoord);
}