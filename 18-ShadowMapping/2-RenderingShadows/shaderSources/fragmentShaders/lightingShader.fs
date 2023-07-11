#version 330

in vec3 fragPosition;
in vec3 normal;
in vec2 texCoords;

in LigthSpace
{
    vec4 fragPositionLigthSpace;
}ligthSpace;

/* UNIFORMS */
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;

/* SHADOW */
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

float calculateShadow(vec4 fp, float bias);

void main(){

    vec3 color = texture(texture_diffuse1, texCoords).rgb;
    vec3 norm = normalize(normal);
    vec3 lightColor = vec3(1.0);
    vec3 ambient = 0.3 * lightColor;
    vec3 lightDir = normalize(lightPos - fragPosition);
    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = diff * lightColor;
    vec3 viewDir = normalize(viewPos - fragPosition);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(norm, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;
    float bias = max(0.05 * (1.0 - dot(norm, lightDir)), 0.005); 
    float shadow = calculateShadow(ligthSpace.fragPositionLigthSpace,bias);                      
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    
    gl_FragColor = vec4(lighting, 1.0);
}

float calculateShadow(vec4 fp, float bias)
{
    /* PERSPECTIVE DIVIDE */
    vec3 projCoords = fp.xyz / fp.w;
    /* TRANSFORM TO RANGE [0,1] */
    projCoords = projCoords * 0.5 + 0.5;
    
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
    
    /* PCF */
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;

    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}