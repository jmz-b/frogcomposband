#include "angband.h"

static int _get_powers(spell_info* spells, int max)
{
    int ct = 0;
    spell_info* spell = &spells[ct++];

    if (is_good_realm(p_ptr->realm1))
    {
        spell->level = 35;
        spell->cost = 70;
        spell->fail = calculate_fail_rate(spell->level, 90, p_ptr->stat_ind[A_WIS]);
        spell->fn = bless_weapon_spell;
    }
    else
    {
        spell->level = 42;
        spell->cost = 40;
        spell->fail = calculate_fail_rate(spell->level, 80, p_ptr->stat_ind[A_WIS]);
        spell->fn = evocation_spell;
    }

    return ct;
}

static caster_info * _caster_info(void)
{
    static caster_info me = {0};
    static bool init = FALSE;
    if (!init)
    {
        me.magic_desc = "prayer";
        me.which_stat = A_WIS;
        me.weight = 430;
        me.options = CASTER_ALLOW_DEC_MANA;
        init = TRUE;
    }
    return &me;
}

static void _calc_weapon_bonuses(object_type *o_ptr, weapon_info_t *info_ptr)
{
    if (o_ptr->tval == TV_SWORD || o_ptr->tval == TV_POLEARM)
    {
        u32b flgs[OF_ARRAY_SIZE];
        obj_flags(o_ptr, flgs);
        if (have_flag(flgs, OF_BLESSED))
        {
        }
        else if (is_evil_realm(p_ptr->realm1))
        {
        }
        else
        {
            info_ptr->to_h -= 2;
            info_ptr->dis_to_h -= 2;

            info_ptr->to_d -= 2;
            info_ptr->dis_to_d -= 2;

            info_ptr->icky_wield = TRUE;
        }
    }
}

bool priest_is_good(void)
{
    if (p_ptr->pclass == CLASS_PRIEST && is_good_realm(p_ptr->realm1))
        return TRUE;
    return FALSE;
}

bool priest_is_evil(void)
{
    if (p_ptr->pclass == CLASS_PRIEST && is_evil_realm(p_ptr->realm1))
        return TRUE;
    return FALSE;
}

class_t *priest_get_class(void)
{
    static class_t me = {0};
    static bool init = FALSE;

    if (!init)
    {           /* dis, dev, sav, stl, srh, fos, thn, thb */
    skills_t bs = { 25,  35,  40,   2,  16,   8,  48,  35};
    skills_t xs = {  7,  11,  12,   0,   0,   0,  13,  11};

        me.name = "Priest";
        me.desc = "A Priest is a character devoted to serving a higher power. They "
                    "explore the dungeon in the service of their God. They are fairly "
                    "familiar with magical devices which they believe act as foci for divine "
                    "intervention in the natural order of things.\n \n"
                    "There are two types of priests: Good and Evil. If the priest chooses Life "
                    "or Crusade as their first realm, then they will follow the path of good. "
                    "As such, they may not choose an evil realm for their second realm. Also, good "
                    "priests abhor bloodshed, and therefore are not comfortable with edged weapons. "
                    "Wielding one will disrupt their ability to concentrate during prayers. "
                    "Should a priest choose Death or Daemon as their first realm, however, then "
                    "they will serve an evil god. As such, they actually enjoy shedding blood, and "
                    "suffer no such weapon restriction. Of course, evil priests abhor good things, "
                    "and are unable to choose Life of Crusade for their second realm.\n \n"
                    "Good priests have a strong affinity for Life prayers, and learn them very well, even better "
                    "than a High Mage. Conversely, evil priests favor prayers of Death, and receive "
                    "strong bonuses when choosing this foul realm. Otherwise, priests learn spells less "
                    "efficiently than a Mage would. Also, they may not choose which spell to learn, but "
                    "are granted new prayers by the whim of their deity, presumably in order to serve "
                    "some greater divine plan of which the priest is not fully cognizant. The priest's "
                    "primary prayer stat is Wisdom.";

        me.stats[A_STR] = -1;
        me.stats[A_INT] = -3;
        me.stats[A_WIS] =  3;
        me.stats[A_DEX] = -1;
        me.stats[A_CON] =  0;
        me.stats[A_CHR] =  2;
        me.base_skills = bs;
        me.extra_skills = xs;
        me.life = 100;
        me.base_hp = 4;
        me.exp = 120;
        me.pets = 35;

        me.caster_info = _caster_info;
        /* TODO: This class uses spell books, so we are SOL
        me.get_spells = _get_spells;*/
        me.get_powers = _get_powers;
        me.calc_weapon_bonuses = _calc_weapon_bonuses;
        me.character_dump = spellbook_character_dump;
        init = TRUE;
    }

    return &me;
}
