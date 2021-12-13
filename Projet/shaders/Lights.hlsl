#ifndef LIGHT_H
#define LIGHT_H

#include "Util.hlsl"
#include "Material.hlsl"

// Light types definition
#define LightType_Spot 0
#define LightType_Point 1
#define LightType_Directional 2

// Generic Representation of a light
struct Light
{
    int Type;
    float3 Color;
    float3 Position;
    float3 Direction;
    float Range;
    float SpotAngle;
    float Intensity;

    FILL(3);
};

// Calculate the phong value for a spot light
float3 CalcSpotPhong(float3 N, float3 V, float3 Pos, Material mat, Light light)
{
    float3 color = WHITE;

    float3 L = normalize(-light.Direction);
    
    float3 cone = dot(L, normalize(-L));

    // Valeur de la composante diffuse 
    float diff = dot(N, L);
    if (diff <= 0.0001f)
        return BLACK;

    // specular shading
    float3 reflectDir = reflect(L, N);
    float spec = pow(max(dot(V, reflectDir), 0.0), mat.Ambient);

    // combine results
    float3 ambient = light.Color * mat.Roughness.xyz;
    float3 diffuse = light.Color * diff * mat.Roughness.xyz;
    float3 specular = light.Color * spec * mat.Specular.xyz;

    return (ambient + diffuse + specular) * cone;
}

// Calculate the phong value for a point light
float3 CalcPointPhong(float3 N, float3 V, float3 Pos, Material mat, Light light)
{
    float3 color = WHITE;

    float3 L = light.Position - Pos;
    float distance = length(L);
    L /= distance;

    // Valeur de la composante diffuse 
    float diff = dot(N, L);
    
    if (diff <= 0.0001f || distance > light.Range)
        return BLACK;

    // specular shading
    float3 reflectDir = reflect(L, N);
    float spec = pow(max(dot(V, reflectDir), 0.0), mat.Ambient);

    // attenuation
    // calculate basic attenuation
    float attenuation = light.Intensity / pow((distance / light.Range) + 1, 2.f);
    attenuation = max(attenuation, 0);

    // combine results
    float3 ambient = light.Color * mat.Roughness.xyz;
    float3 diffuse = light.Color * diff * mat.Roughness.xyz;
    float3 specular = light.Color * spec * mat.Specular.xyz;

    return (ambient + diffuse + specular) * attenuation;
}

// Calculate the phong value for a directional light
float3 CalcDirectionalPhong(float3 N, float3 V, float3 Pos, Material mat, Light light)
{
    float3 color = WHITE;

    float3 L = normalize(-light.Direction);

    // Valeur de la composante diffuse 
    float diff = dot(N, L);
    if (diff <= 0.0001f)
        return BLACK;

    // specular shading
    float3 reflectDir = reflect(L, N);
    float spec = pow(max(dot(V, reflectDir), 0.0), mat.Ambient);

    // combine results
    float3 ambient = light.Color * mat.Roughness.xyz;
    float3 diffuse = light.Color * diff * mat.Roughness.xyz;
    float3 specular = light.Color * spec * mat.Specular.xyz;

    return (ambient + diffuse + specular);
}

// Compute the phong value given by a light
float3 CalcPhong(float3 N, float3 V, float3 Pos, Material mat, Light light)
{
    switch (light.Type)
    {
        case LightType_Spot:
            return CalcSpotPhong(N, V, Pos, mat, light);
        case LightType_Point:
            return CalcPointPhong(N, V, Pos, mat, light);
        case LightType_Directional:
            return CalcDirectionalPhong(N, V, Pos, mat, light);
        default:
            return BLACK;
    }
}

#endif