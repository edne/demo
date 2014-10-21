//#include <GL/glew.h>
//#include <GL/gl.h>
//#include <SDL/SDL.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include "example_shaders.h"


GLuint init(void)
{
    GLuint prog;

    //SDL_SetVideoMode(640,480,0,SDL_OPENGL|SDL_FULLSCREEN);
    SDL_SetVideoMode(640,480,0,SDL_OPENGL);
    SDL_ShowCursor(SDL_DISABLE);

	glViewport(0, 0, 640, 480);
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

    //glUniform2f(glGetUniformLocation(prog, "resolution"), 640.0, 480.0);
    glUniform1f(glGetUniformLocation(prog, "var"), 1.0);

    glBegin(GL_QUADS);
    #define L 1
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
