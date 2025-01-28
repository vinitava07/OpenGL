#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light {
	//vec3 position; nao eh necessario quando usa luz direcional
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

};

out vec4 FragColor;

in vec3 normal;
in vec3 fragPos;  
in vec2 texCoords;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{

    //vec3 lightDir = normalize(light.position - fragPos);
    vec3 lightDir = normalize(-light.direction);
 // ambient
    vec3 ambient = light.ambient * texture(material.diffuse, texCoords).rgb;
 
 // diffuse
    vec3 norm = normalize(normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse =  light.diffuse * diff *texture(material.diffuse, texCoords).rgb;
 
 // specular
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, texCoords).rgb;
 
    vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0f);

}