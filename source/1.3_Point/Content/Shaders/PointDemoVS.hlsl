struct VSInput
{
	float4 position : POSITION;
};

struct VSOutput
{
	float4 position : SV_POSITION;
	//float  pointSize : SV_PointSize;// 셰이더 컴파일 에러
	float  pointSize : PSIZE; // 동작하지 않고 무시됌
};

VSOutput VSMain()
{
	VSOutput output;
	output.position  = float4(0, 0, 0, 1);
	output.pointSize = 100.0f; // Set the point size to 10 pixels
	return output;
}


float4 main() : SV_POSITION
{
	return float4(0, 0, 0, 1);
}