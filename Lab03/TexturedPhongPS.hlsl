//cbuffer LightCBuf : register(b1)
#define NUMBER_OF_LIGHTS 1

struct LightConstants
{
    float3 lightPos;
    float3 ambient;
    float3 diffuseColor;
    float diffuseIntensity;
    float attConst;
    float attLin;
    float attQuad;
};

LightConstants LightCBuf : register(b1);

LightConstants LightCBuf2 : register(b2);

cbuffer ObjectCBuf : register(b0)
{
	float specularIntensity;
	float specularPower;
    float padding[2];
}


Texture2D tex;

SamplerState splr;

float4 light(LightConstants lc, float3 worldPos, float3 n)
{
	// fragment to light vector data
    const float3 vToL = lc.lightPos - worldPos;
    const float distToL = length(vToL);
    const float3 dirToL = vToL / distToL;
	// attenuation
    const float att = 1.0f / (lc.attConst + lc.attLin * distToL + lc.attQuad * (distToL * distToL));
	// diffuse intensity
    const float3 diffuse = lc.diffuseColor * lc.diffuseIntensity * att * max(0.0f, dot(dirToL, n));
	// reflected light vector
    const float3 w = n * dot(vToL, n);
    const float3 r = w * 2.0f - vToL;
	// calculate specular intensity based on angle between viewing vector and reflection vector, narrow with power function
    const float3 specular = att * (lc.diffuseColor * lc.diffuseIntensity) * specularIntensity * pow(max(0.0f, dot(normalize(-r), normalize(worldPos))), specularPower);
	// final color
    return float4(saturate(diffuse + lc.ambient + specular), 1.0f);
}

float4 main( float3 worldPos : Position,float3 n : Normal,float2 tc : Texcoord ) : SV_Target
{
    float4 result = { 0.0f, 0.0f, 0.0f, 0.0f };
    //for (uint i = 0; i < NUMBER_OF_LIGHTS; i++)
    //{
    //    result += light(LightCBuf[i], worldPos, n) * tex.Sample(splr, tc);
    //}
    result += light(LightCBuf, worldPos, n) * tex.Sample(splr, tc);
    result += light(LightCBuf2, worldPos, n) * tex.Sample(splr, tc);
    return result;
}