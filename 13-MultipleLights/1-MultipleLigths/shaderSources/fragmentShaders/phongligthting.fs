#version 330

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirectionalLigth {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLigth
{
    float innerCutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 normal;
in vec3 fragposition;

in vec3 directionalLigthDirection;
in vec3 pointLigthPosition;

in vec3 flashLigthPosition;
in vec3 flashLigthDirection;

in vec2 texCoords;

uniform Material material;
uniform DirectionalLigth directionalLigth;
uniform PointLight pointLigth;
uniform SpotLigth spotLigth;

vec3 calculateDirectionalLigth(DirectionalLigth l, vec3 ld, vec3 n, vec3 v);
vec3 calculatePointLigth(PointLight l, vec3 lp, vec3 n, vec3 fp, vec3 v);
vec3 calculateSpotLigth(SpotLigth l, vec3 lp, vec3 ld, vec3 n, vec3 fp, vec3 v);

void main(){
    
    vec3 viewDirection = normalize(-fragposition);

    vec3 dLigth = calculateDirectionalLigth(directionalLigth,directionalLigthDirection,normal,viewDirection);
    vec3 pLigth = calculatePointLigth(pointLigth,pointLigthPosition,normal,fragposition,viewDirection);
    vec3 sLigth = calculateSpotLigth(spotLigth,flashLigthPosition,flashLigthDirection,normal,fragposition,viewDirection);

    gl_FragColor = vec4(dLigth + pLigth + sLigth,1.0);
}

vec3 calculateDirectionalLigth(DirectionalLigth l, vec3 ld, vec3 n, vec3 v)
{
    /* AMBIENT COMPONENT */
    vec3 ambient = l.ambient * texture(material.diffuse,texCoords).rgb;

    /* DIFFUSIVE COMPONENT */
    vec3 ligthDir = normalize(-ld);
    vec3 diffuse = l.diffuse * (max(dot(normalize(n),ligthDir),0.0) * texture(material.diffuse,texCoords).rgb);

    /* SPECULAR COMPONENT */
    vec3 reflectionDirection = reflect(-ligthDir,normalize(n));
    vec3 specular = l.specular * (pow(max(dot(v,reflectionDirection),0.0),material.shininess) * texture(material.specular,texCoords).rgb);

    return (ambient + diffuse + specular);
}

vec3 calculatePointLigth(PointLight l, vec3 lp, vec3 n, vec3 fp, vec3 v)
{
    /* AMBIENT COMPONENT */
    vec3 ambient = l.ambient * texture(material.diffuse,texCoords).rgb;

    /* DIFFUSIVE COMPONENT */
    vec3 ligthDirection = normalize(lp - fragposition);
    vec3 diffuse = l.diffuse * (max(dot(normalize(normal),ligthDirection),0.0) * texture(material.diffuse,texCoords).rgb);

    /* SPECULAR COMPONENT */
    vec3 viewDirection = normalize(-fragposition);
    vec3 reflectionDirection = reflect(-ligthDirection,normalize(normal));
    vec3 specular = l.specular * (pow(max(dot(v,reflectionDirection),0.0),material.shininess) * texture(material.specular,texCoords).rgb);

    /* ATTENUATION FACTOR */
    float dist = length(lp - fragposition);
    float attenuation = 1.0 / (l.constant + l.linear * dist + l.quadratic * (dist * dist));

    ambient *= attenuation;diffuse *= attenuation;specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 calculateSpotLigth(SpotLigth l, vec3 lp, vec3 ld, vec3 n, vec3 fp, vec3 v)
{
    /* AMBIENT COMPONENT */
    vec3 ambient = l.ambient * texture(material.diffuse,texCoords).rgb;

    /* DIFFUSIVE COMPONENT */
    vec3 ligthDirection = normalize(lp - fp);
    vec3 diffuse = l.diffuse * (max(dot(normalize(normal),ligthDirection),0.0) * texture(material.diffuse,texCoords).rgb);

    /* SPECULAR COMPONENT */
    vec3 viewDirection = normalize(-fp);
    vec3 reflectionDirection = reflect(-ligthDirection,normalize(normal));
    vec3 specular = l.specular * (pow(max(dot(v,reflectionDirection),0.0),material.shininess) * texture(material.specular,texCoords).rgb);

    /* SMOTHER EDGE TO THE LIGTH CONE */
    float theta = dot(ligthDirection,normalize(-ld)); 
    float epsilon   = l.innerCutOff - l.outerCutOff;
    float intensity = clamp((theta - l.outerCutOff) / epsilon, 0.0, 1.0);
    
    diffuse  *= intensity;specular *= intensity;

    /* ATTENUATION FACTOR */
    float dist = length(lp - fp);
    float attenuation = 1.0 / (l.constant + l.linear * dist + l.quadratic * (dist * dist));

    ambient *= attenuation;diffuse *= attenuation;specular *= attenuation;

    return (ambient + diffuse + specular);
}