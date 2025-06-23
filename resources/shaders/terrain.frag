// Fragment Shader (terrain.frag)

in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

struct Material {
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;


out vec4 FragColor;

void main() {
    // Normalize interpolated normal
    vec3 norm = normalize(Normal);

    // Light direction
    vec3 lightDir = normalize(lightPos - FragPos);

    // Diffuse shading (Lambert)
    float diff = max(dot(norm, lightDir), 0.0);

    // View direction
    vec3 viewDir = normalize(viewPos - FragPos);

    // Reflect direction for specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient  = 0.1 * lightColor * material.diffuse;
    vec3 diffuse  = diff * lightColor * material.diffuse;
    vec3 specular = spec * lightColor * material.specular;

    vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);
}
