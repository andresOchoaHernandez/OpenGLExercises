#version 330

struct PointLigth {    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirectionalLigth {
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

in TangentSpace
{
    vec3 fragmentTangentPosition;
    vec3 viewPosition;
    vec3 directionalLigthDirection;
    vec3 pointLigthPosition;
    vec3 flashLigthPosition;
    vec3 flashLigthDirection;
    vec2 texCoords;

} tangentSpace;

/* UNIFORMS */
uniform SpotLigth spotLigth;
uniform PointLigth pointLigth;
uniform DirectionalLigth directionalLigth;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;

/* LIGTHING FUNCTIONS */
vec3 calculateDirectionalLigth(DirectionalLigth l, vec3 ld, vec3 n);
vec3 calculatePointLigth(PointLigth l, vec3 lp, vec3 n, vec3 fp);
vec3 calculateSpotLigth(SpotLigth l, vec3 lp, vec3 ld, vec3 n, vec3 fp);

void main(){
    
    /* NORMAL ALREADY IN TANGENT SPACE */
    vec3 normal = normalize(texture(texture_normal1,tangentSpace.texCoords).rgb * 2.0 - 1.0);
    
    vec3 dLigth = calculateDirectionalLigth(directionalLigth,tangentSpace.directionalLigthDirection,normal);
    vec3 pLigth = calculatePointLigth(pointLigth,tangentSpace.pointLigthPosition,normal,tangentSpace.fragmentTangentPosition);
    vec3 sLigth = calculateSpotLigth(spotLigth,tangentSpace.flashLigthPosition,tangentSpace.flashLigthDirection,normal,tangentSpace.fragmentTangentPosition);

    gl_FragColor = vec4(dLigth + pLigth + sLigth,1.0);
}

vec3 calculateDirectionalLigth(DirectionalLigth l, vec3 ld, vec3 n)
{
    /* AMBIENT COMPONENT */
    vec3 ambient = l.ambient * texture(texture_diffuse1,tangentSpace.texCoords).rgb;

    /* DIFFUSIVE COMPONENT */
    vec3 ligthDirection = normalize(-ld);
    vec3 diffuse = l.diffuse * (max(dot(n,ligthDirection),0.0) * texture(texture_diffuse1,tangentSpace.texCoords).rgb);

    /* SPECULAR COMPONENT */
    vec3 reflectionDirection = reflect(-ligthDirection,n);
    vec3 specular = l.specular * (pow(max(dot(n,reflectionDirection),0.0),32)) * texture(texture_specular1,tangentSpace.texCoords).rgb; 

    return (ambient + diffuse + specular);
}

vec3 calculatePointLigth(PointLigth l, vec3 lp, vec3 n, vec3 fp)
{
    /* AMBIENT COMPONENT */
    vec3 ambient = l.ambient * texture(texture_diffuse1,tangentSpace.texCoords).rgb;

    /* DIFFUSIVE COMPONENT */
    vec3 ligthDirection = normalize(lp - fp);
    vec3 diffuse = l.diffuse * (max(dot(n,ligthDirection),0.0) * texture(texture_diffuse1,tangentSpace.texCoords).rgb);

    /* SPECULAR COMPONENT */
    vec3 reflectionDirection = reflect(-ligthDirection,n);
    vec3 specular = l.specular * (pow(max(dot(n,reflectionDirection),0.0),32)) * texture(texture_specular1,tangentSpace.texCoords).rgb;

    /* ATTENUATION FACTOR */
    float dist = length(lp - fp);
    float attenuation = 1.0 / (l.constant + l.linear * dist + l.quadratic * (dist * dist));

    ambient *= attenuation;diffuse *= attenuation;specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 calculateSpotLigth(SpotLigth l, vec3 lp, vec3 ld, vec3 n, vec3 fp)
{
    /* AMBIENT COMPONENT */
    vec3 ambient = l.ambient * texture(texture_diffuse1,tangentSpace.texCoords).rgb; 

    /* DIFFUSIVE COMPONENT */
    vec3 ligthDirection = normalize(lp - fp);
    vec3 diffuse = l.diffuse * (max(dot(n,ligthDirection),0.0) * texture(texture_diffuse1,tangentSpace.texCoords).rgb);

    /* SPECULAR COMPONENT */
    vec3 reflectionDirection = reflect(-ligthDirection,n); 
    vec3 specular = l.specular * (pow(max(dot(n,reflectionDirection),0.0),32.0)) * texture(texture_specular1,tangentSpace.texCoords).rgb;

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