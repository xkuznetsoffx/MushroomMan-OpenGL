#version 440 core  
out vec4 FragColor;  

in vec2 TexCoords;  
in vec3 FragPos;  
in vec3 Normal;  

// ��������� ��� ���������� �����  
struct DirectionLight {  
    vec3 direction;  
    vec3 ambient;  
    vec3 diffuse;  
    vec3 specular;  
};  

struct PointLight {  
    vec3 position;  
    
    vec3 ambient;  
    vec3 diffuse;  
    vec3 specular;  
    
    float constant;  
    float linear;  
    float quadratic;  
};  

struct SpotLight {  
    vec3 position;  
    vec3 direction;  
    
    vec3 ambient;  
    vec3 diffuse;  
    vec3 specular;  
    
    float constant;  
    float linear;  
    float quadratic;  
    
    float cutOff;  
    float outerCutOff;  
};  

#define NR_POINT_LIGHTS 1 

uniform sampler2D grassTexture;  
uniform vec3 viewPos;  

uniform DirectionLight dirLight;  
uniform PointLight pointLights[NR_POINT_LIGHTS];  
uniform SpotLight spotLight;  

// ������� ������� ����� ��� ������� ���� ���������  
vec3 CalcDirectionLight(DirectionLight light, vec3 normal, vec3 viewDir);  
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);  
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);  

void main()  
{  
    vec4 texColor = texture(grassTexture, TexCoords);  
    
    if(texColor.a < 0.1)  
        discard;  

    // ���������� ������ ��� ������� ���������  
    vec3 norm = normalize(Normal);  
    vec3 viewDir = normalize(viewPos - FragPos);  

    // ������ ������������� �����  
    vec3 result = CalcDirectionLight(dirLight, norm, viewDir);  

    // ������ �������� ���������� �����  
    for(int i = 0; i < NR_POINT_LIGHTS; i++)  
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);  

    // ������ ������������� �����  
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);  

    // ���������� ��������� � ��������  
    result *= texColor.rgb;  
    FragColor = vec4(result, texColor.a);  
}  

// ������� ������� ������������� �����  
vec3 CalcDirectionLight(DirectionLight light, vec3 normal, vec3 viewDir)  
{  
    vec3 lightDir = normalize(-light.direction);  
    
    // ��������� ���������  
    float diff = max(dot(normal, lightDir), 0.0);  
    
    // ���������� ���������  
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);  
    
    // ����������� ���������  
    vec3 ambient = light.ambient;  
    vec3 diffuse = light.diffuse * diff;  
    vec3 specular = light.specular * spec;  
    
    return (ambient + diffuse + specular);  
}  

// ������� ������� ��������� ��������� �����  
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)  
{  
    vec3 lightDir = normalize(light.position - fragPos);  
    
    // ��������� ���������  
    float diff = max(dot(normal, lightDir), 0.0);  
    
    // ���������� ���������  
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);  
    
    // ���������  
    float distance = length(light.position - fragPos);  
    float attenuation = 1.0 / (light.constant + light.linear * distance +   
                 light.quadratic * (distance * distance));  
    
    // ����������� ���������  
    vec3 ambient = light.ambient;  
    vec3 diffuse = light.diffuse * diff;  
    vec3 specular = light.specular * spec;  
    
    ambient *= attenuation;  
    diffuse *= attenuation;  
    specular *= attenuation;  
    
    return (ambient + diffuse + specular);  
}  

// ������� ������� ������������� �����  
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)  
{  
    vec3 lightDir = normalize(light.position - fragPos);  
    
    // ��������� ���������  
    float diff = max(dot(normal, lightDir), 0.0);  
    
    // ���������� ���������  
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);  
    
    // ���������  
    float distance = length(light.position - fragPos);  
    float attenuation = 1.0 / (light.constant + light.linear * distance +   
                 light.quadratic * (distance * distance));  
    
    // ������������� ����������  
    float theta = dot(lightDir, normalize(-light.direction));  
    float epsilon = light.cutOff - light.outerCutOff;  
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);  
    
    // ����������� ���������  
    vec3 ambient = light.ambient;  
    vec3 diffuse = light.diffuse * diff;  
    vec3 specular = light.specular * spec;  
    
    ambient *= attenuation * intensity;  
    diffuse *= attenuation * intensity;  
    specular *= attenuation * intensity;  
    
    return (ambient + diffuse + specular);  
}