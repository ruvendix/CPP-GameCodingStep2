struct GS_Input
{
	float4 pos : POSITION;
	float2 scale : SCALE;
};

struct GS_Output
{
	float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

[maxvertexcount(4)]
void DefaultGS(point GS_Input input[1], inout TriangleStream<GS_Output> stream)
{
    //반만큼 줄입니다.
    float2 halfScale = input[0].scale * 0.5f;
	
    //버텍스를 펼칩니다.
    GS_Output outputs[4];
    outputs[0].pos = float4(input[0].pos.x + halfScale.x, input[0].pos.y + halfScale.y, 0.0f, 1.0f); // 오른쪽위
    outputs[1].pos = float4(input[0].pos.x + halfScale.x, input[0].pos.y - halfScale.y, 0.0f, 1.0f); // 오른쪽아래
    outputs[2].pos = float4(input[0].pos.x - halfScale.x, input[0].pos.y + halfScale.y, 0.0f, 1.0f); // 왼쪽위
    outputs[3].pos = float4(input[0].pos.x - halfScale.x, input[0].pos.y - halfScale.y, 0.0f, 1.0f); // 왼쪽아래

    outputs[0].uv = float2(1.0f, 0.0f);
    outputs[1].uv = float2(1.0f, 1.0f);
    outputs[2].uv = float2(0.0f, 0.0f);
    outputs[3].uv = float2(0.0f, 1.0f);
    
    [unroll(4)] // 반복문을 하나씩 펼치는 방식 (이거 없으면 경고 뜸)
    for (uint i = 0; i < 4; i++)
    {     
        stream.Append(outputs[i]);
    }
}