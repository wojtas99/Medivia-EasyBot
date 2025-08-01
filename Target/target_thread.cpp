#include "target_thread.h"
#include "../Functions/memory_functions.h"
#include <QThread>
#include <iostream>
#include "../Structs/medivia_struct.h"

void TargetThread::run() {
    int closest_dist = 100;
    int idx = 0;
    Entity* closest_entity = nullptr;
    bool open_corpse = false;
    int enemy_coords[3] = {0, 0, 0};
    while (m_running && !m_targets.isEmpty()) {
        std::string target_name = m_targets[idx].value("name").toString().toStdString();
        int dist_threshold = m_targets[idx].value("distance").toInt();
        int hp_from = m_targets[idx].value("hpFrom").toInt();
        int hp_to = m_targets[idx].value("hpTo").toInt();
        idx = (idx + 1) % m_targets.size();
        closest_dist = 100;
        if (MemoryFunctions::player_base->Entity == 0) {
            if (open_corpse)
            {
                open_corpse = false;
                uint64_t tile = tile = MemoryFunctions::queue_getTile(enemy_coords[0], enemy_coords[1], enemy_coords[2]);
                uint64_t top_thing = MemoryFunctions::queue_getTopThing(tile);
                MemoryFunctions::queue_open(reinterpret_cast<Item*>(top_thing));
                msleep(500);
                std::vector<Container*> containers = MemoryFunctions::queue_getContainers();
                for (int i = 0; i < containers.size(); i++)
                {
                    for (int j = 0; j < containers[i]->number_of_items; ++j)
                    {
                        Item* item = MemoryFunctions::getItem(containers[i], j);
                        if (item->id == 2148 && ((containers[0]->item->x + 1) != (containers[0]->item->x + i)))
                        {
                            MemoryFunctions::queue_move(item, containers[0]->item);
                        }
                    }
                }
            }
            closest_entity = nullptr;
            std::vector<Entity*> entities = MemoryFunctions::queue_getSpectatorsInRangeEx(dist_threshold);
            for (Entity* entity : entities) {
                int dist_x = abs(static_cast<int>(entity->x - MemoryFunctions::map_view->LocalPlayer->x));
                int dist_y = abs(static_cast<int>(entity->y - MemoryFunctions::map_view->LocalPlayer->y));
                if (dist_x + dist_y <= closest_dist &&
                    (entity->name == target_name || target_name == "*") &&
                    hp_from >= entity->hp && entity->hp > hp_to) {
                    closest_entity = entity;
                    closest_dist = dist_x + dist_y;
                }
            }
            if (closest_entity != nullptr) {
                MemoryFunctions::queue_attack(closest_entity);
                msleep(500);
                MemoryFunctions::has_target = true;
                open_corpse = true;
            } else {
                MemoryFunctions::has_target = false;
            }
        } else {
            if (closest_entity-> x != 65535 && closest_entity-> x != 65535 && closest_entity-> x != 255) {
                enemy_coords[0] = closest_entity-> x;
                enemy_coords[1] = closest_entity-> y;
                enemy_coords[2] = closest_entity-> z;
            }
        }
        msleep(50);
    }
}


