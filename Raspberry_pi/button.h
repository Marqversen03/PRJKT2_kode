#ifndef BUTTON_H
#define BUTTON_H

bool button_init();
void button_run();
void button_cleanup();

bool button_is_pressed(int gpio);

#endif
