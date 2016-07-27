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
xcb_window_t *get_windows(xcb_connection_t *c, xcb_window_t root, int *len);
void focus_next(xcb_connection_t *c,xcb_drawable_t *focus_win,xcb_window_t *windows,int len,int *focus_pos);
int main(){
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
	int window_num;
	xcb_window_t *windows;
	int focus_win;
	const xcb_keycode_t keycode_j = (xcb_key_symbols_get_keycode(keysyms, XK_j))[0];
	const xcb_keycode_t keycode_k = (xcb_key_symbols_get_keycode(keysyms, XK_k))[0];
	const xcb_keycode_t keycode_h = (xcb_key_symbols_get_keycode(keysyms, XK_h))[0];
	const xcb_keycode_t keycode_l = (xcb_key_symbols_get_keycode(keysyms, XK_l))[0];
	const xcb_keycode_t keycode_tab = (xcb_key_symbols_get_keycode(keysyms, XK_Tab))[0];
	xcb_grab_key(connection,1,root,XCB_MOD_MASK_1,XCB_GRAB_ANY,XCB_GRAB_MODE_ASYNC,XCB_GRAB_MODE_ASYNC);

	xcb_flush(connection);
	
	windows = get_windows(connection,root,&window_num);
	win = windows[0];
	focus_win = 0;
	xcb_set_input_focus(connection,XCB_INPUT_FOCUS_NONE,win,XCB_CURRENT_TIME);
	for(;;){
		event = xcb_wait_for_event(connection);
		switch(event->response_type & ~0x80){
			case XCB_KEY_PRESS:
				{
			  	keypress = ( xcb_key_press_event_t *) event;
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
					else if(keycode == keycode_tab){
						focus_next(connection,&win,windows,window_num,&focus_win);
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
xcb_window_t *get_windows(xcb_connection_t *c, xcb_window_t root,int *len){
	xcb_query_tree_reply_t *reply = xcb_query_tree_reply(c,xcb_query_tree(c,root),0);
	*len = xcb_query_tree_children_length(reply);
	return xcb_query_tree_children(reply);
}
void focus_next(xcb_connection_t *c, xcb_drawable_t *focus_win, xcb_window_t *windows, int len, int *focus_win_pos){
	if(*focus_win_pos < len){
		*focus_win = windows[++(*focus_win_pos)];
	}
	else{
		*focus_win = windows[*focus_win_pos = 0];
	}
	xcb_set_input_focus(c,XCB_INPUT_FOCUS_NONE,*focus_win,XCB_CURRENT_TIME);
}
