#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>
#include <X11/keysym.h>
#include "uwm.h"

xcb_get_geometry_reply_t *geom;
int values[3];
enum direction {UP, DOWN, LEFT, RIGHT};
void move_window(xcb_drawable_t win, xcb_connection_t *c,enum direction d);
int main(){
	//helop
	xcb_screen_t *screen;
	xcb_connection_t *connection = xcb_connect(NULL, NULL);
	xcb_setup_t const *setup= xcb_get_setup(connection);
	screen = (xcb_setup_roots_iterator(setup)).data;
	xcb_drawable_t root = screen->root;
	xcb_drawable_t win;
	xcb_generic_event_t *event;
	xcb_key_press_event_t *keypress;
	xcb_key_symbols_t *keysyms = xcb_key_symbols_alloc(connection);
	xcb_keycode_t keycode;
	const xcb_keycode_t keycode_j = (xcb_key_symbols_get_keycode(keysyms, XK_j))[0];
	const xcb_keycode_t keycode_k = (xcb_key_symbols_get_keycode(keysyms, XK_k))[0];
	const xcb_keycode_t keycode_h = (xcb_key_symbols_get_keycode(keysyms, XK_h))[0];
	const xcb_keycode_t keycode_l = (xcb_key_symbols_get_keycode(keysyms, XK_l))[0];
	xcb_grab_key(connection,1,root,XCB_MOD_MASK_1,XCB_GRAB_ANY,XCB_GRAB_MODE_ASYNC,XCB_GRAB_MODE_ASYNC);

	xcb_flush(connection);
	for(;;){
		event = xcb_wait_for_event(connection);
		switch(event->response_type & ~0x80){
			case XCB_KEY_PRESS:
				{
			  	keypress = ( xcb_key_press_event_t *) event;
					if(keypress->child != NULL)
						win = keypress->child;
					keycode = keypress->detail;
					if(keycode == keycode_j){
						move_window(win,connection,DOWN);
					}
					else if(keycode == keycode_k){
						move_window(win,connection,UP);
					}
					else if(keycode == keycode_l){
						move_window(win,connection,RIGHT);
					}
					else if(keycode == keycode_h){
						move_window(win,connection,LEFT);
					}
					xcb_flush(connection);
					break;
				}
		}
	}
	xcb_disconnect(connection);
	return 0;
}
void move_window(xcb_drawable_t win,xcb_connection_t *c,enum direction d){
	geom = xcb_get_geometry_reply(c,xcb_get_geometry(c,win),NULL);
	switch(d){
		case UP:
			{
				values[0] = geom->x;
				values[1] = geom->y - STEP;
				break;
			}
		case DOWN:
			{
				values[0] = geom->x;
				values[1] = geom->y + STEP;
				break;
			}
		case LEFT:
			{
				values[0] = geom->x - STEP;
				values[1] = geom->y;
				break;
			}
		case RIGHT:
			{
				values[0] = geom->x + STEP;
				values[1] = geom->y;
				break;
			}
	}
	xcb_configure_window(c,win,XCB_CONFIG_WINDOW_X|XCB_CONFIG_WINDOW_Y,values);
	xcb_flush(c);
}
