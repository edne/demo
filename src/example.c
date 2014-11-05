#include "SDL/SDL.h"
#include <GL/glew.h>
#include "example_shaders.h"

#define TWOPI 6.283185307179586476925287
#define FREQ 22050
#define SAMPLERATE FREQ
#define MAXAMP 24576
#define FILTERLEN 10
GLuint video_prog;

//float notes[] = {440,587.33,739.99,880};
const float notes[] = {110,146.83,185,220};

float filter(float sample){
	static float filterbank[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	char i;
	static float acc = 0;
	acc = acc + sample - filterbank[FILTERLEN-1];
	for (i=FILTERLEN-1; i>0; i--){
		filterbank[i] = filterbank[i-1];
	}
	filterbank[0] = sample;
	return acc/FILTERLEN;
}

void audio_cb(void *udata, Uint8 *stream, int len)
{
  unsigned short int i;
	static char filterlen = FILTERLEN;
	static unsigned char note=0;
//  static unsigned short int p=0;
	static float p = -1;
  static unsigned short int n=0;
	float sawIncr= (2 * notes[note]) / SAMPLERATE;
	static short int a=MAXAMP;
  for(i=0; i < len/2; i++){
	  a = (a+MAXAMP-2)%MAXAMP;
  	if (a==0){
			note=(note+1)%4;
			sawIncr= (2 * notes[note]) / SAMPLERATE;
		} 
		if (a % (MAXAMP/10) == 0){
			filterlen--;
			if (filterlen <= 0) filterlen = FILTERLEN;
		} 
    *((unsigned short*) stream+i) = filter(p) * a;
		p += sawIncr;
		if (p >= 1) p = -1;
    //*((unsigned short*)stream+i) = sin((TWOPI/SAMPLERATE)*notes[note]*p)*a;
		//p++;
	}
}


void audio_init(void)
{

    SDL_AudioSpec as;
    as.freq = FREQ;
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
