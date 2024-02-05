#version 440 core

out vec4 FragColor;

in vec3 crntPos;
in vec3 Normal;
in vec3 color;
in vec2 texCoord;

uniform sampler2D diffuse0;
uniform sampler2D specular0;

uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

#define POINT_LIGHT 0
#define DIRECT_LIGHT 1
#define SPOTLIGHT 2

#define TYPE_OF_LIGHT SPOTLIGHT

vec4 light()
{
#if TYPE_OF_LIGHT == POINT_LIGHT
	vec3 lightVec = lightPos - crntPos;
	float dist = length(lightVec);
	float a = 3.0f;
	float b = 0.7f;
	float inten = 1.0f / (a * dist * dist + b * dist * 1.0f);

	float ambient = 0.2f;

	vec3 normal = normalize(Normal); // It's normally normal to normalize your normals --Javidx9
	vec3 lightDirection = normalize(lightVec);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	float specularLight = 0.5f;

	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;
	
	return texture(diffuse0, texCoord) * (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten * lightColor;
#elif TYPE_OF_LIGHT == DIRECT_LIGHT
	float ambient = 0.2f;

	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f));
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	float specularLight = 0.5f;

	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;
	
	return texture(diffuse0, texCoord) * (diffuse + ambient) + texture(specular0, texCoord).r * specular * lightColor;
#elif TYPE_OF_LIGHT == SPOTLIGHT
	float outerCone = 0.9f;
	float innerCone = 0.95f;

	float ambient = 0.2f;

	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - crntPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	float specularLight = 0.5f;

	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;
	
	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
	float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

	return texture(diffuse0, texCoord) * (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten * lightColor;
#endif
}

void main()
{
	FragColor = light();
}
