#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

class ScreenConfig {
	public:
	
	void resize_terminal(int rows, int cols) {
		char command[50];
		sprintf(command, "resize -s %d %d", rows, cols);
		system(command);
	}

	void position_terminal(int x, int y) {
		char command[50];
		sprintf(command, "printf '\e[3;%d;%dt'", x, y);
		system(command);
	}

	void get_screen_resolution(int *rows, int *cols) {
		FILE *fp = popen("xdpyinfo | grep dimensions | awk '{print $2}'", "r");
		if (fp != NULL) {
			char buffer[20];
			if (fgets(buffer, sizeof(buffer), fp) != NULL) {
				pclose(fp);
				sscanf(buffer, "%dx%d", cols, rows);
				return;
			}
			pclose(fp);
		}
		// Default values if failed to get resolution
		*rows = 24;
		*cols = 80;
	}

	void calculate_terminal_size(int screen_width, int screen_height, int font_width, int font_height, int *rows, int *cols) {
		*rows = ((screen_height / font_height) / 2) - 6;
		*cols = screen_width / font_width;
	}
	
	void get_screen_resolution_ioctl(int *width, int *height) {
		struct winsize w;
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
		*width = w.ws_col;
		*height = w.ws_row;
	}
	
	void get_font_size(int screen_width, int screen_height, int terminal_cols, int terminal_rows, int *font_width, int *font_height) {
		*font_width = screen_width / terminal_cols;
		*font_height = screen_height / terminal_rows;
	}
	
	void get_terminal_position(int *x_pos, int *y_pos) {
		FILE *fp = popen("xdotool getwindowgeometry $(xdotool getactivewindow) | grep -oP '(?<=Absolute\\sx:\\s)\\d+' | tail -n1", "r");
		if (fp != NULL) {
			char buffer[20];
			if (fgets(buffer, sizeof(buffer), fp) != NULL) {
				pclose(fp);
				*x_pos = atoi(buffer);
			}
		}

		fp = popen("xdotool getwindowgeometry $(xdotool getactivewindow) | grep -oP '(?<=Absolute\\sy:\\s)\\d+' | tail -n1", "r");
		if (fp != NULL) {
			char buffer[20];
			if (fgets(buffer, sizeof(buffer), fp) != NULL) {
				pclose(fp);
				*y_pos = atoi(buffer);
			}
		}
	}

};
