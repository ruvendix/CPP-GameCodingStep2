SamplerState samplerState : register(s0);
Texture2D tex2D : register(t0);

struct PS_Input
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

float4 DefaultPS(PS_Input input) : SV_TARGET
{
    return tex2D.Sample(samplerState, input.uv);
}