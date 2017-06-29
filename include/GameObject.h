#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Rectangle.h"
#include <iostream>
#include <sstream>
#include <unordered_map>

using std::string;
using std::stringstream;
using std::unordered_map;

class GameObject{
public:
	virtual ~GameObject(){};
	virtual void update(float delta) = 0;

	virtual void render() = 0;
	virtual bool is_dead() = 0;
	virtual void notify_collision(GameObject & object) = 0;
	virtual bool is(string type) {
		stringstream types(type);
		string tag;
		while(types >> tag) {
			if(not tags[tag]) return false;
		}
		return true;
	}

	virtual void add_tags(string ctags) {
		stringstream types(ctags);
		string tag;
		while(types >> tag) {
			tags[tag] = true;
		}
	}

	virtual void remove_tags(string ctags) {
		stringstream types(ctags);
		string tag;
		while(types >> tag) {
			tags[tag] = false;
		}
	}

	Rectangle box;
	float rotation = 0;

	unordered_map<string, bool> tags;
};

#endif
