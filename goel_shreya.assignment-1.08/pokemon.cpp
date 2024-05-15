#include "pokemon.h"
#include "poke327.h"
#include "db_parse.h"
#include <vector>
#include <bits/stdc++.h>

bool compare_level_moves(levelup_move m1, levelup_move m2) {
    if (m1.level == m2.level){
        return m1.move < m2.move;
    }
    return m1.level < m2.level;
}

bool is_equal_moves(levelup_move m1, levelup_move m2){ 
    return m1.move == m2.move;
}

int set_level() {
    int distance = (abs(world.cur_idx[dim_x] - 200) + abs(world.cur_idx[dim_y] - 200));
    int minLevel = 0;
    int maxLevel = 0;

    if(distance <= 200){
        minLevel = 1;
        maxLevel = distance / 2;
    }else if(distance >= 200){
        minLevel = (distance - 200) / 2;
        maxLevel = 100;
    }

    //in case level becomes negative
    if(minLevel < 1){
        minLevel = 1;
    }
    if(maxLevel < 1){
        maxLevel = 1;
    }
    if(minLevel > 100){
        minLevel = 100;
    }
    if(maxLevel > 100){
        maxLevel = 100;
    }

    int final_level = (rand() % maxLevel) + minLevel;
   return final_level;
}


Pokemon::Pokemon() : Pokemon(set_level()) {}
Pokemon::Pokemon(int l){
    int i;
    // Set level
    this->level = l;

    //Set pokemon
    this->poke_id = (rand() % 898) + 1;

    //set shiny
    this->shiny = rand() % 8192 == 0;

    //set gender
    this->gender = (pokemon_gender) (rand() % 1);

    // Set stats and IVs
    for (i = 0; i < num_poke_stats; i++) {
        this->IV[i] = rand() % 16; // IVs are some number between 0-15
        // Each poke_id has six stats, multiply by 6 to get the last stat, then subtract accordingly to get current stat
        int stat_index = (this->poke_id * num_poke_stats) - (num_poke_stats - i + 1); // Ex. id = 1, HP is at index 1. 1 = 6 * 1 - (6 - 0 + 1) = 6 - 5
        this->stats[i] = (((pokemon_stats[stat_index].base_stat + IV[i]) * this->level) / 100) + 5;
        if (i == 0) {
            this->stats[i] += 5 + this->level;
        }
    }

    // Have vector for levelup moveset
    std::vector<levelup_move> moveset;
    std::vector<levelup_move>::iterator move_iter;
    for (i = 1; i < 528239; i++) {
        if (pokemon_moves[i].pokemon_id == poke_id && pokemon_moves[i].pokemon_move_method_id == 1) {
                levelup_move m;
                m.move = pokemon_moves[i].move_id;
                m.level = pokemon_moves[i].level;
                moveset.push_back(m);
        }
    }

    sort(moveset.begin(), moveset.end(), compare_level_moves);
    moveset.erase( unique( moveset.begin(), moveset.end(), is_equal_moves), moveset.end() );

    size_t eligible_moves = 0;
    while (eligible_moves < moveset.size() && moveset[eligible_moves].level <= this->level){
        eligible_moves++;
    }
    if (eligible_moves == 0) {
        do {
            this->level++;
            eligible_moves = 0;
            while (eligible_moves < moveset.size() && moveset[eligible_moves].level <= this->level){
                eligible_moves++;
            }
        } while (eligible_moves == 0);
    }

    this->poke_moves[0] = moveset[rand() % eligible_moves].move;
        if(eligible_moves > 1) {
            this->poke_moves[1] = moveset[rand() % eligible_moves].move;
            while (this->poke_moves[1] == this->poke_moves[0]){
                int random = rand() % eligible_moves;
                this->poke_moves[1] = moveset[random].move;
            }
    }
    else {
        this->poke_moves[1] = 0;
    }

}
char* Pokemon::getName() const {
    return species[this->poke_id].identifier;
}
int Pokemon::getLevel() const{
    return this->level;
}

int Pokemon::getHP() const{
    return stats[hp];
}

int Pokemon::getAtk() const{
    return stats[attack];
}

int Pokemon::getDef() const{
    return stats[defense];
}

int Pokemon::getSpAtk() const{
    return stats[special_attack];
}

int Pokemon::getSpDef() const{
    return stats[special_defense];
}

int Pokemon::getSpd() const {
    return stats[speed];
}

bool Pokemon::isShiny() const {
    return this->shiny;
}
char* Pokemon::getMove1() const {
    return moves[this->poke_moves[0]].identifier;
}
char* Pokemon::getMove2() const {
    if (this->poke_moves[1] > 0)
    {
        return moves[this->poke_moves[1]].identifier;
    }
    return (char *)"N/A";
}