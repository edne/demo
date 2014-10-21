#include "SDL/SDL.h"
#include <GL/glew.h>

#include "example_shaders.h"


GLuint init(void)
{
    GLuint prog;

    SDL_Init(SDL_INIT_VIDEO);
 
    //SDL_SetVideoMode(640,480,0,SDL_OPENGL|SDL_FULLSCREEN);
    SDL_SetVideoMode(640,480,0,SDL_OPENGL);
    SDL_ShowCursor(SDL_DISABLE);

	glViewport(0, 0, 640, 480);

	glewInit();

    prog = glCreateProgram();

	GLuint sdr;

	sdr = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(sdr, 1, &VERT_SOURCE, NULL);
	glCompileShader(sdr);
	glAttachShader(prog, sdr);

	sdr = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(sdr, 1, &FRAG_SOURCE, NULL);
	glCompileShader(sdr);
	glAttachShader(prog, sdr);

	glLinkProgram(prog);
	glUseProgram(prog);

    return prog;
}

void display(GLuint prog)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    glUniform2f(glGetUniformLocation(prog, "resolution"), 640, 480);
    glUniform1f(glGetUniformLocation(prog, "time"), (float)SDL_GetTicks()/1000);

    glBegin(GL_QUADS);
    #define L 100
    float i,j;
    for(i=-L; i<L; i++)
        for(j=-L; j<L; j++)
        {
            glVertex2f(i*1.0/L, j*1.0/L);
            glVertex2f((i+1)*1.0/L, j*1.0/L);
            glVertex2f((i+1)*1.0/L, (j+1)*1.0/L);
            glVertex2f(i*1.0/L, (j+1)*1.0/L);
        }
    glEnd();

    SDL_GL_SwapBuffers();
}

int main(void)
{
    GLuint prog;
    prog = init();
    
    SDL_Event event;
    do
    {
        display(prog);

        SDL_PollEvent(&event);
    } while (event.type!=SDL_KEYDOWN);

    SDL_Quit();
    return 0;
}
