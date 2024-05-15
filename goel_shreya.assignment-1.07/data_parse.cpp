#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <sys/stat.h>
#include <climits>
#include <iostream>

#include <fstream>
#include <sstream>

#define print_int(v)  \
((v == INT_MAX) ? "" : std::to_string(v))

//Creates a structure for the pokemon; parsing the pokemon.csv file
typedef struct pokemon{
    int id;
    std::string identifier; 
    int species_id;
    int height;
    int weight;
    int base_experience;
    int order;
    int default_int;
}pokemon_t;

typedef struct moves {
    int id;
    std::string identifier;
    int generation_id;
    int type_id;
    int power;
    int pp;
    int accuracy;
    int priority;
    int target_id;
    int damage_class_id;
    int effect_id;
    int effect_chance;
    int contest_type_id;
    int contest_effect_id;
    int super_contest_effect_id;
} moves_t;

typedef struct p_moves{
    int id;
    int version_group;
    int move_id;
    int pokemon_move_method;
    int level;
    int order;
}p_moves_t;

typedef struct pokemon_species {
    int id;
    std::string identifier;
    int generation_id;
    int evolves_from_species_id;
    int evolution_chain_id;
    int color_id;
    int shape_id;
    int habitat_id;
    int gender_rate;
    int capture_rate;
    int base_happiness;
    int is_baby;
    int hatch_counter;
    int has_gender_differences;
    int growth_rate_id;
    int forms_switchable;
    int is_legendary;
    int is_mythical;
    int order;
    int conquest_order;
} pokemon_species_t;

typedef struct experience {
    int growth_rate_id;
    int level;
    int experience;
} experience_t;

typedef struct type_names {
    int type_id;
    int local_language_id;
    std::string name;
} type_names_t;

typedef struct pokemon_stats {
    int pokemon_id;
    int stat_id;
    int base_stat;
    int effort;
} pokemon_stats_t;

typedef struct stats {
    int id;
    int damage_class_id;
    std::string identifier;
    int is_battle_only;
    int game_index;
} stats_t;

typedef struct pokemon_types {
    int pokemon_id;
    int type_id;
    int slot;
} pokemon_types_t;

pokemon pokemon_data[1093];
moves moves_data[845];
p_moves p_moves_data[528239];
pokemon_species pokemon_species_data[899];
experience experience_data[601];
type_names type_data[19];
pokemon_stats pokemon_stats_data[6553];
stats stats_data[9];
pokemon_types pokemon_types_data[1676];

bool can_set(std::string c){
    if (c == ""){
        return false;
    }else{
        return true;
    }
}

void set_pokemon(std::string column, int columNum, pokemon *p){
                switch(columNum){
                    case 1:
                    if(can_set(column) == true){
                        p->id = std::stoi(column);
                    }else{
                        p->id = INT_MAX;
                    }
                    break;
                    //identifier
                    case 2:
                    p->identifier = column;
                    break;
                    
                    case 3:
                    if(can_set(column) == true){
                        p->species_id = std::stoi(column);
                    }else{
                        p->species_id = INT_MAX;
                    }
                    break;

                    case 4:
                    if(can_set(column) == true){
                        p->height = std::stoi(column);
                    }else{
                        p->height = INT_MAX;
                    }
                    break;

                    case 5:
                    if(can_set(column) == true){
                        p->weight = std::stoi(column);
                    }else{
                        p->weight = INT_MAX;
                    }
                    break;

                    case 6:
                    if(can_set(column) == true){
                        p->base_experience = std::stoi(column);
                    }else{
                        p->base_experience = INT_MAX;
                    }
                    break;

                    case 7:
                      if(can_set(column) == true){
                        p->order = std::stoi(column);
                    }else{
                        p->order = INT_MAX;
                    }
                    break;

                    case 8:
                    if(can_set(column) == true){
                        p->default_int = std::stoi(column);
                    }else{
                        p->default_int = INT_MAX;
                    }
                    break;
                }
}

void set_moves(std::string column, int columNum, moves *m){
                    switch(columNum){
                    //id
                    case 1:
                    if(can_set(column) == true){
                        m->id = std::stoi(column);
                    }else{
                        m->id = INT_MAX;
                    }
                    break;
                    //identifier
                    case 2:
                    m->identifier = column;
                    break;
                    //generation id
                    case 3:
                    if(can_set(column) == true){
                        m->generation_id = std::stoi(column);
                    }else{
                        m->generation_id  = INT_MAX;
                    }
                    break;
                    //type id
                    case 4:
                    if(can_set(column) == true){
                        m->type_id = std::stoi(column);
                    }else{
                        m->type_id = INT_MAX;
                    }
                    break;
                    //power
                    case 5:
                    if(can_set(column) == true){
                        m->power = std::stoi(column);
                    }else{
                        m->power = INT_MAX;
                    }
                    break;
                    //pp
                    case 6:
                    if(can_set(column) == true){
                        m->pp = std::stoi(column);
                    }else{
                        m->pp = INT_MAX;
                    }
                    break;
                    //accuracy
                    case 7:
                      if(can_set(column) == true){
                        m->accuracy = std::stoi(column);
                    }else{
                        m->accuracy  = INT_MAX;
                    }
                    break;
                    //priority
                    case 8:
                    if(can_set(column) == true){
                        m->priority = std::stoi(column);
                    }else{
                        m->priority = INT_MAX;
                    }
                    break;
                    //target ide
                    case 9:
                    if(can_set(column) == true){
                        m->target_id= std::stoi(column);
                    }else{
                        m->target_id = INT_MAX;
                    }
                    break;
                    //damage class id
                    case 10:
                    if(can_set(column) == true){
                        m->damage_class_id = std::stoi(column);
                    }else{
                        m->damage_class_id = INT_MAX;
                    }
                    break;
                //effect id
                    case 11:
                    if(can_set(column) == true){
                        m->effect_id= std::stoi(column);
                    }else{
                        m->effect_id = INT_MAX;
                    }
                    break;
                    //effect chance
                    case 12:
                     if(can_set(column) == true){
                        m->effect_chance= std::stoi(column);
                    }else{
                        m->effect_chance = INT_MAX;
                    }
                    break;  
                    //contest type id
                    case 13:
                    if(can_set(column) == true){
                        m->contest_type_id= std::stoi(column);
                    }else{
                        m->contest_type_id = INT_MAX;
                    }
                    break;
                    //contest effect id
                    case 14:
                    if(can_set(column) == true){
                        m->contest_effect_id= std::stoi(column);
                    }else{
                        m->contest_effect_id = INT_MAX;
                    }
                    break;

                  case 15:
                    if(can_set(column) == true){
                        m->super_contest_effect_id = std::stoi(column);
                    }else{
                        m->super_contest_effect_id = INT_MAX;
                    }
                }
}

void set_pmoves(std::string column, int columNum, p_moves *p){
                switch(columNum){
                    case 1:
                    if(can_set(column) == true){
                        p->id = std::stoi(column);
                    }else{
                        p->id = INT_MAX;
                    }
                    break;
                    
                    case 2:
                    if(can_set(column) == true){
                        p->version_group = std::stoi(column);
                    }else{
                        p->version_group = INT_MAX;
                    }
                    break;

                    case 3:
                    if(can_set(column) == true){
                        p->move_id = std::stoi(column);
                    }else{
                        p->move_id = INT_MAX;
                    }
                    break;

                    case 4:
                    if(can_set(column) == true){
                        p->pokemon_move_method = std::stoi(column);
                    }else{
                        p->pokemon_move_method = INT_MAX;
                    }
                    break;

                    case 5:
                    if(can_set(column) == true){
                        p->level = std::stoi(column);
                    }else{
                        p->level = INT_MAX;
                    }
                    break;

                    case 7:
                      if(can_set(column) == true){
                        p->order = std::stoi(column);
                    }else{
                        p->order = INT_MAX;
                    }
                    break;
                }
}

void set_pokemon_species(std::string column, int columNum, pokemon_species *p){
                    switch(columNum){
                    //id
                    case 1:
                    if(can_set(column) == true){
                        p->id = std::stoi(column);
                    }else{
                        p->id = INT_MAX;
                    }
                    break;
                    //identifier
                    case 2:
                    p->identifier = column;
                    break;
                    //generation id
                    case 3:
                    if(can_set(column) == true){
                        p->generation_id = std::stoi(column);
                    }else{
                        p->generation_id  = INT_MAX;
                    }
                    break;
                    //type id
                    case 4:
                    if(can_set(column) == true){
                        p->evolves_from_species_id = std::stoi(column);
                    }else{
                        p->evolves_from_species_id = INT_MAX;
                    }
                    break;
                    //power
                    case 5:
                    if(can_set(column) == true){
                        p->evolution_chain_id = std::stoi(column);
                    }else{
                        p->evolution_chain_id = INT_MAX;
                    }
                    break;
                    //pp
                    case 6:
                    if(can_set(column) == true){
                        p->color_id = std::stoi(column);
                    }else{
                        p->color_id = INT_MAX;
                    }
                    break;
                    //accuracy
                    case 7:
                      if(can_set(column) == true){
                        p->shape_id = std::stoi(column);
                    }else{
                        p->shape_id  = INT_MAX;
                    }
                    break;
                    //priority
                    case 8:
                    if(can_set(column) == true){
                        p->habitat_id = std::stoi(column);
                    }else{
                        p->habitat_id = INT_MAX;
                    }
                    break;
                    //target ide
                    case 9:
                    if(can_set(column) == true){
                        p->gender_rate= std::stoi(column);
                    }else{
                        p->gender_rate = INT_MAX;
                    }
                    break;
                    //damage class id
                    case 10:
                    if(can_set(column) == true){
                        p->capture_rate = std::stoi(column);
                    }else{
                        p->capture_rate = INT_MAX;
                    }
                    break;
                //effect id
                    case 11:
                    if(can_set(column) == true){
                        p->base_happiness= std::stoi(column);
                    }else{
                        p->base_happiness = INT_MAX;
                    }
                    break;
                    //effect chance
                    case 12:
                     if(can_set(column) == true){
                        p->is_baby = std::stoi(column);
                    }else{
                        p->is_baby = INT_MAX;
                    }
                    break;  
                    //contest type id
                    case 13:
                    if(can_set(column) == true){
                        p->hatch_counter = std::stoi(column);
                    }else{
                        p->hatch_counter = INT_MAX;
                    }
                    break;
                    //contest effect id
                    case 14:
                    if(can_set(column) == true){
                        p->has_gender_differences = std::stoi(column);
                    }else{
                        p->has_gender_differences = INT_MAX;
                    }
                    break;

                  case 15:
                    if(can_set(column) == true){
                        p->growth_rate_id = std::stoi(column);
                    }else{
                        p->growth_rate_id = INT_MAX;
                    }
                    break;
                    case 16:
                    if(can_set(column) == true){
                        p->forms_switchable = std::stoi(column);
                    }else{
                        p->forms_switchable = INT_MAX;
                    }
                    break;
                    case 17:
                    if(can_set(column) == true){
                        p->is_legendary = std::stoi(column);
                    }else{
                        p->is_legendary = INT_MAX;
                    }
                    break;
                    case 18:
                    if(can_set(column) == true){
                        p->is_mythical = std::stoi(column);
                    }else{
                        p->is_mythical = INT_MAX;
                    }
                    break;
                    case 19:
                    if(can_set(column) == true){
                        p->order = std::stoi(column);
                    }else{
                        p->order = INT_MAX;
                    }
                    break;
                    case 20:
                    if(can_set(column) == true){
                        p->conquest_order = std::stoi(column);
                    }else{
                        p->conquest_order = INT_MAX;
                    }
                    break;
                }   
}

void set_experience(std::string column, int columNum, experience *e ){
    switch(columNum){
                    case 1:
                    if(can_set(column) == true){
                        e->growth_rate_id = std::stoi(column);
                    }else{
                        e->growth_rate_id = INT_MAX;
                    }
                    break;
                    //identifier
                    case 2:
                    if(can_set(column) == true){
                        e->level = std::stoi(column);
                    }else{
                        e->level = INT_MAX;
                    }
                    break;
                    
                    case 3:
                    if(can_set(column) == true){
                        e->experience = std::stoi(column);
                    }else{
                        e->experience = INT_MAX;
                    }
                    break;
        }
}

void set_type(std::string column, int columNum, type_names *t ){
    switch(columNum){
                    case 1:
                    if(can_set(column) == true){
                        t->type_id= std::stoi(column);
                    }else{
                        t->type_id = INT_MAX;
                    }
                    break;
                    //identifier
                    case 2:
                    if(can_set(column) == true){
                        t->local_language_id = std::stoi(column);
                    }else{
                        t->local_language_id = INT_MAX;
                    }
                    break;

                    case 3:
                    t->name = column;
                    break;
        }
}

void set_pokemon_stats(std::string column, int columNum, pokemon_stats *t ){
    switch(columNum){
                    case 1:
                    if(can_set(column) == true){
                        t->pokemon_id= std::stoi(column);
                    }else{
                        t->pokemon_id = INT_MAX;
                    }
                    break;
                    //identifier
                    case 2:
                    if(can_set(column) == true){
                        t->stat_id = std::stoi(column);
                    }else{
                        t->stat_id= INT_MAX;
                    }
                    break;

                    case 3:
                    if(can_set(column) == true){
                        t->base_stat = std::stoi(column);
                    }else{
                        t->base_stat= INT_MAX;
                    }
                    break;
                    
                    case 4:
                    if(can_set(column) == true){
                        t->effort = std::stoi(column);
                    }else{
                        t->effort = INT_MAX;
                    }
                    break;
        }
}

void set_stats(std::string column, int columNum, stats *t ){
    switch(columNum){
                    case 1:
                    if(can_set(column) == true){
                        t->id= std::stoi(column);
                    }else{
                        t->id = INT_MAX;
                    }
                    break;
                    //identifier
                    case 2:
                    if(can_set(column) == true){
                        t->damage_class_id = std::stoi(column);
                    }else{
                        t->damage_class_id = INT_MAX;
                    }
                    break;

                    case 3:
                    t -> identifier = column;
                    break;
                    
                    case 4:
                    if(can_set(column) == true){
                        t->is_battle_only = std::stoi(column);
                    }else{
                        t->is_battle_only = INT_MAX;
                    }
                    break;

                    case 5:
                    if(can_set(column) == true){
                        t->game_index = std::stoi(column);
                    }else{
                        t->game_index = INT_MAX;
                    }
                    break;
        }
}

void set_pokemon_type(std::string column, int columNum, pokemon_types *t ){
    switch(columNum){
                    case 1:
                    if(can_set(column) == true){
                        t->pokemon_id= std::stoi(column);
                    }else{
                        t->pokemon_id = INT_MAX;
                    }
                    break;
                    //identifier
                    case 2:
                    if(can_set(column) == true){
                        t->type_id = std::stoi(column);
                    }else{
                        t->type_id = INT_MAX;
                    }
                    break;
                    
                    case 3:
                    if(can_set(column) == true){
                        t->slot = std::stoi(column);
                    }else{
                        t->slot = INT_MAX;
                    }
                    break;
        }    
}
void parse_data(std::ifstream &file, std::string filename){
    std::string line;
    std::string cell;
    std::stringstream ss;
    //get the first line in the file
    std::getline(file, line);


    //case: pokemon file
    if(filename == "pokemon"){

        int i = 0;
        while(std::getline(file, line)){
            //set stream for current line
            std::stringstream ss(line);

            //std::string s = ss.str();
            //std::cout << s << std::endl;

            int colNum = 1;
            //create new pokemon object
            pokemon_t p;
            //loop through each cell
            while (std::getline(ss, cell, ',') || colNum < 9){

                //each individual case for cell
                set_pokemon(cell, colNum, &p);
                //increment col num
                colNum++;
            }
            pokemon_data[i] = p;
            i++;
        }
    }else if(filename == "moves"){
        int i = 0;
        while(std::getline(file, line)){
            //set stream for current line
            std::stringstream ss(line);

            //std::string s = ss.str();
            //std::cout << s << std::endl;

            int colNum = 1;
            //create new moves object
            moves_t moves;
            //loop through each cell
            while (std::getline(ss, cell, ',') || colNum < 16){

                //each individual case for cell
                set_moves(cell, colNum, &moves);
                //increment col num
                colNum++;
            }
            moves_data[i] = moves;
            i++;
        }
    }else if (filename == "pokemon_moves"){
        int i = 0;
        while(std::getline(file, line)){
            //set stream for current line
            std::stringstream ss(line);

            //std::string s = ss.str();
            //std::cout << s << std::endl;

            int colNum = 1;
            //create new moves object
            p_moves_t p_moves;
            //loop through each cell
            while (std::getline(ss, cell, ',') || colNum < 7){

                //each individual case for cell
                set_pmoves(cell, colNum, &p_moves);
                //increment col num
                colNum++;
            }
            p_moves_data[i] = p_moves;
            i++;
        }
    }else if(filename == "pokemon_species"){
        int i = 0;
         while(std::getline(file, line)){
            //set stream for current line
            std::stringstream ss(line);

            //std::string s = ss.str();
            //std::cout << s << std::endl;

            int colNum = 1;
            //create new moves object
            pokemon_species_t pokemon_species;
            //loop through each cell
            while (std::getline(ss, cell, ',') || colNum < 21){

                //each individual case for cell
                set_pokemon_species(cell, colNum, &pokemon_species);
                //increment col num
                colNum++;
            }
            pokemon_species_data[i] = pokemon_species;
            i++;
        }       
    }else if(filename == "experience"){
        int i = 0;
         while(std::getline(file, line)){
            //set stream for current line
            std::stringstream ss(line);

            //std::string s = ss.str();
            //std::cout << s << std::endl;

            int colNum = 1;
            //create new moves object
            experience_t experience;
            //loop through each cell
            while (std::getline(ss, cell, ',') || colNum < 4){

                //each individual case for cell
                set_experience(cell, colNum, &experience);
                //increment col num
                colNum++;
            }
            experience_data[i] = experience;
            i++;
        } 
    }else if(filename == "type_names"){
         int i = 0;
         while(std::getline(file, line)){
            //set stream for current line
            std::stringstream ss(line);

            //std::string s = ss.str();
            //std::cout << s << std::endl;

            int colNum = 1;
            //create new moves object
            type_names_t type;
            //loop through each cell
            while (std::getline(ss, cell, ',') || colNum < 4){

                //each individual case for cell
                set_type(cell, colNum, &type);
                //increment col num
                colNum++;
            }
            type_data[i] = type;
            i++;
        }        
    }else if(filename == "pokemon_stats"){
         int i = 0;
         while(std::getline(file, line)){
            //set stream for current line
            std::stringstream ss(line);

            //std::string s = ss.str();
            //std::cout << s << std::endl;

            int colNum = 1;
            //create new moves object
            pokemon_stats_t pokemon_stats;
            //loop through each cell
            while (std::getline(ss, cell, ',') || colNum < 5){

                //each individual case for cell
                set_pokemon_stats(cell, colNum, &pokemon_stats);
                //increment col num
                colNum++;
            }
            pokemon_stats_data[i] = pokemon_stats;
            i++;
        }          
    }else if(filename == "stats"){
        int i = 0;
         while(std::getline(file, line)){
            //set stream for current line
            std::stringstream ss(line);

            //std::string s = ss.str();
            //std::cout << s << std::endl;

            int colNum = 1;
            //create new moves object
            stats_t stats;
            //loop through each cell
            while (std::getline(ss, cell, ',') || colNum < 6){

                //each individual case for cell
                set_stats(cell, colNum, &stats);
                //increment col num
                colNum++;
            }
            stats_data[i] = stats;
            i++;
        }         
    }else if(filename == "pokemon_types"){
         int i = 0;
         while(std::getline(file, line)){
            //set stream for current line
            std::stringstream ss(line);

            //std::string s = ss.str();
            //std::cout << s << std::endl;

            int colNum = 1;
            //create new moves object
            pokemon_types_t pokemon_type;
            //loop through each cell
            while (std::getline(ss, cell, ',') || colNum < 4){

                //each individual case for cell
                set_pokemon_type(cell, colNum, &pokemon_type);
                //increment col num
                colNum++;
            }
            pokemon_types_data[i] = pokemon_type;
            i++;
        }        
    }
    file.close();
}
void print_pokemon(){
            for(int i = 0; i < 1092; i++){
                std::cout << "id: " << print_int(pokemon_data[i].id) <<
                    " identifier: " << pokemon_data[i].identifier <<  
                    " species id: " << print_int(pokemon_data[i].species_id) <<
                    " height: " << print_int(pokemon_data[i].height) <<
                    " weight: " << print_int(pokemon_data[i].weight) <<
                    " base xp " << print_int(pokemon_data[i].base_experience) <<
                    " order: " << print_int(pokemon_data[i].order) <<
                    " is default: " << print_int(pokemon_data[i].default_int) <<
                    std::endl;
        }
}

void print_moves(){
    for(int i = 0; i < 10018; i++){
        std::cout << "id: " << print_int(moves_data[i].id) <<
            " identifier: " << moves_data[i].identifier <<  
            " generation id: " << print_int(moves_data[i].generation_id) <<
            " type id: " << print_int(moves_data[i].type_id) <<
            " power: " << print_int(moves_data[i].power) <<
            " pp " << print_int(moves_data[i].pp) <<
            " accuracy: " << print_int(moves_data[i].accuracy) <<
            " priority: " << print_int(moves_data[i].priority) <<
            "target id: " << print_int(moves_data[i].target_id) <<
            "damage class id: " << print_int(moves_data[i].damage_class_id) <<
            "effect id: " << print_int(moves_data[i].effect_id) << 
            "effect chance: " << print_int(moves_data[i].effect_chance) << 
            "contest type id: " << print_int(moves_data[i].contest_type_id) << 
            "contest effect id: " << print_int(moves_data[i].contest_effect_id) << 
            "super contest effect id: " << print_int(moves_data[i].super_contest_effect_id) << 
            std::endl;
        }
}

void print_pokemon_moves(){
               for(int i = 0; i < 528239; i++){
                std::cout << "id: " << print_int(p_moves_data[i].id) <<
                    " version group id: " << p_moves_data[i].version_group <<  
                    " move id: " << print_int(p_moves_data[i].move_id) <<
                    " pokemon move method id " << print_int(p_moves_data[i].pokemon_move_method) <<
                    " level: " << print_int(p_moves_data[i].level) <<
                    " order " << print_int(p_moves_data[i].order) <<
                    std::endl;
        } 
}

void print_pokemon_species(){
    for(int i = 0; i < 899; i++){
        std::cout << "id: " << print_int(pokemon_species_data[i].id) <<
            " identifier: " << pokemon_species_data[i].identifier <<  
            " generation id: " << print_int(pokemon_species_data[i].generation_id) <<
            " evolves id: " << print_int(pokemon_species_data[i].evolves_from_species_id) <<
            " evolution chain: " << print_int(pokemon_species_data[i].evolution_chain_id) <<
            " color id " << print_int(pokemon_species_data[i].color_id) <<
            " shape id: " << print_int(pokemon_species_data[i].shape_id) <<
            " habitat id: " << print_int(pokemon_species_data[i].habitat_id) <<
            "gender rate: " << print_int(pokemon_species_data[i].gender_rate) <<
            "capture rate: " << print_int(pokemon_species_data[i].capture_rate) <<
            "base happiness: " << print_int(pokemon_species_data[i].base_happiness) << 
            "baby: " << print_int(pokemon_species_data[i].is_baby) << 
            "hatch counter: " << print_int(pokemon_species_data[i].hatch_counter) << 
            "gender differences: " << print_int(pokemon_species_data[i].has_gender_differences) << 
            "growth rate: " << print_int(pokemon_species_data[i].growth_rate_id) << 
            "forms switchable" << print_int(pokemon_species_data[i].forms_switchable) << 
            "legendary" << print_int(pokemon_species_data[i].is_legendary) << 
            "mythical" << print_int(pokemon_species_data[i].is_mythical) << 
            "order" << print_int(pokemon_species_data[i].order) << 
            "conquest order" << print_int(pokemon_species_data[i].conquest_order) << 
            std::endl;
        }
}

void print_experience(){
    for(int i = 0; i < 601; i++){
         std::cout << "growth rate id: " << print_int(experience_data[i].growth_rate_id) << 
                        " level: " << print_int(experience_data[i].level) <<
                        " experience: " << print_int(experience_data[i].experience) <<
                        std::endl;
    }
}

void print_type(){
    for(int i = 0; i < 19; i++){
          std::cout << "type id: " << print_int(type_data[i].type_id) << 
                        " local language: " << print_int(type_data[i].local_language_id) <<
                        " name: " << type_data[i].name <<
                        std::endl;
    }       
    }

void print_pokemon_stats(){
    for(int i = 0; i < 6553; i++){
          std::cout << "pokemon id: " << print_int(pokemon_stats_data[i].pokemon_id) << 
                        " stats id: " << print_int(pokemon_stats_data[i].stat_id) <<
                        " base stat: " << print_int(pokemon_stats_data[i].base_stat)<<
                        "effort: " << print_int(pokemon_stats_data[i].effort) <<
                        std::endl;
    }       
}

void print_stats(){
    for(int i = 0; i < 9; i++){
            std::cout << "id: " << print_int(stats_data[i].id) << 
                        " damage class id: " << print_int(stats_data[i].damage_class_id) <<
                        " identifier: " << stats_data[i].identifier <<
                        "battle: " << print_int(stats_data[i].is_battle_only) <<
                        "game index: " << print_int(stats_data[i].game_index) <<
                        std::endl;
    } 
}

void print_pokemon_types(){
    for(int i = 0; i < 1676; i++){
                    std::cout << "id: " << print_int(pokemon_types_data[i].pokemon_id) << 
                        "type id: " << print_int(pokemon_types_data[i].type_id) <<
                        "slot: " << print_int(pokemon_types_data[i].slot) <<
                        std::endl;
    }
}
void print_data(std::string &filename){
    if(filename == "pokemon"){
        print_pokemon();
    }else if(filename == "moves"){
        print_moves();
    }else if(filename == "pokemon_moves"){
        print_pokemon_moves();
    }else if(filename == "pokemon_species"){
        print_pokemon_species();
    }else if(filename == "experience"){
        print_experience();
    }else if(filename == "type_name"){
        print_type();
    }else if(filename == "pokemon_stats"){
        print_pokemon_stats();
    }else if(filename == "stats"){
        print_stats();
    }else if(filename == "pokemon_types"){
        print_pokemon_types();
    }
}