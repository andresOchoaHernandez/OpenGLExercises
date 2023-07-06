#version 330 core

in vec3 normal;
in vec3 fragposition;
in vec3 ligthPosition;

void main()
{   
    vec3 objectColor = vec3(1.0,0.5,0.2);
    vec3 ligthColor  = vec3(1.0,1.0,1.0);

    /* AMBIENT COMPONENT */
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * ligthColor;

    /* DIFFUSIVE COMPONENT */
    vec3 ligthDirection = normalize(ligthPosition - fragposition);
    vec3 diffuse = max(dot(normalize(normal),ligthDirection),0.0) * ligthColor;

    /* SPECULAR COMPONENT */
    float specularStrength = 0.5;
    vec3 viewDirection = normalize(-fragposition);
    vec3 reflectionDirection = reflect(-ligthDirection,normalize(normal));
    vec3 specular = specularStrength * pow(max(dot(viewDirection,reflectionDirection),0.0),256) * ligthColor;

    gl_FragColor = vec4((ambient + diffuse + specular)*objectColor,1.0);
}