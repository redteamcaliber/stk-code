uniform sampler2D Albedo;

in vec2 uv;
in vec4 color;
out vec4 FragColor;

vec3 getLightFactor(float specMapValue);

void main(void)
{
    vec4 col = texture(Albedo, uv);
    col.xyz *= pow(color.xyz, vec3(2.2));
    vec3 LightFactor = getLightFactor(1.);
    FragColor = vec4(col.xyz * LightFactor, 1.);
}
