#version 330

in vec3 fragposition;
in vec3 normal;
in vec3 color;

uniform vec3 ligthColor;
uniform vec3 ligthPosition;
uniform vec3 cameraPosition;

void main(){

    /* AMBIENT COMPONENT */
    float ambientstrength = 0.1;
    vec3 ambientcomponent = ambientstrength * ligthColor;

    /* DIFFUSIVE COMPONENT */
    vec3 ligthdirection = normalize(ligthPosition - fragposition);
    vec3 diffusecomponent = max(dot(normalize(normal),ligthdirection),0.0) * ligthColor;

    /* SPECULAR COMPONENT */
    float specularstrength = 0.5;
    vec3 viewdirection = normalize(cameraPosition-fragposition);
    vec3 reflectdirection = reflect(-ligthdirection,normalize(normal));
    vec3 specularcomponent = specularstrength * pow(max(dot(viewdirection,reflectdirection),0.0),256) * ligthColor;

    gl_FragColor = vec4((ambientcomponent + diffusecomponent + specularcomponent)*color,1.0);
}