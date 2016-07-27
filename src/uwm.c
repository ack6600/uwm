#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <xcb/xcb.h>
#include <X11/keysym.h>

int main(){
	//helop
	int values[3];
	xcb_screen_t *screen;
	xcb_connection_t *connection = xcb_connect(NULL, NULL);
	screen = (xcb_setup_roots_iterator(xcb_get_setup(connection))).data;
	xcb_drawable_t root = screen->root;
	xcb_drawable_t win;
	xcb_generic_event_t *event;
	xcb_key_press_event_t *keypress;
	xcb_get_geometry_reply_t *geom;

	xcb_grab_key(connection,1,root,XCB_MOD_MASK_1,XCB_GRAB_ANY,XCB_GRAB_MODE_ASYNC,XCB_GRAB_MODE_ASYNC);

	xcb_flush(connection);
	for(;;){
		event = xcb_wait_for_event(connection);
		switch(event->response_type & ~0x80){
			case XCB_KEY_PRESS:
				{
			  keypress = ( xcb_key_press_event_t *) event;
				win = keypress->child;
				if(win != NULL){
					geom = xcb_get_geometry_reply(connection,xcb_get_geometry(connection,win),NULL);
					printf("%c",keypress->detail);
					values[0] = ++geom->x;
					values[1] = ++geom->y;
					xcb_configure_window(connection,win,XCB_CONFIG_WINDOW_X|XCB_CONFIG_WINDOW_Y,values);
				}
				xcb_flush(connection);
				break;
				}
		}
	}
	pause();
	xcb_disconnect(connection);
	return 0;
}
