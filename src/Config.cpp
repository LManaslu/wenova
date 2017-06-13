#include "Config.h"

#include <fstream>

int Config::width;
int Config::height;
int Config::fullscreen;

void Config::init(){
	std::fstream config_file(RES_FOLDER + "config_file.dat");
	config_file >> width >> height >> fullscreen;
}

int Config::get_width(){
	return width;
}

int Config::get_height(){
	return height;
}

int Config::is_fullscreen(){
	return fullscreen;
}

void Config::update_information(int cwidth, int cheight, int cfullscreen){
	width = cwidth;
	height = cheight;
	fullscreen = cfullscreen;
	std::ofstream config_file(RES_FOLDER + "config_file.dat", std::ios::trunc);
	config_file << width << " " << height << " " << fullscreen << std::endl;
	config_file.close();

}
