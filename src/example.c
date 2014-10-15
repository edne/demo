#include "GL/gl.h"
#include "SDL/SDL.h"
#include "fcntl.h"
static uint8_t audio_buf[4096];

void fillaudio(void *udata, Uint8 *stream, int len){
  memcpy(stream, (uint8_t*)audio_buf, len);
}

void main()
{ 
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) goto cleanclose;
  SDL_Event event;
  SDL_SetVideoMode(640,480,0,SDL_OPENGL|SDL_FULLSCREEN);
  SDL_ShowCursor(SDL_DISABLE);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-1.33,1.33,-1,1,1.5,100);
  glMatrixMode(GL_MODELVIEW);
  glEnable(GL_DEPTH_TEST);
  glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

  glLoadIdentity();
  glBegin(GL_TRIANGLES);
  glVertex3i(1,1,-10);
  glVertex3i(1,-1,-10);
  glVertex3i(-1,1,-10);
  glEnd();
  SDL_GL_SwapBuffers();
   
  // SDL audio part
  unsigned short i=0;
  for (; i < 4096; i++){
    audio_buf[i] = i >> 2;
  }
  SDL_AudioSpec as;
  as.freq = 22050;
  as.format = AUDIO_S16;
  as.channels = 1;
  as.samples = 4096;
  as.callback = fillaudio;
  if(SDL_OpenAudio(&as, NULL)<0) goto cleanclose;
  SDL_PauseAudio(0);
  do
  {
    SDL_PollEvent(&event);
    SDL_Delay(100);
  } while (event.type!=SDL_KEYDOWN);
  
cleanclose:
  SDL_CloseAudio();
  SDL_Quit();
}


