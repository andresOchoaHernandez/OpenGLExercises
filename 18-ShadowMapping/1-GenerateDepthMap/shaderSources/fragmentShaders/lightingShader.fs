#version 330

struct DirectionalLigth {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in TangentSpace
{
    vec3 fragmentTangentPosition;
    vec3 viewPosition;
    vec3 directionalLigthDirection;
    vec2 texCoords;

} tangentSpace;

/* UNIFORMS */
uniform DirectionalLigth directionalLigth;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;

/* LIGTHING FUNCTIONS */
vec3 calculateDirectionalLigth(DirectionalLigth l, vec3 ld, vec3 n);

void main(){
    
    /* NORMAL ALREADY IN TANGENT SPACE */
    vec3 normal = normalize(texture(texture_normal1,tangentSpace.texCoords).rgb * 2.0 - 1.0);
    
    vec3 dLigth = calculateDirectionalLigth(directionalLigth,tangentSpace.directionalLigthDirection,normal);

    gl_FragColor = vec4(dLigth,1.0);
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