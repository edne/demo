uniform vec2 resolution;
uniform float time;

void main(void)
{
    vec2 p = gl_FragCoord.xy / resolution;
    gl_FragColor = vec4(
            0.0,
            (0.5+0.5*sin(time*10.0 + p.y)) * (0.5 + 0.5*sin(p.x*2.0 + 5.0*time)),
            (0.5+0.5*sin(time)) * sin(time*2.0) * (0.5 + 0.5*sin(p.x*2.0 + 5.0*time)),
            1.0);
    /*gl_FragColor.b = gl_Color.b;*/
}
