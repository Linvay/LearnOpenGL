#version 330 core

in vec3 Normal;
in vec3 FragPosition;
in vec2 texCoord;

out vec4 FragColor;

uniform vec3 cameraPosition;
uniform sampler2D textureDiffuse0;
uniform sampler2D textureSpecular0;
uniform float shininess;

vec3 objectDiffuse = texture(textureDiffuse0, texCoord).rgb;
vec3 objectSpecular = vec3(texture(textureSpecular0, texCoord).r);


struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct SpotLight {
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};



#define MAX_POINT_LIGHTS 4
#define MAX_SPOT_LIGHTS 4

uniform DirLight dirLight;
uniform PointLight pointLight;
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform int numSpotLights = 0;



// Function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);



void main()
{
	vec3 normal = normalize(Normal);
	vec3 viewDir = normalize(cameraPosition - FragPosition);

	if (objectDiffuse == vec3(0.0f))
		objectDiffuse = vec3(1.0f);

	vec3 result = CalcDirLight(dirLight, normal, viewDir);
//	result += CalcPointLight(pointLight, normal, FragPosition, viewDir);
//	for (int i = 0; i < min(numSpotLights, MAX_SPOT_LIGHTS); i++)
//		result += CalcSpotLight(spotLights[i], normal, FragPosition, viewDir);
		
	FragColor = vec4(result, 1.0);
}



vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 ambient = light.ambient * objectDiffuse;

	vec3 lightDirection = normalize(-light.direction);
	vec3 diffuse = light.diffuse * max(dot(normal, lightDirection), 0.0) * objectDiffuse;

	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specularAmount = pow(max(dot(viewDir, reflectionDirection), 0.0), shininess);
	vec3 specular = light.specular * specularAmount * objectSpecular;



	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 frag2Light = normalize(light.position - FragPosition);

	// Attenuation
	float dist = length(light.position - FragPosition);
	float attenuation = 1.0 / (light.quadratic * dist * dist + light.linear * dist + light.constant);



	vec3 ambient = light.ambient * objectDiffuse;

	vec3 diffuse = light.diffuse * max(dot(normal, frag2Light), 0.0) * objectDiffuse;

	vec3 reflectionDirection = reflect(-frag2Light, normal);
	float specularAmount = pow(max(dot(viewDir, reflectionDirection), 0.0), shininess);
	vec3 specular = light.specular * specularAmount * objectSpecular;



	return (ambient + diffuse + specular) * attenuation;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 frag2Light = normalize(light.position - FragPosition);

	// Attenuation
	float dist = length(light.position - FragPosition);
	float attenuation = 1.0 / (light.quadratic * dist * dist + light.linear * dist + light.constant);



	vec3 ambient = light.ambient * objectDiffuse;

	vec3 diffuse = light.diffuse * max(dot(normal, frag2Light), 0.0) * objectDiffuse;

	vec3 reflectionDirection = reflect(-frag2Light, normal);
	float specularAmount = pow(max(dot(viewDir, reflectionDirection), 0.0), shininess);
	vec3 specular = light.specular * specularAmount * objectSpecular;



	// Spotlight intensity
	float angle = dot(frag2Light, normalize(-light.direction));
	float intensity = clamp((angle - light.outerCutOff) / (light.cutOff - light.outerCutOff), 0.0, 1.0);


	
	return (ambient + diffuse + specular) * intensity * attenuation;
}