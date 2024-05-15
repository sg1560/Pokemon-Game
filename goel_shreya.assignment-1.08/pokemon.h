#ifndef POKEMON_H
  #define POKEMON_H

enum poke_stat {
    hp,
    attack,
    defense,
    special_attack,
    special_defense,
    speed,
    num_poke_stats
};

enum pokemon_gender{
    male,
    female
};

class Pokemon {
private:
    int poke_id;
    int level;
    int poke_moves[4];
    int stats[num_poke_stats];
    int IV[num_poke_stats];
    bool shiny;
   pokemon_gender gender;
public:
    Pokemon();
    Pokemon(int level);
    char* getName() const;
    int getLevel() const;
    int getHP() const;
    int getAtk() const;
    int getDef() const;
    int getSpAtk() const;
    int getSpDef() const;
    int getSpd() const;   
    bool isShiny() const;  
    char* getMove1() const;
    char* getMove2() const;
};
#endif