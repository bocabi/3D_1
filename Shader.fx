//
// Shader.fx
//

//	Pixel shader output structure
struct PS_OUTPUT
{
	float4 RGBColor : COLOR0;	// Pixel Color
};

// Shader outputs the pixel
PS_OUTPUT RenderScenePS()
{
	PS_OUTPUT Output;

	Output.RGBColor = 1;

	return Output;
}

// Renders scene to render target
technique RenderScene
{
	pass P0
	{
		PixelShader = compile ps_2_0 RenderScenePS(true);
	};
};