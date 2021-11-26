//*****************************************//
//       Structures for lights data        //
//*****************************************//

// Interface for ligth, each class must define it's lightning behaviour
interface ILight
{
    float3 IlluminateAmbient(float3 vNormal);
    float3 IlluminateDiffuse(float3 vNormal);
    float3 IlluminateSpecular(float3 vNormal, int specularPower);
};

// Common base for all lights
struct BaseLightAttrib
{
    float3 Color;
    float Intensity;
};

// Ambiant light is just a simple unidirectional light
struct AmbiantLight : BaseLightAttrib, ILight {
    float3 IlluminateAmbient(float3 vNormal) { return float3(0,0,0); }
    float3 IlluminateDiffuse(float3 vNormal) { return float3(0,0,0); }
	float3 IlluminateSpecular(float3 vNormal, int specularPower) { return float3(0,0,0); }
};

// A light with ray directions
struct DirectionalLight : BaseLightAttrib, ILight {
    float3 Direction;
    float _DataAlign_;
    
    float3 IlluminateAmbient(float3 vNormal) { return float3(0,0,0); }
    float3 IlluminateDiffuse(float3 vNormal) { return float3(0,0,0); }
	float3 IlluminateSpecular(float3 vNormal, int specularPower) { return float3(0,0,0); }
};

// A light bulb representation, intensity fade toward 0 as we go further from the position
struct PointLight : BaseLightAttrib, ILight {
    float3 Position;
    float Range;
    
    float3 IlluminateAmbient(float3 vNormal) { return float3(0,0,0); }
    float3 IlluminateDiffuse(float3 vNormal) { return float3(0,0,0); }
	float3 IlluminateSpecular(float3 vNormal, int specularPower) { return float3(0,0,0); }
};


// Simmilar to a spotlight but with a restrictive angle, like a flashligth
struct SpotLight : BaseLightAttrib, ILight {
    float3 Position;
    float3 Direction;
    float Angle;
    float Range;
    
    float3 IlluminateAmbient(float3 vNormal) { return float3(0,0,0); }
    float3 IlluminateDiffuse(float3 vNormal) { return float3(0,0,0); }
	float3 IlluminateSpecular(float3 vNormal, int specularPower) { return float3(0,0,0); }
};

