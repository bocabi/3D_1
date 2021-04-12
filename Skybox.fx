struct VS_OUTPUT
{
	float4 mPosition : POSITION;
	float2 mUv : TEXCOORD0;
};

struct VS_INPUT
{
	float4 mPosition : POSITION;
	float2 mUv : TEXCOORD0;
};

float4x4 gWorldMatrix;
float4x4 gViewMatrix;
float4x4 gProjMatrix;


VS_OUTPUT vs_main(VS_INPUT Input)
{
	VS_OUTPUT Output;

	Output.mPosition = mul(Input.mPosition, gWorldMatrix);
	Output.mPosition = mul(Output.mPosition, gViewMatrix);
	Output.mPosition = mul(Output.mPosition, gProjMatrix);

	Output.mUv = Input.mUv;
	
	return Output;
}

struct PS_INPUT
{
	float2 mUv: TEXCOORD0;
};

texture gMap;
sampler gMapSampler = sampler_state
{
	Texture = gMap;
};

float4 ps_main(PS_INPUT Input) : COLOR
{
	float4 albedo = tex2D(gMapSampler, Input.mUv);
	return albedo;
}


technique SkyBox
{
	pass Pass_0
	{
		ZEnable = false;
		CullMode = NONE;
		LightEnable[0] = false;

		VertexShader = compile vs_2_0 vs_main();
		PixelShader = compile ps_2_0 ps_main();
	}
};
