uniform vec2 resolution;
uniform float var;

void main(void)
{
    if(var != 0.0)
        /*gl_FragColor = gl_FragCoord / 1000.0;*/
        gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
    else
        gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
