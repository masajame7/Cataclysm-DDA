#ifndef MARTIALARTS_H
#define MARTIALARTS_H

#include "pldata.h"
#include "json.h"
#include <string>
#include <vector>
#include <map>
#include <set>

class effect;
class player;

struct ma_requirements {
    bool unarmed_allowed; // does this bonus work when unarmed?
    bool melee_allowed; // what about with a melee weapon?

    int min_melee; // minimum amount of unarmed to trigger this bonus
    int min_unarmed; // minimum amount of unarmed to trigger this bonus
    int min_bashing; // minimum amount of unarmed to trigger this bonus
    int min_cutting; // minimum amount of unarmed to trigger this bonus
    int min_stabbing; // minimum amount of unarmed to trigger this bonus

    int min_bashing_damage; // minimum amount of bashing damage on the weapon
    int min_cutting_damage; // minimum amount of cutting damage on the weapon

    std::set<mabuff_id> req_buffs; // other buffs required to trigger this bonus
    std::set<std::string> req_flags; // any item flags required for this technique

    ma_requirements()
    {
        unarmed_allowed = false; // does this bonus work when unarmed?
        melee_allowed = false; // what about with a melee weapon?

        min_melee = 0; // minimum amount of unarmed to trigger this technique
        min_unarmed = 0; // etc
        min_bashing = 0;
        min_cutting = 0;
        min_stabbing = 0;

        min_bashing_damage = 0;
        min_cutting_damage = 0;
    }

    bool is_valid_player(player &u);
    bool is_valid_weapon(item &i);
};

class ma_technique
{
    public:
        ma_technique();

        std::string id;
        std::string name;

        std::string goal; // the melee goal this achieves

        // given a player's state, does this bonus apply to him?
        bool is_valid_player(player &u);

        std::set<std::string> flags;

        // message to be displayed when player (0) or npc (1) uses the technique
        std::vector<std::string> messages;

        bool defensive;
        bool dummy;
        bool crit_tec;

        ma_requirements reqs;

        int down_dur;
        int stun_dur;
        int knockback_dist;
        float knockback_spread; // adding randomness to knockback, like tec_throw
        std::string aoe; // corresponds to an aoe shape, defaults to just the target

        // offensive
        bool disarms; // like tec_disarm
        bool dodge_counter; // counter move activated on a dodge
        bool block_counter; // counter move activated on a block

        bool miss_recovery; // allows free recovery from misses, like tec_feint
        bool grab_break; // allows grab_breaks, like tec_break

        bool flaming; // applies fire effects etc

        int hit; // flat bonus to hit
        int bash; // flat bonus to bash
        int cut; // flat bonus to cut
        int pain; // attacks cause pain

        int weighting; //how often this technique is used

        float bash_mult; // bash damage multiplier
        float cut_mult; // cut damage multiplier
        float speed_mult; // speed multiplier (fractional is faster)

        float bash_str; // bonus damage to add per str point
        float bash_dex; // "" dex point
        float bash_int; // "" int point
        float bash_per; // "" per point

        float cut_str; // bonus cut damage to add per str point
        float cut_dex; // "" dex point
        float cut_int; // "" int point
        float cut_per; // "" per point

        float hit_str; // bonus to-hit to add per str point
        float hit_dex; // "" dex point
        float hit_int; // "" int point
        float hit_per; // "" per point

};

class ma_buff
{
    public:
        ma_buff();

        // utility function so to prevent duplicate buff copies, we use this
        // instead of add_disease (since all buffs have the same distype)
        void apply_buff( player &u );

        // given a player's state, does this bonus apply to him?
        bool is_valid_player(player &u);

        // apply static bonuses to a player
        void apply_player(player &u);

        // returns the stat bonus for the on-hit stat (for rolls)
        int hit_bonus(player &u);
        int dodge_bonus(player &u);
        int speed_bonus(player &u);
        int block_bonus(player &u);

        // returns the armor bonus for various armor stats (equivalent to armor)
        int arm_bash_bonus(player &u);
        int arm_cut_bonus(player &u);

        // returns the stat bonus for the various damage stats (for rolls)
        int bash_bonus(player &u);
        int cut_bonus(player &u);

        // returns damage multipliers for the various damage stats (applied after
        // bonuses)
        float bash_mult();
        float cut_mult();

        // returns various boolean flags
        bool is_throw_immune();
        bool is_quiet();
        bool can_melee();

        // The ID of the effect that is used to store this buff
        std::string get_effect_id() const;
        // If the effects represents an ma_buff effect, return the ma_buff, otherwise retur null.
        static ma_buff *from_effect( const effect &eff );

        std::string id;
        std::string name;
        std::string description;

        ma_requirements reqs;

        // mapped as buff_id -> min stacks of buff

        int buff_duration; // total length this buff lasts
        int max_stacks; // total number of stacks this buff can have

        int dodges_bonus; // extra dodges, like karate
        int blocks_bonus; // extra blocks, like karate

        int arm_bash; // passive bonus to bash armor
        int arm_cut; // passive bonus to cut armor

        int hit; // flat bonus to hit
        int bash; // flat bonus to bash
        int cut; // flat bonus to cut
        int dodge; // flat dodge bonus
        int speed; // flat speed bonus
        int block; // unarmed block damage reduction

        float bash_stat_mult; // bash damage multiplier, like aikido
        float cut_stat_mult; // cut damage multiplier

        float bash_str; // bonus damage to add per str point
        float bash_dex; // "" dex point
        float bash_int; // "" int point
        float bash_per; // "" per point

        float cut_str; // bonus cut damage to add per str point
        float cut_dex; // "" dex point
        float cut_int; // "" int point
        float cut_per; // "" per point

        float hit_str; // bonus to-hit to add per str point
        float hit_dex; // "" dex point
        float hit_int; // "" int point
        float hit_per; // "" per point

        float dodge_str; // bonus dodge to add per str point
        float dodge_dex; // "" dex point
        float dodge_int; // "" int point
        float dodge_per; // "" per point

        float block_str; // bonus block DR per str point
        float block_dex; // "" dex point
        float block_int; // "" int point
        float block_per; // "" per point

        bool quiet;
        bool melee_allowed;
        bool throw_immune; // are we immune to throws/grabs?
};

class martialart
{
    public:
        martialart();

        // modifies a player's "current" stats with various types of bonuses
        void apply_static_buffs(player &u);

        void apply_onmove_buffs(player &u);

        void apply_onhit_buffs(player &u);

        void apply_onattack_buffs(player &u);

        void apply_ondodge_buffs(player &u);

        void apply_onblock_buffs(player &u);

        void apply_ongethit_buffs(player &u);

        // determines if a technique is valid or not for this style
        bool has_technique(player &u, matec_id tech);
        // determines if a weapon is valid for this style
        bool has_weapon(std::string item) const;
        // gets custom melee string for a technique under this style
        std::string melee_verb(matec_id tech, player &u);

        std::string id;
        std::string name;
        std::string description;
        int arm_block;
        int leg_block;
        bool arm_block_with_bio_armor_arms;
        bool leg_block_with_bio_armor_legs;
        std::set<matec_id> techniques; // all available techniques
        std::set<std::string> weapons; // all style weapons
        std::vector<ma_buff> static_buffs; // all buffs triggered by each condition
        std::vector<ma_buff> onmove_buffs;
        std::vector<ma_buff> onhit_buffs;
        std::vector<ma_buff> onattack_buffs;
        std::vector<ma_buff> ondodge_buffs;
        std::vector<ma_buff> onblock_buffs;
        std::vector<ma_buff> ongethit_buffs;

};

void load_technique(JsonObject &jo);
void load_martial_art(JsonObject &jo);
void check_martialarts();
void clear_techniques_and_martial_arts();
void finialize_martial_arts();

extern std::map<matype_id, martialart> martialarts;
extern std::map<mabuff_id, ma_buff> ma_buffs;
extern std::map<matec_id, ma_technique> ma_techniques;

#endif
