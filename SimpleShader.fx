
// Matrix definitions.
matrix View;
matrix Projection;
matrix Rotation;

// The color we want to paint with.
float3 paintColor;



struct VS_INPUT
{
    float4 Pos : POSITION;
    float3 Norm : NORMAL;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 Color : COLOR;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;
	output.Pos = mul( input.Pos, Rotation );
    output.Pos = mul( output.Pos, View );
    output.Pos = mul( output.Pos, Projection );

	// We illuminate with parallel light from z and y direction. 
	// We can compute this in the vertex shader as phong shading woul not make a difference in our case.

	float3 transformedNormal = mul(input.Norm, Rotation);
	output.Color = saturate(dot(transformedNormal, float3(1.0, 0.4, -0.4))) * paintColor;
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input) : SV_Target
{
   return float4(input.Color, 1.0);
}


//--------------------------------------------------------------------------------------
technique10 Render
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}


