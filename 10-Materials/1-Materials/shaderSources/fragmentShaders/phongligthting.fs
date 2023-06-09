#version 330

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Ligth
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 normal;
in vec3 fragposition;
in vec3 ligthPosition;

uniform Material material;
uniform Ligth ligth;

void main(){
    
    /* AMBIENT COMPONENT */
    vec3 ambient = ligth.ambient * material.ambient;

    /* DIFFUSIVE COMPONENT */
    vec3 ligthDirection = normalize(ligthPosition - fragposition);
    vec3 diffuse = ligth.diffuse * (max(dot(normalize(normal),ligthDirection),0.0) * material.diffuse);

    /* SPECULAR COMPONENT */
    vec3 viewDirection = normalize(-fragposition);
    vec3 reflectionDirection = reflect(-ligthDirection,normalize(normal));
    vec3 specular = ligth.specular * (pow(max(dot(viewDirection,reflectionDirection),0.0),material.shininess) * material.specular);

    gl_FragColor = vec4(ambient + diffuse + specular,1.0);
}