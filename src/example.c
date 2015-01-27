#include "SDL/SDL.h"
#include <GL/glew.h>
#include "example_shaders.h"


#define SAMPLERATE  44100
#define N_SAMPLES   4096

#define SAMPLE  Uint16
#define RANGE   (0xffff)

// Utilities
#define OUT(i)      ( *((SAMPLE*) stream+i) )
#define MULT(a,b)   ( (a) * (b) / RANGE )

// Oscillators
#define SAW(t, f)       ( (t * f * RANGE / SAMPLERATE) % RANGE )
#define SQUARE(t, f)    ( RANGE * (SAW(t,f) > RANGE/2) )
#define PWM(t, f, w)    ( RANGE * (SAW(t,f) > w) )  // w in [0, RANGE)

// Filters
#define LP2(s1, s2)         ( (s1 + s2)/2 )
#define LP3(s1, s2, s3)     ( (s1 + s2 + s3)/3 )

// Envelopes
#define EXP(t, p)   (RANGE >> (t >> p))  // RANGE * pow(2, -t*p)
#define PULSE(t,ts) (RANGE * (t>=0 && t<ts)
//#define LIN(t, 
//#define ASR(t,ts, a,s,r)
//#define ADSR(t,ts, a,d,s,r)

// Instruments
#define KICK(t)     MULT(EXP(t,11), PWM(t, 100, RANGE/2 - EXP(t,10)/4))


void audio_cb(void *udata, Uint8 *stream, int len)
{
    static unsigned int t = 0;
    static int tk = 0;

    int i;
    for(i=0; i<N_SAMPLES; i++)
    {
        if( !(t%(1<<15)) ) tk=0;
        OUT(i) |= KICK(tk);
        tk++;

        OUT(i) |= PWM(t, 440, (t>>4) % RANGE) & SAW(t,442) / 8;
        //OUT(i) = SAW(t, MULT(220,SAW(t,1)));

        t++;
    }
}


void audio_init(void)
{
    SDL_AudioSpec as;
    as.freq = SAMPLERATE;
    as.format = AUDIO_U16;
    as.channels = 1;
    as.samples = N_SAMPLES*2;
    as.callback = audio_cb;
    SDL_OpenAudio(&as, NULL);
    SDL_PauseAudio(0);
}


GLuint video_prog;

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
    //glRotatef(60, 1, 0, 0);

    glUniform2f(glGetUniformLocation(video_prog, "resolution"), 640, 480);
    glUniform1f(glGetUniformLocation(video_prog, "time"), (float)SDL_GetTicks()/1000);

    glBegin(GL_QUADS);
    #define L 10
    float i,j;
    for(i=-L; i<L; i++)
        for(j=-L; j<L; j++)
        {
            //glColor3f(0,0,1);
            glVertex2f(i*1.0/L, j*1.0/L);
            //glColor3f(0,1,0);
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
