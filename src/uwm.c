#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <xcb/xcb.h>
#include <X11/keysym.h>

int main(){
	int i;
	xcb_screen_t *screen;
	xcb_connection_t *connection = xcb_connect(NULL, NULL);
	screen = (xcb_setup_roots_iterator(xcb_get_setup(connection))).data;
	xcb_drawable_t root = screen->root;
	xcb_drawable_t win;
	xcb_generic_event_t *event;
	i = 0;

	xcb_grab_key(connection,1,root,XCB_MOD_MASK_1,XCB_GRAB_ANY,XCB_GRAB_MODE_ASYNC,XCB_GRAB_MODE_ASYNC);

	xcb_flush(connection);
	for(;;){
		event = xcb_wait_for_event(connection);
		switch(event->response_type & ~0x80){
			case XCB_KEY_PRESS:
				{
				xcb_key_press_event_t *keypress;
			  keypress = ( xcb_key_press_event_t *) event;
				win = keypress->child;
				if(keypress->detail = XK_j);
				break;
				}
		}
	}
	pause();
	xcb_disconnect(connection);
	return 0;
}
