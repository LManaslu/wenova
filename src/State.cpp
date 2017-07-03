#include "State.h"

#include "Collision.h"

State::State(){
	m_pop_requested = m_quit_requested = false;
}

State::~State(){
}

void State::add_object(GameObject * object){
	object_array.emplace_back(object);
}

bool State::pop_requested(){
	return m_pop_requested;
}

bool State::quit_requested(){
	return m_quit_requested;
}

void State::update_array(float delta){
	//collision tests
	for(unsigned i = 0; i < object_array.size(); ++i){
		for(unsigned j = i + 1; j < object_array.size(); ++j){
			auto a = object_array[i].get();
			auto b = object_array[j].get();
			if(Collision::is_colliding(a->box, b->box, a->rotation, b->rotation)){
				a->notify_collision(*b);
				b->notify_collision(*a);
			}
		}
	}

	//update
	for(unsigned it = 0; it < object_array.size(); ++it){
		object_array[it]->update(delta);
	}

	//death check
	for(unsigned it = 0; it < object_array.size(); ++it){
		if(object_array[it]->is_dead()){
			object_array.erase(object_array.begin() + it);
			break;
		}
	}
}

void State::render_array(){
	for(auto & go : object_array){
		go->render();
	}
}

void State::load_assets(){
	//NOTHING TO DO
}
