#version 330

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;

out vec3 resultingLigth;

uniform mat4 model;
uniform mat4 view;
uniform mat4 clip;

uniform vec3 ligthColor;
uniform vec3 ligthPos;

void main(){
    
    vec3 normal = mat3(transpose(inverse(view * model)))*vNormal;
    vec3 fragposition = vec3(view * model * vec4(vPos,1.0));
    vec3 ligthPosition = vec3(view * vec4(ligthPos, 1.0));
    
    /* AMBIENT COMPONENT */
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * ligthColor;

    /* DIFFUSIVE COMPONENT */
    vec3 ligthDirection = normalize(ligthPosition - fragposition);
    vec3 diffuse = max(dot(normalize(normal),ligthDirection),0.0) * ligthColor;

    /* SPECULAR COMPONENT */ 
    float specularStrength = 1.0;
    vec3 viewDirection = normalize(-fragposition);
    vec3 reflectionDirection = reflect(-ligthDirection,normalize(normal));
    vec3 specular = specularStrength * pow(max(dot(viewDirection,reflectionDirection),0.0),256) * ligthColor;
    
    resultingLigth = ambient + diffuse + specular;

    gl_Position = clip * view * model * vec4(vPos,1.0);
}