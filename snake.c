#include <stdlib.h>
#include <curses.h>
#include <unistd.h>
#include <stdbool.h>

typedef struct {
  int x;
  int y;
} vec2;

int score = 0;
vec2 segments[256]; 
int screen_width = 40;
int screen_height = 20;

bool check_collision(vec2 head);

void draw_border(WINDOW *win);

int main(){


  // initialize screen
  WINDOW *win = initscr();
  // take player input
  keypad(win, true);
  nodelay(win, true);
  curs_set(0);


  // snake
  vec2 head = { screen_width / 2, screen_height / 2 };
  vec2 dir = { 1, 0};

  // berry
  vec2 berry = { rand() % (screen_width - 2) + 1, rand() % (screen_height - 2) + 1};
  
  bool game_over = false;

  while (!game_over) {

    int pressed = wgetch(win);
    if (pressed == KEY_LEFT){
      if(dir.x == 1) continue;
      dir.x = -1;
      dir.y = 0;
      mvaddch(head.y, head.x * 2, '<');
    }
    if (pressed == KEY_RIGHT){
      if(dir.x == -1) continue;
      dir.x = 1;
      dir.y = 0;
      mvaddch(head.y, head.x * 2, '>');
    }
    if (pressed == KEY_UP){
      if (dir.y == 1) continue;
      dir.x = 0;
      dir.y = -1;
      mvaddch(head.y, head.x * 2, '^');
    }
    if (pressed == KEY_DOWN){
      if (dir.y == -1) continue;
      dir.x = 0;
      dir.y = 1;
      mvaddch(head.y, head.x * 2, 'V');
    }
    if (pressed == '\e'){
      break;
    }
    // Update
    // Update segments
    for (int i = score; i > 0; i--){
      segments[i] = segments[i - 1];
    }

    segments[0] = head;

    head.x += dir.x;
    head.y += dir.y;
    if (head.x == berry.x && head.y == berry.y){
      score += 1;

      berry.x = rand() % (screen_width - 2) + 1;
      berry.y = rand() % (screen_height - 2) + 1;
    }
    // ----- draw ------
    erase();
    draw_border(win);
    mvaddch(berry.y, berry.x * 2, '@');


    // Snake body
    for (int i = 0; i < score; i++) {
      mvaddch(segments[i].y, segments[i].x * 2, 'o'); 
    }
    // Snake head
    // key down
    while (dir.x == 0 && dir.y == 1){
       mvaddch(head.y, head.x * 2, 'v');
       break;
    }
    // key up
    while (dir.x == 0 && dir.y == -1){
      mvaddch(head.y, head.x * 2, '^');
      break;
    } 
    // KEY_LEFT
    while (dir.x == 1 && dir.y == 0){
      mvaddch(head.y, head.x * 2, '>');
      break;
    }
    // KEY_RIGHT
    while (dir.x == -1 && dir.y == 0){
      mvaddch(head.y, head.x * 2, '<');
      break;
    }


    // collision 
    //
    if (check_collision(head)){
      game_over = true;
      break;
      }
    // Display score
    mvprintw(0, 0, "Score : %d", score);
    refresh();
    usleep(125000);
  }
  // Game over
  erase();
  mvprintw(screen_height, (screen_width - 10) / 2, "GAME OVER");
  mvprintw(screen_height / 2, (screen_width - 15) / 2, "YOUR SCORE = %d", score);
  refresh();
  nodelay(win, false);
  getchar();
  endwin();

return 0;
}


bool check_collision(vec2 head){
  //check wall collision
  if(head.x <= 0 || head.x >= screen_width -1 || head.y <= 0 || head.y >= screen_height - 1){
    return true;
  }
  // check self collision
  for(int i = 0; i < score; i++){
    if (head.x == segments[i].x && head.y == segments[i].y){
      return true;
    }
  }
return false;
}


void draw_border(WINDOW *win){
  // draw top and bottom border
  for(int x = 0; x < screen_width; x++){
    mvaddch(0, x * 2, '-');
    mvaddch(screen_height - 1, x * 2, '-');
  }
  //draw left and right border
  for(int y = 1; y < screen_height - 1; y++){
    mvaddch(y, 0, '|');
    mvaddch(y, (screen_width - 1) * 2, '|');
  }
  // draw corners
  mvaddch(0, 0, '+');
  mvaddch(0, (screen_width - 1) * 2, '+');
  mvaddch(screen_height -1, 0, '+');
  mvaddch(screen_height - 1, (screen_width - 1) * 2, '+');
}
