struct PS_Input
{
    float4 pos : SV_POSITION;
};

float4 DefaultPS(PS_Input input) : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}