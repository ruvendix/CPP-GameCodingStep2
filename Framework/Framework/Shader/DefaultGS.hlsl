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
    //�ݸ�ŭ ���Դϴ�.
    float2 halfScale = input[0].scale * 0.5f;
	
    //���ؽ��� ��Ĩ�ϴ�.
    float4 quadPos[4];
    quadPos[0] = float4(input[0].pos.x + halfScale.x, input[0].pos.y + halfScale.y, 0.0f, 1.0f); // ��������
    quadPos[1] = float4(input[0].pos.x + halfScale.x, input[0].pos.y - halfScale.y, 0.0f, 1.0f); // �����ʾƷ�
    quadPos[2] = float4(input[0].pos.x - halfScale.x, input[0].pos.y + halfScale.y, 0.0f, 1.0f); // ������
    quadPos[3] = float4(input[0].pos.x - halfScale.x, input[0].pos.y - halfScale.y, 0.0f, 1.0f); // ���ʾƷ�
    
    [unroll(4)] // �ݺ����� �ϳ��� ��ġ�� ��� (�̰� ������ ��� ��)
    for (uint i = 0; i < 4; i++)
    {     
        GS_Output output;
        output.pos = quadPos[i];
        stream.Append(output);
    }
}