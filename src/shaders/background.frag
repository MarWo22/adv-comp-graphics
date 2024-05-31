#version 430

// required by GLSL spec Sect 4.5.3 (though nvidia does not, amd does)
precision highp float;

layout(location = 0) out vec4 fragmentColor;
layout(binding = 6) uniform sampler2D environmentMap;
in vec2 texCoord;
uniform mat4 inv_PV;
uniform vec3 camera_pos;
uniform float environment_multiplier;
#define PI 3.14159265359

////////////////////////// Atmosphere
/// Direction from which the sun is facing
uniform vec3 sun_direction;
/// Radius of the planet.
uniform float planet_radius;
/// Radius of the atmosphere around the planet.
uniform float atmosphere_radius;
/// Atmospheric thickness for Rayleigh scattering (Hr). Assumes density as uniform.
uniform float rayleigh_atmospheric_thickness;
/// Atmospheric thickness for Mie scattering (Hm). Assumes density as uniform.
uniform float mie_atmospheric_thickness;
/// Rayleigh scattering coefficient (βr). Assumes sea level.
uniform vec3 beta_rayleigh;
/// Mie scattering coefficient (βm). Assumes sea level.
uniform vec3 beta_mie;
/// Assumes sea level. Measured in kg/m^3.
uniform float molecular_density;
/// Assumes sea level.
uniform float air_refraction_index;
/// "A general way to describe how a value fades away".
uniform float rayleigh_scale_height;
/// "A general way to describe how a value fades away".
uniform float mie_scale_height;
/// Wavelength peaks
uniform vec3 wavelength_peak;
/// Samples
uniform int num_samples;
// Light Samples
uniform int num_light_samples;
// Use texture?
uniform int use_texture;

bool solveQuadratic(float a, float b, float c, inout float x1, inout float x2)
{
	if (b == 0) {
		// Handle special case where the the two vector ray.dir and V are perpendicular
		// with V = ray.orig - sphere.centre
		if (a == 0) return false;
		x1 = 0;
		x2 = sqrt(-c / a);
		return true;
	}
	float discr = b * b - 4 * a * c;

	if (discr < 0) return false;

	float q = (b < 0.f) ? -0.5f * (b - sqrt(discr)) : -0.5f * (b + sqrt(discr));
	x1 = q / a;
	x2 = c / q;

	return true;
}

// [comment]
// A simple routine to compute the intersection of a ray with a sphere
// [/comment]
bool raySphereIntersect(const vec3 orig, const vec3 dir, const float radius, inout float t0, inout float t1)
{
	// They ray dir is normalized so A = 1
	float A = dir.x * dir.x + dir.y * dir.y + dir.z * dir.z;
	float B = 2 * (dir.x * orig.x + dir.y * orig.y + dir.z * orig.z);
	float C = orig.x * orig.x + orig.y * orig.y + orig.z * orig.z - radius * radius;

	if (!solveQuadratic(A, B, C, t0, t1)) return false;

	if (t0 > t1) {
		float tTemp = t1;
		t1 = t0;
		t0 = tTemp;
	};

	return true;
}

vec3 beta_rayleigh_computed(const float height) {
	return
	8 * pow(PI, 3) * pow(pow(air_refraction_index, 2) - 1, 2) /
	(3 * molecular_density * pow(wavelength_peak, vec3(4))) *
	exp(-height / rayleigh_scale_height);
}


vec3 incident_light(const vec3 origin, const vec3 dir, bool computeR, float tMin, float tMax, int numSamples, int numLightSamples)
{
	float t0, t1;
	if (!raySphereIntersect(origin, dir, atmosphere_radius, t0, t1) || t1 < 0) return vec3(0);
	if (t0 > tMin && t0 > 0) tMin = t0;
	if (t1 < tMax) tMax = t1;

	float segmentLength = (tMax-tMin) / numSamples;
	float tCurrent = tMin;
	vec3 sumR = vec3(0), sumM = vec3(0);
	float opticalDepthR = 0, opticalDepthM = 0;
	float mu = dot(dir, sun_direction);
	float phaseR = 3.0f / (16.0f * PI) * (1 + pow(mu, 2));
	float g = 0.76f;
	float phaseM = 3.0f / (8.0f * PI) *
	(
		(1.0f - pow(g,2)) * (1.0f + pow(mu,2)) /
		((2.0f + pow(g,2)) * pow(1.0f + pow(g,2) - 2.0f * g * mu, 1.5f))
	);

	for (int i = 0; i < numSamples; ++i) {
		vec3 samplePosition = origin + (tCurrent + segmentLength / 2) * dir;
		float height = length(samplePosition) - planet_radius;

		// Optical Depth
		float hr = exp(-height / rayleigh_atmospheric_thickness) * segmentLength;
		float hm = exp(-height / mie_atmospheric_thickness)      * segmentLength;
		opticalDepthR += hr;
		opticalDepthM += hm;


		// Optical Depth Light
		float t0Light, t1Light;
		raySphereIntersect(samplePosition, sun_direction, atmosphere_radius, t0Light, t1Light);
		float segmentLengthLight = t1Light / num_light_samples;
		float tCurrentLight = 0;
		float opticalDepthLightR = 0, opticalDepthLightM = 0;

		int j;
		for(j = 0; j < numLightSamples; ++j) {
			vec3 samplePositionLight = samplePosition + (tCurrentLight + segmentLengthLight / 2) * sun_direction;
			float heightLight = length(samplePositionLight) - planet_radius;
			if (heightLight < 0) break;
			opticalDepthLightR += exp(-heightLight / rayleigh_atmospheric_thickness) * segmentLengthLight;
			opticalDepthLightM += exp(-heightLight / mie_atmospheric_thickness)      * segmentLengthLight;
			tCurrentLight += segmentLengthLight;
		}
		if(j == numLightSamples) {
			vec3 tau =
				(computeR ? beta_rayleigh_computed(height) : beta_rayleigh) * (opticalDepthR + opticalDepthLightR) +
				beta_mie * 1.1f * (opticalDepthM + opticalDepthLightM);
			vec3 attenuation = vec3(exp(-tau.x), exp(-tau.y), exp(-tau.z));
			sumR += attenuation * hr;
			sumM += attenuation * hm;
		}
		tCurrent += segmentLength;
	}

	return (
		sumR * (computeR ? beta_rayleigh_computed(length(origin) - planet_radius) : beta_rayleigh) * phaseR +
		sumM * beta_mie * phaseM
	) * 20.0f;
}

vec4 greyscale(const vec4 color)
{
	return vec4(vec3(color.r * .299f + color.g * .587f + color.b * .114f), color.w);
}

void main()
{
	// Calculate the world-space position of this fragment on the near plane
	vec4 pixel_world_pos = inv_PV * vec4(texCoord * 2.0 - 1.0, 1.0, 1.0);
	pixel_world_pos = (1.0 / pixel_world_pos.w) * pixel_world_pos;
	// Calculate the world-space direction from the camera to that position
	vec3 dir = normalize(pixel_world_pos.xyz - camera_pos);
	// Calculate the spherical coordinates of the direction
	float theta = acos(max(-1.0f, min(1.0f, dir.y)));
	float phi = atan(dir.z, dir.x);
	if(phi < 0.0f)
	phi = phi + 2.0f * PI;
	// Use these to lookup the color in the environment map
	vec2 lookup = vec2(phi / (2.0 * PI), 1 - theta / PI);

	vec3 sky_light = incident_light(vec3(camera_pos.x, camera_pos.y + planet_radius, camera_pos.z), dir, false, 0, (1.0 / 0.0) /* Hacky way to say infinity */, num_samples, num_light_samples);

	fragmentColor = vec4(sky_light, 1) * ((use_texture == 1) ? greyscale(texture(environmentMap, lookup)) : vec4(1)) * environment_multiplier;
//	fragmentColor = environment_multiplier * texture(environmentMap, lookup) ;
}
