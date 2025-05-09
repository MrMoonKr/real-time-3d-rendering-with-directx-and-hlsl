struct VSOutput
{
	float4 position : SV_POSITION;
	//float  pointSize : SV_PointSize; // 셰이더 컴파일 에러
	float  pointSize : PSIZE; // 동작하지 않고 무시됌
};

float4 PSMain( VSOutput input) : SV_TARGET
{
	// The pixel shader does not use the point size, but it is passed through for demonstration purposes.
	// You can use input.pointSize if you need to access the point size in the pixel shader.
	
	// Output a solid color 
	return float4(1.0f, 0.0f, 0.0f, 1.0f);
}



float4 main() : SV_TARGET
{
	return float4(1.0f, 1.0f, 0.0f, 1.0f);
}