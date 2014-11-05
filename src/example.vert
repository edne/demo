uniform float time;

void main(void)
{
    vec4 v = vec4(gl_Vertex);

    gl_Position = gl_ModelViewProjectionMatrix * v;
    gl_FrontColor = gl_Color;
}
