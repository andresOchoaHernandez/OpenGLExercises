#version 330

in vec4 color;
in vec3 fragPosition;
in vec3 normal;

uniform vec3 ligthColor;
uniform float ambientStrength;
uniform vec3 ligthPosition;
uniform vec3 camPosition;

void main(){

    vec4 ligthCol = vec4(ligthColor,1.0);

    vec4 ambient = ambientStrength * ligthCol;

    vec3 ligthDirection = normalize(ligthPosition - fragPosition);

    vec3 norm = normalize(normal);
    vec3 diffuseComponent = max(dot(norm,ligthDirection),0.0) * ligthCol.xyz;

    vec3 viewDirection = normalize(camPosition - fragPosition);
    vec3 reflectDirection = reflect(-ligthDirection,norm);
    
    float specularStrenght = 1.0;
    vec3 specularComponent = specularStrenght * pow(max(dot(viewDirection,reflectDirection),0.0),256) * ligthCol.xyz;

    gl_FragColor =  (vec4(diffuseComponent,1.0) + vec4(specularComponent,1.0) + ambient) * color;
}