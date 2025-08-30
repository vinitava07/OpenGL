#version 330 core

#define NR_POINT_LIGHTS 4

struct Material {
	sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_diffuse3;
    sampler2D texture_specular1;
    sampler2D texture_specular2;
	float shininess;
};

struct SpotLight {
    vec3 position;
	vec3 direction;

    float cutOff;
    float outerCutOff;

    vec3 ambient;
	vec3 diffuse;
	vec3 specular;

    float constant;
    float linear;
    float quadratic;

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

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;

in VS_OUT {

    vec3 fragPos;
    vec3 normal;
    vec2 TexCoords;

}fs_in;

uniform samplerCube skybox;
uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform bool blinn;

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir);  
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);  
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

float near = 0.1; 
float far  = 100.0; 

float linearizeDepth(float depth){

    float z = depth * 2.0 - 1.0; //back to ndc
    return (2.0 * near * far) / (far + near - z * (far - near));

}


void main()
{
    
    vec3 color = texture(material.texture_diffuse1, fs_in.TexCoords).rgb;

    vec3 ambient = 0.05 * color;

    vec3 lightDir = normalize(lightPos - fs_in.fragPos);
    vec3 normal = normalize(fs_in.normal);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;

    vec3 viewDir = normalize(viewPos - fs_in.fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;

    if(blinn) {

        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    }
    else {

        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
    }

    vec3 specular = vec3(0.3) * spec;

    FragColor = vec4(ambient + diffuse + specular, 1.0f);


}

//vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
//
//    vec3 lightDir = normalize(-light.direction);
//
//    float diff = max(dot(normal, lightDir), 0.0);
//
//    vec3 reflectDir = reflect(-lightDir, normal);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//
//    vec3 ambient = light.ambient * texture(material.texture_diffuse1, TexCoords).rgb;
//    vec3 diffuse = light.diffuse * diff * texture(material.texture_diffuse1, TexCoords).rgb;
//    vec3 specular = light.specular * spec * texture(material.texture_specular1, TexCoords).rgb;
//
//    return (ambient + diffuse + specular);
//}
//
//vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
//
//    vec3 lightDir = normalize(light.position - fragPos);
//
//    float diff = max(dot(normal, lightDir), 0.0);
//
//    vec3 reflectDir = reflect(-lightDir, normal);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//
//    float distance = length(light.position - fragPos);
//    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
//
//    vec3 ambient = light.ambient * texture(material.texture_diffuse1, TexCoords).rgb;
//    vec3 diffuse = light.diffuse * diff * texture(material.texture_diffuse1, TexCoords).rgb;
//    vec3 specular = light.specular * spec * texture(material.texture_specular1, TexCoords).rgb;
//
//    ambient *= attenuation;
//    diffuse *= attenuation;
//    specular *= attenuation;
//
//    return (ambient + diffuse + specular);
//}
//
//vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
//
//    vec3 lightDir = normalize(light.position - fragPos);
//
//    float diff = max(dot(normal, lightDir), 0.0);
//
//    vec3 reflectDir = reflect(-lightDir, normal);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//
//    float theta = dot(lightDir, normalize(-light.direction));
//    float epsilon   = light.cutOff - light.outerCutOff;
//    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0); 
//
//    vec3 ambient = light.ambient * texture(material.texture_diffuse1, TexCoords).rgb;
//    vec3 diffuse =  light.diffuse * diff * texture(material.texture_diffuse1, TexCoords).rgb;
//    vec3 specular = light.specular * spec * texture(material.texture_specular1, TexCoords).rgb;
//
//    float distance = length(light.position - fragPos);
//    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
//
//    ambient  *= intensity;
//    diffuse  *= intensity;
//    specular *= intensity;
//
//    ambient *= attenuation;
//    diffuse *= attenuation;
//    specular *= attenuation;
//    
//    return (ambient + diffuse + specular);
//}