struct VS_Input
{
	float2 pos : POSITION;
};

struct VS_Output
{
	float4 pos : SV_POSITION;
};

VS_Output DefaultVS(VS_Input input)
{
	VS_Output output;
    output.pos = float4(input.pos, 0.0f, 1.0f);

    return output;
}