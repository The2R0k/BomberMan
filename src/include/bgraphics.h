#ifndef _BGRAPHICS_H_
#define _BGRAPHICS_H_

struct GraphSources {
  SDL_Window *win; // Window for graphics
  SDL_Renderer *ren; // Render
  SDL_Surface *bmp; // Bitmap image
  SDL_Texture *wall, *empty, *box, *bomb, *interface, 
  *player_one, *player_two, *player_three, *player_four, *fire; // Sources  
};

/* prototypes of graphic functions */
void ShowCurrentBar(struct Field, struct Dklb, struct GraphSources);
struct GraphSources GraphicsInit(); 
struct GraphSources CleanGraph(struct GraphSources);

/* graphics constants */
#define FIELDSIZE 19 // field size
#define PCSIZE 34 // bitmap block size

#endif /* _BGRAPHICS_H_ */