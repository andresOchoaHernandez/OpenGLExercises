#version 330

in vec3 fragposition;
in vec3 normal;
in vec3 color;
in vec3 ligthposition;
in vec2 texcoords;

uniform vec3 ligthColor;

void main(){

    /* AMBIENT COMPONENT */
    float ambientstrength = 0.1;
    vec3 ambientcomponent = ambientstrength * ligthColor;

    /* DIFFUSIVE COMPONENT */
    vec3 ligthdirection = normalize(ligthposition - fragposition);
    vec3 diffusecomponent = max(dot(normalize(normal),ligthdirection),0.0) * ligthColor;

    /* SPECULAR COMPONENT */
    float specularstrength = 0.5;
    vec3 viewdirection = normalize(-fragposition);
    vec3 reflectdirection = reflect(-ligthdirection,normalize(normal));
    vec3 specularcomponent = specularstrength * pow(max(dot(viewdirection,reflectdirection),0.0),256) * ligthColor;

    gl_FragColor = vec4((ambientcomponent + diffusecomponent + specularcomponent)*color,1.0);
}