#version 330

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Ligth
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    /* ATTENUATION CONSTANTS */
    float constant;
    float linear;
    float quadratic;

    /* SPOTLIGTH PROPS */
    float cutoff;
    float outerCutoff;
};

in vec3 normal;
in vec3 fragposition;
in vec3 ligthPosition;
in vec3 ligthDir;
in vec2 texCoords;

uniform Material material;
uniform Ligth ligth;

void main(){

    /* AMBIENT COMPONENT */
    vec3 ambient = ligth.ambient * texture(material.diffuse,texCoords).rgb;

    /* DIFFUSIVE COMPONENT */
    vec3 ligthDirection = normalize(ligthPosition - fragposition);
    vec3 diffuse = ligth.diffuse * (max(dot(normalize(normal),ligthDirection),0.0) * texture(material.diffuse,texCoords).rgb);

    /* SPECULAR COMPONENT */
    vec3 viewDirection = normalize(-fragposition);
    vec3 reflectionDirection = reflect(-ligthDirection,normalize(normal));
    vec3 specular = ligth.specular * (pow(max(dot(viewDirection,reflectionDirection),0.0),material.shininess) * texture(material.specular,texCoords).rgb);

    /* SMOTHER EDGE TO THE LIGTH CONE */
    float theta = dot(ligthDirection,normalize(-ligthDir)); 
    float epsilon   = ligth.cutoff - ligth.outerCutoff;
    float intensity = clamp((theta - ligth.outerCutoff) / epsilon, 0.0, 1.0);
    
    diffuse  *= intensity;specular *= intensity;

    /* ATTENUATION FACTOR */
    float dist = length(ligthPosition - fragposition);
    float attenuation = 1.0 / (ligth.constant + ligth.linear * dist + ligth.quadratic * (dist * dist));

    ambient *= attenuation;diffuse *= attenuation;specular *= attenuation;

    gl_FragColor = vec4(ambient + diffuse + specular,1.0);
}