/* tnnt.h - file to hold structs, constants, and achievements added to support
 * The November NetHack Tournament. */

#ifndef TNNT_H
#define TNNT_H

/* TNNT specific achievements.
 * Does not contain achievements which will be in TNNT but are already
 * represented in the 'achieve' xlogfile field, such as killing Medusa.
 */
#define TNNT_ACH_ENUM
#define NO_TNNT_ACHIEVEMENT -1
enum tnnt_achievements {
#include "tnnt_achievements.h"
    NUM_TNNT_ACHIEVEMENTS
};
#undef TNNT_ACH_ENUM

/* Achievement names and descriptions */
struct tnnt_achievement_strings {
    const char* name;
    const char* descr;
};

/* Generic globals that don't belong in you.h */
struct tnnt_globals_t {
    /* A note on #defines:
     * Use one if it's a bitmask of some sort, or if it will be used in at least
     * 2 places and so prevent magic-number repetition, but not if it's a simple
     * "increment counter to at least N" achievement which will only need the
     * number once.
     * Defines supporting an achievement should come BEFORE any variables
     * supporting that achievement, for style consistency.
     */

    struct tnnt_ach_statusfields {
        enum tnnt_ach_statuses {
            ACH_NOT_EARNED = 0,
            ACH_EARNED_IN_PREVIOUS_GAME = 1,
            ACH_EARNED_THIS_GAME = 2
        } status;
        /* when achievement was just earned, flag it with this to give a
         * deferred "achievement unlocked" message at the end of the current
         * player input round */
        boolean needs_notification;
    } ach_status[NUM_TNNT_ACHIEVEMENTS];

    /* The actual achievement bitfields which get written out to xlogfile and
     * track whether or not you have achieved something. Only achievements
     * earned in this game, ignoring the ones from prior games.
     *
     * Since there are more than 64 achievements, we need multiple 64 bit ints.
     *
     * As of 2025, this is now redundant with ach_status[i].status, but
     * is still kept around because it wouldn't be performant for various bits
     * of code that need to write or show it to loop over all the achievements.
     */
    uint64_t achievement_bitmap[(NUM_TNNT_ACHIEVEMENTS / 64) + 1];

    /* Various achievement counters */
    unsigned char graffiti_found;
#define MAX_TAMED_FELINES 7
    unsigned char felines_tamed; /* redundant; for #tnntstats */
    unsigned char high_altars; /* bitmask using regular AM_* masks */
    unsigned char regular_altars;

    /* More highly unstable code. This relies on the "produce" foods being the
     * 12th through 22nd foods, inclusive, defined in objects.c. Yay!
     * The expectation here is that 0x1 = first food type defined in objects.c,
     * 0x2 = second defined, etc. */
#define FOODMASK_ALL     0x1FFFFFFFF /* all foods, period */
#define FOODMASK_PRODUCE 0x0003FF800 /* all fruits & veggies */
#define FOODMASK_TIN     0x100000000 /* only tin */
#define FOODMASK_GLOBS   0x000000780 /* all types of glob (only need one) */
    uint64_t foods_eaten; /* sadly there are 33 foods */

    unsigned short genderflips;

/* TNNT TODO FOR 3.7: The Amulet of Yendor will grant a wish when picked up. Do
 * we add it to this list?
 * Also, should Vlad's throne count separately from a normal throne because its
 * wish is guaranteed? */
#define WISHSRC_WAND         0x01
#define WISHSRC_LAMP         0x02
#define WISHSRC_SMOKY_POTION 0x04
#define WISHSRC_THRONE       0x08
#define WISHSRC_WATER_DEMON  0x10
#define WISHSRC_ALL          0x1F
    unsigned char wish_sources;

    boolean blew_up_ludios; /* blew up any of its land mines */

#define RDBL_SCROLL  0x0001
#define RDBL_BOOK    0x0002
#define RDBL_COIN    0x0004
/* TNNT TODO FOR 3.7: Hawaiian shirt can be read, but doesn't actually contain
 * text. We should decide if we want to count them (and change the achievement
 * description to "shirt") or explicitly keep it as only T-shirt (and make this
 * constant RDBL_T_SHIRT). */
#define RDBL_SHIRT   0x0008
#define RDBL_MARKER  0x0010
#define RDBL_CANDY   0x0020
#define RDBL_CARD    0x0040
#define RDBL_COOKIE  0x0080
#define RDBL_APRON   0x0100
#define ALL_READABLE 0x01FF
    /* Book of the Dead and Orb of Fate are also readable items, but are not
     * counted towards the "read all readable items" achievement. */
    unsigned short readables;

    /* TNNT TODO FOR 3.7: this counter will no longer be needed; we will be able
     * to use u.uconduct.sokocheats */
    unsigned short soko_guilts;

    /* Devteam quest */
#define NUM_MISSING_CODE_SCROLLS 3
    xint16 missing_scroll_levels[NUM_MISSING_CODE_SCROLLS];
    enum dtquest_status {
        DTQUEST_NOTSTARTED = 0,
        DTQUEST_INPROGRESS,
        DTQUEST_COMPLETED
    } devteam_quest_status;

    /* NPC Deathmatch */
    boolean deathmatch_started;
    boolean deathmatch_completed;
    int deathmatch_prize_oid;
    unsigned int deathmatch_m_id;
#define is_deathmatch_opponent(mtmp) (mtmp->m_id == tnnt_globals.deathmatch_m_id)

    /* TNNT TODO: probably move this above, or the other achievements below,
     * the devteam/npc side quests */
    unsigned feline_m_ids[MAX_TAMED_FELINES]; /* Cat Lady: ids of tamed cats */

    /* wizards_killed increments only when the player kills the Wizard - not
     * when the wizard or an iswiz monster dies by other means, so it is not
     * suitable for "never killed the wizard" conduct tracking. */
    unsigned short wizards_killed;
    unsigned char wizkills_this_action;

    /* Inedible object classes:
     * RANDOM_CLASS, ILLOBJ_CLASS, VENOM_CLASS are obvious.
     * GEM_CLASS and ROCK_CLASS have no edible items in them.
     * POTION_CLASS... you don't *eat* potions... (or rather you can't. There is
     * code to handle eating a potion, but no polyform is capable of reaching
     * that code, so we don't require it.)
     * This variable is a bitmask: 0x1 = 1 << 0 = the 0th object class,
     * 0x2 = 1 << 1 = the 1th object class, and so on. */
#define ALL_EDIBLE_OCLASSES 0x19EFC
    uint32_t objclasses_eaten;

    unsigned int astral_worm_gulps;
    unsigned short artis_eaten;
    /* ordered by plane: earth, air, fire, water */
#define TNNT_ELEMENTAL_KILL_THRESHOLD 4
    unsigned short elementals_killed_on_planes[4];

#define TNNT_LITROOM_GOAL 20
    /* 20 entries of ledger_no and roomno */
    struct {
        unsigned short ledgerno;
        unsigned short roomno;
    } dark_rooms_lit[TNNT_LITROOM_GOAL];

    unsigned short mysterious_forced_back;
    unsigned short num_planes_pets;
    unsigned int* planes_pet_m_ids; /* dynamically allocated entering Earth */

    /* #untrap-able trap types: arrow, dart, squeaky board, bear, landmine, web.
     * Also door and container traps.
     * There are ways to get rid of others but these are the only ones you can
     * actually use #untrap to get rid of. Helping a monster out of a trap is
     * not the point of this achievement, either, so doing that on pits and such
     * don't count. */
#define TNNT_UNTRAP_ARROW    0x0001
#define TNNT_UNTRAP_DART     0x0002
#define TNNT_UNTRAP_BOARD    0x0004
#define TNNT_UNTRAP_BEARTRAP 0x0008
#define TNNT_UNTRAP_LANDMINE 0x0010
#define TNNT_UNTRAP_WEB      0x0020
#define TNNT_UNTRAP_DOOR     0x0040
#define TNNT_UNTRAP_CONT     0x0080
#define ALL_UNTRAPPABLE_TRAPTYPES 0x00FF
    uint16_t untrapped_types;

    /* mthrowu.c tracks an 'archer' variable, but for some reason only for a
     * monster deliberately shooting at another monster. This does the same for
     * monster shooting at the player, for the "friendly fire" achievement. */
    struct monst *psuedo_archer;
    unsigned char consecutive_door_resists;
    unsigned char door_resist_max;
    coordxy door_attempt_x, door_attempt_y;
    xint16 door_attempt_ledger;
    unsigned char bones_piles_found;
    unsigned char maxpw_from_corpses;
    unsigned nazgul_ring_o_ids[9];
    boolean nemesis_got_a_move;
    int genocides; /* # of times genocide was done, rather than number of
                      species */

#define TNNT_CASTLE_TURNS 30
    long entered_castle_time;

#define ALL_POTS_DRUNK 0x1ffffff /* not currently used */
    /* considered harmful: confusion, blindness, paralysis, hallucination,
     * sleep, booze, sickness, acid */
#define HARMFUL_DRUNK  0x0b2009c
    uint32_t pots_drunk;
#define tnnt_pot_is_harmful(otyp) \
    ((HARMFUL_DRUNK & (1UL << ((otyp) - svb.bases[POTION_CLASS]))) != 0UL)

#define FIRST_POLEARM PARTISAN
#define LAST_POLEARM BEC_DE_CORBIN
    /* Polearms (as we count them for this achievement) are not a contiguous
     * block, hence why this does not have all lower bits set. */
#define ALL_POLEARMS_FOUND 0x00003EEF
    uint32_t polearms_found;

    boolean killed_izchak;
    coord kitten_loc;
    boolean minetown_bereft_of_watch;

    /* Extra tracking for the "never killed X" conducts.
     * The reason we need this is because the hero can slime or polymorph these
     * foes, and kill the resulting monster; this will not treat the original
     * monster as vanquished, so simply checking mvitals[...].died is
     * insufficient.
     */
#define NUM_UNIQUES_TRACKED 7 /* nemesis, Vlad, Wizard, HPoM, 3 Riders */
    struct {
        int mndx; /* PM_ constant, initialized in newgame() */
        unsigned m_id;
        boolean died;
    } unique_info[NUM_UNIQUES_TRACKED];

    boolean lifesaved_this_turn;
    long turns_entered_last_plane;
    boolean too_long_on_planes;

#define LEVEL_NOT_GENERATED 0
#define LEVEL_HAS_NO_VAULT 1
#define VAULT_NOT_ENTERED 2
#define VAULT_ENTERED 3
    short vault_status[30]; /* indexes into this are u.uz.dlevel, only relevant
                               in Dungeons of Doom; has a little extra space */

    uchar non_wish_djinni;

    /* these are conduct-ish achievements but not actual conducts since they
     * don't require being done for the whole game. So we track them here. */
    boolean u_backtracked; /* revisited any level */
    boolean u_entered_soko;
    boolean u_backtracked_after_soko; /* can only be set after u_entered_soko is
                                         true */
    boolean non_downstairs_move_in_hell;
    boolean unforeseen_fall; /* so if someone hits a trap door in Gehennom they
                                legitimately didn't know about, it won't break the
                                achievement - the alternative would be
                                establishing a "safe" way to do this by
                                methodically checking every level for traps
                                which would be annoying */

    uchar swapchest1_dlevel;
    boolean ach_needs_notification; /* this flag is so we don't have to loop
                                       over all achievements every player action
                                       to see if any are newly achieved */
    xint8 total_bounces; /* extracted from dobuzz() after part of it that
                            manipulated former local total_bounces variable was
                            split out into a different function */
#define FIRST_DLORD PM_JUIBLEX
    uchar dlords_on_level; /* extracted after movemon/movemon_singlemon split,
                              to avoid having to loop over fmon twice */


    /* tnnt devs: add more as needed */
};

/* Any misc defines that do get reused in multiple places, but aren't attached
 * to a counter above */
#define TNNT_OHKO_DMG 20

#endif /* TNNT_H */
