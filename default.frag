#version 330 core

in vec3 Normal;
in vec3 FragPosition;
in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D textureDiffuse0;
uniform sampler2D textureDiffuse1;
uniform sampler2D textureDiffuse2;
uniform sampler2D textureDiffuse3;
uniform sampler2D textureDiffuse4;
uniform sampler2D textureSpecular0;
uniform sampler2D textureSpecular1;
uniform sampler2D textureSpecular2;
uniform sampler2D textureSpecular3;
uniform sampler2D textureSpecular4;

uniform vec4 lightColor;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;
uniform vec3 spotLightPosition;
uniform vec3 spotLightDirection;



vec4 pointLight()
{
	vec3 lightVec = lightPosition - FragPosition;

	// Attenuation
	float dist = length(lightVec);
	float a = 0.032;
	float b = 0.09;
	float c = 1.0;
	// 1 over quadratic equation, with quadratic, linear and constant components
	float attenuation = 1.0 / (a * dist * dist + b * dist + c);



	float ambient = 0.2;

	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightVec);
	float diffuse = max(dot(normal, lightDirection), 0.0);

	float specularStrength = 0.5;
	vec3 viewDirection = normalize(cameraPosition - FragPosition);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specularAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0), 16);
	float specular = specularStrength * specularAmount;


	
	return (texture(textureDiffuse0, texCoord) * (ambient + diffuse) 
		+ texture(textureSpecular0, texCoord).r * specular) * lightColor * attenuation;
}



vec4 directionalLight()
{
	float ambient = 0.2;

	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(vec3(1.0, 1.0, 0.0));
	float diffuse = max(dot(normal, lightDirection), 0.0);

	float specularStrength = 0.5;
	vec3 viewDirection = normalize(cameraPosition - FragPosition);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specularAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0), 64);
	float specular = specularStrength * specularAmount;

	

	return (texture(textureDiffuse0, texCoord) * (ambient + diffuse) 
		+ texture(textureSpecular0, texCoord).r * specular) * lightColor;
}

vec4 spotLight()
{
	float innerConeAngle = 0.95;
	float outerConeAngle = 0.90;



	vec3 lightVec = spotLightPosition - FragPosition;

	// Attenuation
	float dist = length(lightVec);
	float a = 0.032;
	float b = 0.09;
	float c = 1.0;
	// 1 over quadratic equation, with quadratic, linear and constant components
	float attenuation = 1.0 / (a * dist * dist + b * dist + c);



	float ambient = 0.2;

	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightVec);
	float diffuse = max(dot(normal, lightDirection), 0.0);

	float specularStrength = 0.5;
	vec3 viewDirection = normalize(cameraPosition - FragPosition);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specularAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0), 64);
	float specular = specularStrength * specularAmount;



	float angle = dot(viewDirection, normalize(-spotLightDirection));
	float intensity = clamp((angle - outerConeAngle) / (innerConeAngle - outerConeAngle), 0.0, 1.0);


	
	return (texture(textureDiffuse0, texCoord) * (ambient + diffuse * intensity) 
		+ texture(textureSpecular0, texCoord) * specular * intensity) * lightColor * attenuation;
}

void main()
{
	FragColor = directionalLight();
}