#pragma once
#ifndef ENTITY_H
#define ENTITY_H
#include <unordered_map>
#include <string>
const int  MAX_ENTITIES =5000;
class Component;
class Entity {
    /*friend class EntityManager;
    Entity();
    int id;*/
    
public:
    //bool active =true; // if false we need to remove that entity from the game
    std::unordered_map<std::string, Component*> components;
    //std::vector<std::unique_ptr<Component> components;
   // ComponentArray compArr;
   // ComponentBitSet compBitset;
    /*void update(){
        for (auto& c:components) c->update();
        for (auto& c:components) c->draw();
    }
    void draw(){}
    bool isActive(){
        return active;
    }
    void destroy(){active=false;}*/
    static int entityCount;
    int id;
    Entity();
    void addComponent(Component* x, std::string y);
    
    Component*getComponent(std:: string name);
    void removeComponent(std::string x);
    int getId();
    void setId(int id);
    void update();
    
    ~Entity();
};


#endif //TEARS_ENGINE_ENTITY_H