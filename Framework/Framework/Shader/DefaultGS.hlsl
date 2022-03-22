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
    //�ݸ�ŭ ���Դϴ�.
    float2 halfScale = input[0].scale * 0.5f;
	
    //���ؽ��� ��Ĩ�ϴ�.
    GS_Output outputs[4];
    outputs[0].pos = float4(input[0].pos.x + halfScale.x, input[0].pos.y + halfScale.y, 0.0f, 1.0f); // ��������
    outputs[1].pos = float4(input[0].pos.x + halfScale.x, input[0].pos.y - halfScale.y, 0.0f, 1.0f); // �����ʾƷ�
    outputs[2].pos = float4(input[0].pos.x - halfScale.x, input[0].pos.y + halfScale.y, 0.0f, 1.0f); // ������
    outputs[3].pos = float4(input[0].pos.x - halfScale.x, input[0].pos.y - halfScale.y, 0.0f, 1.0f); // ���ʾƷ�

    outputs[0].uv = float2(1.0f, 0.0f);
    outputs[1].uv = float2(1.0f, 1.0f);
    outputs[2].uv = float2(0.0f, 0.0f);
    outputs[3].uv = float2(0.0f, 1.0f);
    
    [unroll(4)] // �ݺ����� �ϳ��� ��ġ�� ��� (�̰� ������ ��� ��)
    for (uint i = 0; i < 4; i++)
    {     
        stream.Append(outputs[i]);
    }
}