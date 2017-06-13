#ifndef CONFIG_H
#define CONFIG_H

#include <string>

#define RES_FOLDER string("res/")

using std::string;

class Config{
public:
	static void init();
	static int get_width();
	static int get_height();
	static int is_fullscreen();
	static void update_information(int cwidth, int cheight, int cfullscreen);

private:
	static int width;
	static int height;
	static int fullscreen;
};

#endif
