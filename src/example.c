#include "SDL/SDL.h"
#include <GL/glew.h>

#include "example_shaders.h"


static uint8_t audio_buf[4096];
GLuint video_prog;


void audio_cb(void *udata, Uint8 *stream, int len)
{
    memcpy(stream, (uint8_t*)audio_buf, len);
}


void audio_init(void)
{
    unsigned short int i;
    for(i=0; i < 4096; i++)
        audio_buf[i] = i >> 2;

    SDL_AudioSpec as;
    as.freq = 22050;
    as.format = AUDIO_S16;
    as.channels = 1;
    as.samples = 4096;
    as.callback = audio_cb;
    SDL_OpenAudio(&as, NULL);
    SDL_PauseAudio(0);
}


void video_init(void)
{
    //SDL_SetVideoMode(640,480,0,SDL_OPENGL|SDL_FULLSCREEN);
    SDL_SetVideoMode(640,480,0,SDL_OPENGL);
    SDL_ShowCursor(SDL_DISABLE);

	glViewport(0, 0, 640, 480);

	glewInit();

    video_prog = glCreateProgram();

	GLuint sdr;

	sdr = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(sdr, 1, &VERT_SOURCE, NULL);
	glCompileShader(sdr);
	glAttachShader(video_prog, sdr);

	sdr = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(sdr, 1, &FRAG_SOURCE, NULL);
	glCompileShader(sdr);
	glAttachShader(video_prog, sdr);

	glLinkProgram(video_prog);
	glUseProgram(video_prog);
}


void init(void)
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    audio_init();
    video_init();
}


void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    glUniform2f(glGetUniformLocation(video_prog, "resolution"), 640, 480);
    glUniform1f(glGetUniformLocation(video_prog, "time"), (float)SDL_GetTicks()/1000);

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
    init();
    
    SDL_Event event;
    do
    {
        display();

        SDL_PollEvent(&event);
    } while (event.type!=SDL_KEYDOWN);

    SDL_CloseAudio();
    SDL_Quit();
    return 0;
}
