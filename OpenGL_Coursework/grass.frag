#version 440 core  
out vec4 FragColor;  

in vec2 TexCoords;  
in vec3 FragPos;  
in vec3 Normal;  

// Структуры для источников света  
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

// Функции расчета света для каждого типа источника  
vec3 CalcDirectionLight(DirectionLight light, vec3 normal, vec3 viewDir);  
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);  
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);  

void main()  
{  
    vec4 texColor = texture(grassTexture, TexCoords);  
    
    if(texColor.a < 0.1)  
        discard;  

    // Подготовка данных для расчета освещения  
    vec3 norm = normalize(Normal);  
    vec3 viewDir = normalize(viewPos - FragPos);  

    // Расчет направленного света  
    vec3 result = CalcDirectionLight(dirLight, norm, viewDir);  

    // Расчет точечных источников света  
    for(int i = 0; i < NR_POINT_LIGHTS; i++)  
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);  

    // Расчет прожекторного света  
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);  

    // Применение освещения к текстуре  
    result *= texColor.rgb;  
    FragColor = vec4(result, texColor.a);  
}  

// Функция расчета направленного света  
vec3 CalcDirectionLight(DirectionLight light, vec3 normal, vec3 viewDir)  
{  
    vec3 lightDir = normalize(-light.direction);  
    
    // Диффузное освещение  
    float diff = max(dot(normal, lightDir), 0.0);  
    
    // Зеркальное освещение  
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);  
    
    // Объединение компонент  
    vec3 ambient = light.ambient;  
    vec3 diffuse = light.diffuse * diff;  
    vec3 specular = light.specular * spec;  
    
    return (ambient + diffuse + specular);  
}  

// Функция расчета точечного источника света  
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)  
{  
    vec3 lightDir = normalize(light.position - fragPos);  
    
    // Диффузное освещение  
    float diff = max(dot(normal, lightDir), 0.0);  
    
    // Зеркальное освещение  
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);  
    
    // Затухание  
    float distance = length(light.position - fragPos);  
    float attenuation = 1.0 / (light.constant + light.linear * distance +   
                 light.quadratic * (distance * distance));  
    
    // Объединение компонент  
    vec3 ambient = light.ambient;  
    vec3 diffuse = light.diffuse * diff;  
    vec3 specular = light.specular * spec;  
    
    ambient *= attenuation;  
    diffuse *= attenuation;  
    specular *= attenuation;  
    
    return (ambient + diffuse + specular);  
}  

// Функция расчета прожекторного света  
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)  
{  
    vec3 lightDir = normalize(light.position - fragPos);  
    
    // Диффузное освещение  
    float diff = max(dot(normal, lightDir), 0.0);  
    
    // Зеркальное освещение  
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);  
    
    // Затухание  
    float distance = length(light.position - fragPos);  
    float attenuation = 1.0 / (light.constant + light.linear * distance +   
                 light.quadratic * (distance * distance));  
    
    // Интенсивность прожектора  
    float theta = dot(lightDir, normalize(-light.direction));  
    float epsilon = light.cutOff - light.outerCutOff;  
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);  
    
    // Объединение компонент  
    vec3 ambient = light.ambient;  
    vec3 diffuse = light.diffuse * diff;  
    vec3 specular = light.specular * spec;  
    
    ambient *= attenuation * intensity;  
    diffuse *= attenuation * intensity;  
    specular *= attenuation * intensity;  
    
    return (ambient + diffuse + specular);  
}