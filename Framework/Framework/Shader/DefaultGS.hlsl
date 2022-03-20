struct GS_Input
{
	float4 pos : POSITION;
	float2 scale : SCALE;
};

struct GS_Output
{
	float4 pos : SV_POSITION;
};

[maxvertexcount(4)]
void DefaultGS(point GS_Input input[1], inout TriangleStream<GS_Output> stream)
{
    //반만큼 줄입니다.
    float2 halfScale = input[0].scale * 0.5f;
	
    //버텍스를 펼칩니다.
    float4 quadPos[4];
    quadPos[0] = float4(input[0].pos.x + halfScale.x, input[0].pos.y + halfScale.y, 0.0f, 1.0f); // 오른쪽위
    quadPos[1] = float4(input[0].pos.x + halfScale.x, input[0].pos.y - halfScale.y, 0.0f, 1.0f); // 오른쪽아래
    quadPos[2] = float4(input[0].pos.x - halfScale.x, input[0].pos.y + halfScale.y, 0.0f, 1.0f); // 왼쪽위
    quadPos[3] = float4(input[0].pos.x - halfScale.x, input[0].pos.y - halfScale.y, 0.0f, 1.0f); // 왼쪽아래
    
    [unroll(4)] // 반복문을 하나씩 펼치는 방식 (이거 없으면 경고 뜸)
    for (uint i = 0; i < 4; i++)
    {     
        GS_Output output;
        output.pos = quadPos[i];
        stream.Append(output);
    }
}