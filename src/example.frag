uniform vec2 resolution;
uniform float time;

void main(void)
{
    vec2 p = gl_FragCoord.xy / resolution;
    gl_FragColor = vec4(p.x, 0.5+0.5*sin(time*3.0 + p.y), 0.5+0.5*sin(time), 1.0);
}
