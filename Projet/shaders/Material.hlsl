#ifndef MATERIAL_H
#define MATERIAL_H

// Representation of a material
struct Material
{
    float4 Ambient;
    float4 Roughness;
    float4 Specular;
    float Intensity;
    float3 _FILL_;
};

#endif // MATERIAL_H
