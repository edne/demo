#include "SDL/SDL.h"
#include <GL/glew.h>

#include "example_shaders.h"


static uint8_t audio_buf[4096];

void fillaudio(void *udata, Uint8 *stream, int len)
{
    memcpy(stream, (uint8_t*)audio_buf, len);
}


GLuint init(void)
{
    GLuint prog;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
 
    unsigned short int i;
    for(i=0; i < 4096; i++)
        audio_buf[i] = i >> 2;

    SDL_AudioSpec as;
    as.freq = 22050;
    as.format = AUDIO_S16;
    as.channels = 1;
    as.samples = 4096;
    as.callback = fillaudio;
    SDL_OpenAudio(&as, NULL);
    SDL_PauseAudio(0);

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

    SDL_CloseAudio();
    SDL_Quit();
    return 0;
}
