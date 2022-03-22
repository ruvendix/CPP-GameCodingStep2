struct VS_Input
{
	float2 pos : POSITION;
    float2 scale : SCALE;
};

struct VS_Output
{
    float4 pos : POSITION;
    float2 scale : SCALE;
};

VS_Output DefaultVS(VS_Input input)
{
	VS_Output output;
    output.pos = float4(input.pos, 0.0f, 1.0f);
    output.scale = input.scale;

    return output;
}