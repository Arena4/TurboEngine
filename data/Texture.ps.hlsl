Texture2D shaderTexture;
SamplerState SampleType;

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

float4 TexturePixelShader(PixelInputType input) : SV_Target
{
    float4 textureColor;

    textureColor = shaderTexture.Sample(SampleType, input.tex);

    return textureColor;
}