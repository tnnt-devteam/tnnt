/* NetHack 3.6  decl.h  $NHDT-Date: 1573869061 2019/11/16 01:51:01 $  $NHDT-Branch: NetHack-3.6 $:$NHDT-Revision: 1.165 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Michael Allison, 2007. */
/* NetHack may be freely redistributed.  See license for details. */

#ifndef DECL_H
#define DECL_H

#define E extern

E int NDECL((*occupation));
E int NDECL((*afternmv));

E const char *hname;
E int hackpid;
#if defined(UNIX) || defined(VMS)
E int locknum;
#endif
#ifdef DEF_PAGER
E char *catmore;
#endif /* DEF_PAGER */

E char SAVEF[];
#ifdef MICRO
E char SAVEP[];
#endif

/* max size of a windowtype option */
#define WINTYPELEN 16
E char chosen_windowtype[WINTYPELEN];

E NEARDATA int bases[MAXOCLASSES];

E NEARDATA int multi;
E const char *multi_reason;
E NEARDATA int nroom;
E NEARDATA int nsubroom;
E NEARDATA int occtime;

#define WARNCOUNT 6 /* number of different warning levels */
E nhsym warnsyms[WARNCOUNT];
E NEARDATA int warn_obj_cnt; /* count of monsters meeting criteria */

E int x_maze_max, y_maze_max;
E int otg_temp;

E NEARDATA int in_doagain;

E struct dgn_topology { /* special dungeon levels for speed */
    d_level d_oracle_level;
    d_level d_bigroom_level; /* unused */
    d_level d_rogue_level;
    d_level d_medusa_level;
    d_level d_stronghold_level;
    d_level d_valley_level;
    d_level d_wiz1_level;
    d_level d_wiz2_level;
    d_level d_wiz3_level;
    d_level d_juiblex_level;
    d_level d_orcus_level;
    d_level d_baalzebub_level; /* unused */
    d_level d_asmodeus_level;  /* unused */
    d_level d_portal_level;    /* only in goto_level() [do.c] */
    d_level d_sanctum_level;
    d_level d_earth_level;
    d_level d_water_level;
    d_level d_fire_level;
    d_level d_air_level;
    d_level d_astral_level;
    xchar d_tower_dnum;
    xchar d_sokoban_dnum;
    xchar d_mines_dnum, d_quest_dnum;
    d_level d_qstart_level, d_qlocate_level, d_nemesis_level;
    d_level d_knox_level;
    d_level d_devteam_level;
    d_level d_deathmatch_level;
    d_level d_mineend_level;
    d_level d_sokoend_level;
} dungeon_topology;
/* macros for accessing the dungeon levels by their old names */
/* clang-format off */
#define oracle_level            (dungeon_topology.d_oracle_level)
#define bigroom_level           (dungeon_topology.d_bigroom_level)
#define rogue_level             (dungeon_topology.d_rogue_level)
#define medusa_level            (dungeon_topology.d_medusa_level)
#define stronghold_level        (dungeon_topology.d_stronghold_level)
#define valley_level            (dungeon_topology.d_valley_level)
#define wiz1_level              (dungeon_topology.d_wiz1_level)
#define wiz2_level              (dungeon_topology.d_wiz2_level)
#define wiz3_level              (dungeon_topology.d_wiz3_level)
#define juiblex_level           (dungeon_topology.d_juiblex_level)
#define orcus_level             (dungeon_topology.d_orcus_level)
#define baalzebub_level         (dungeon_topology.d_baalzebub_level)
#define asmodeus_level          (dungeon_topology.d_asmodeus_level)
#define portal_level            (dungeon_topology.d_portal_level)
#define sanctum_level           (dungeon_topology.d_sanctum_level)
#define earth_level             (dungeon_topology.d_earth_level)
#define water_level             (dungeon_topology.d_water_level)
#define fire_level              (dungeon_topology.d_fire_level)
#define air_level               (dungeon_topology.d_air_level)
#define astral_level            (dungeon_topology.d_astral_level)
#define tower_dnum              (dungeon_topology.d_tower_dnum)
#define sokoban_dnum            (dungeon_topology.d_sokoban_dnum)
#define mines_dnum              (dungeon_topology.d_mines_dnum)
#define quest_dnum              (dungeon_topology.d_quest_dnum)
#define qstart_level            (dungeon_topology.d_qstart_level)
#define qlocate_level           (dungeon_topology.d_qlocate_level)
#define nemesis_level           (dungeon_topology.d_nemesis_level)
#define knox_level              (dungeon_topology.d_knox_level)
#define devteam_level           (dungeon_topology.d_devteam_level)
#define deathmatch_level        (dungeon_topology.d_deathmatch_level)
#define mineend_level           (dungeon_topology.d_mineend_level)
#define sokoend_level           (dungeon_topology.d_sokoend_level)
/* clang-format on */

E NEARDATA stairway dnstair, upstair; /* stairs up and down */
#define xdnstair (dnstair.sx)
#define ydnstair (dnstair.sy)
#define xupstair (upstair.sx)
#define yupstair (upstair.sy)

E NEARDATA stairway dnladder, upladder; /* ladders up and down */
#define xdnladder (dnladder.sx)
#define ydnladder (dnladder.sy)
#define xupladder (upladder.sx)
#define yupladder (upladder.sy)

E NEARDATA stairway sstairs;

E NEARDATA dest_area updest, dndest; /* level-change destination areas */

E NEARDATA coord inv_pos;
E NEARDATA dungeon dungeons[];
E NEARDATA s_level *sp_levchn;
#define dunlev_reached(x) (dungeons[(x)->dnum].dunlev_ureached)

#include "quest.h"
E struct q_score quest_status;

E NEARDATA char pl_character[PL_CSIZ];
E NEARDATA char pl_race; /* character's race */

E NEARDATA char pl_fruit[PL_FSIZ];
E NEARDATA struct fruit *ffruit;

E NEARDATA char tune[6];

#define MAXLINFO (MAXDUNGEON * MAXLEVEL)
E struct linfo level_info[MAXLINFO];

E NEARDATA struct sinfo {
    int gameover;  /* self explanatory? */
    int stopprint; /* inhibit further end of game disclosure */
#ifdef HANGUPHANDLING
    volatile int done_hup; /* SIGHUP or moral equivalent received
                            * -- no more screen output */
    int preserve_locks;    /* don't remove level files prior to exit */
#endif
    int something_worth_saving; /* in case of panic */
    int panicking;              /* `panic' is in progress */
    int exiting;                /* an exit handler is executing */
    int in_moveloop;
    int in_impossible;
#ifdef PANICLOG
    int in_paniclog;
#endif
    int wizkit_wishing;
} program_state;

E boolean restoring;
E boolean ransacked;

E const char quitchars[];
E const char vowels[];
E const char ynchars[];
E const char ynqchars[];
E const char ynaqchars[];
E const char ynNaqchars[];
E NEARDATA long yn_number;

E const char disclosure_options[];

E NEARDATA int smeq[];
E NEARDATA int doorindex;
E NEARDATA char *save_cm;

E NEARDATA struct kinfo {
    struct kinfo *next; /* chain of delayed killers */
    int id;             /* uprop keys to ID a delayed killer */
    int format;         /* one of the killer formats */
#define KILLED_BY_AN 0
#define KILLED_BY 1
#define NO_KILLER_PREFIX 2
    char name[BUFSZ]; /* actual killer name */
} killer;

#ifdef WHEREIS_FILE
E char whereis_real_path[255];
#endif

E long done_money;
E NEARDATA char plname[PL_NSIZ];
E NEARDATA char dogname[];
E NEARDATA char catname[];
E NEARDATA char horsename[];
E char preferred_pet;
E const char *occtxt; /* defined when occupation != NULL */
E const char *nomovemsg;
E char lock[];

E const schar xdir[], ydir[], zdir[];

E NEARDATA schar tbx, tby; /* set in mthrowu.c */

E NEARDATA struct multishot {
    int n, i;
    short o;
    boolean s;
} m_shot;

E NEARDATA long moves, monstermoves;
E NEARDATA long wailmsg;

E NEARDATA boolean in_mklev;
E NEARDATA boolean stoned;
E NEARDATA boolean unweapon;
E NEARDATA boolean mrg_to_wielded;
E NEARDATA boolean defer_see_monsters;

E NEARDATA boolean in_steed_dismounting;
E NEARDATA boolean has_strong_rngseed;

E const int shield_static[];

#include "spell.h"
E NEARDATA struct spell spl_book[]; /* sized in decl.c */

#include "color.h"
#ifdef TEXTCOLOR
E const int zapcolors[];
#endif

E const struct class_sym def_oc_syms[MAXOCLASSES]; /* default class symbols */
E uchar oc_syms[MAXOCLASSES];                      /* current class symbols */
E const struct class_sym def_monsyms[MAXMCLASSES]; /* default class symbols */
E uchar monsyms[MAXMCLASSES];                      /* current class symbols */

#include "obj.h"
E NEARDATA struct obj *invent, *uarm, *uarmc, *uarmh, *uarms, *uarmg, *uarmf,
    *uarmu, /* under-wear, so to speak */
    *uskin, *uamul, *uleft, *uright, *ublindf, *uwep, *uswapwep, *uquiver;

E NEARDATA struct obj *uchain; /* defined only when punished */
E NEARDATA struct obj *uball;
E NEARDATA struct obj *migrating_objs;
E NEARDATA struct obj *billobjs;
E NEARDATA struct obj *current_wand, *thrownobj, *kickedobj;

E NEARDATA const struct obj zeroobj; /* for init; also, &zeroobj is used
                                      * as special value */

E NEARDATA const anything zeroany;   /* init'd and defined in decl.c */

#include "you.h"
E NEARDATA struct you u;
E NEARDATA time_t ubirthday;
E NEARDATA struct u_realtime urealtime;

#include "onames.h"
#ifndef PM_H /* (pm.h has already been included via youprop.h) */
#include "pm.h"
#endif

E NEARDATA const struct monst zeromonst; /* for init of new or temp monsters */
E NEARDATA struct monst youmonst; /* monster details when hero is poly'd */
E NEARDATA struct monst *mydogs, *migrating_mons;

E NEARDATA struct mvitals {
    uchar born;
    uchar died;
    uchar mvflags;
    /* TNNT: Monsters the player has personally killed. Should be <= died. */
    uchar ukilled;
} mvitals[NUMMONS];

E NEARDATA long domove_attempting;
E NEARDATA long domove_succeeded;
#define DOMOVE_WALK         0x00000001
#define DOMOVE_RUSH         0x00000002

E NEARDATA struct c_color_names {
    const char *const c_black, *const c_amber, *const c_golden,
        *const c_light_blue, *const c_red, *const c_green, *const c_silver,
        *const c_blue, *const c_purple, *const c_white, *const c_orange;
} c_color_names;
#define NH_BLACK c_color_names.c_black
#define NH_AMBER c_color_names.c_amber
#define NH_GOLDEN c_color_names.c_golden
#define NH_LIGHT_BLUE c_color_names.c_light_blue
#define NH_RED c_color_names.c_red
#define NH_GREEN c_color_names.c_green
#define NH_SILVER c_color_names.c_silver
#define NH_BLUE c_color_names.c_blue
#define NH_PURPLE c_color_names.c_purple
#define NH_WHITE c_color_names.c_white
#define NH_ORANGE c_color_names.c_orange

/* The names of the colors used for gems, etc. */
E const char *c_obj_colors[];

E struct c_common_strings {
    const char *const c_nothing_happens, *const c_thats_enough_tries,
        *const c_silly_thing_to, *const c_shudder_for_moment,
        *const c_something, *const c_Something, *const c_You_can_move_again,
        *const c_Never_mind, *c_vision_clears, *const c_the_your[2],
        *const c_fakename[2];
} c_common_strings;
#define nothing_happens c_common_strings.c_nothing_happens
#define thats_enough_tries c_common_strings.c_thats_enough_tries
#define silly_thing_to c_common_strings.c_silly_thing_to
#define shudder_for_moment c_common_strings.c_shudder_for_moment
#define something c_common_strings.c_something
#define Something c_common_strings.c_Something
#define You_can_move_again c_common_strings.c_You_can_move_again
#define Never_mind c_common_strings.c_Never_mind
#define vision_clears c_common_strings.c_vision_clears
#define the_your c_common_strings.c_the_your
/* fakename[] used occasionally so vtense() won't be fooled by an assigned
   name ending in 's' */
#define fakename c_common_strings.c_fakename

/* material strings */
E const char *materialnm[];

/* Monster name articles */
#define ARTICLE_NONE 0
#define ARTICLE_THE 1
#define ARTICLE_A 2
#define ARTICLE_YOUR 3

/* Monster name suppress masks */
#define SUPPRESS_IT 0x01
#define SUPPRESS_INVISIBLE 0x02
#define SUPPRESS_HALLUCINATION 0x04
#define SUPPRESS_SADDLE 0x08
#define EXACT_NAME 0x0F
#define SUPPRESS_NAME 0x10

/* Vision */
E NEARDATA boolean vision_full_recalc; /* TRUE if need vision recalc */
E NEARDATA char **viz_array;           /* could see/in sight row pointers */

/* Window system stuff */
E NEARDATA winid WIN_MESSAGE;
E NEARDATA winid WIN_STATUS;
E NEARDATA winid WIN_MAP, WIN_INVEN;

/* pline (et al) for a single string argument (suppress compiler warning) */
#define pline1(cstr) pline("%s", cstr)
#define Your1(cstr) Your("%s", cstr)
#define You1(cstr) You("%s", cstr)
#define verbalize1(cstr) verbalize("%s", cstr)
#define You_hear1(cstr) You_hear("%s", cstr)
#define Sprintf1(buf, cstr) Sprintf(buf, "%s", cstr)
#define panic1(cstr) panic("%s", cstr)

E char toplines[];
#ifndef TCAP_H
E struct tc_gbl_data {   /* also declared in tcap.h */
    char *tc_AS, *tc_AE; /* graphics start and end (tty font swapping) */
    int tc_LI, tc_CO;    /* lines and columns */
} tc_gbl_data;
#define AS tc_gbl_data.tc_AS
#define AE tc_gbl_data.tc_AE
#define LI tc_gbl_data.tc_LI
#define CO tc_gbl_data.tc_CO
#endif

/* xxxexplain[] is in drawing.c */
E const char *const monexplain[], invisexplain[], *const oclass_names[];

/* Some systems want to use full pathnames for some subsets of file names,
 * rather than assuming that they're all in the current directory.  This
 * provides all the subclasses that seem reasonable, and sets up for all
 * prefixes being null.  Port code can set those that it wants.
 */
#define HACKPREFIX 0
#define LEVELPREFIX 1
#define SAVEPREFIX 2
#define BONESPREFIX 3
#define DATAPREFIX 4 /* this one must match hardcoded value in dlb.c */
#define SCOREPREFIX 5
#define LOCKPREFIX 6
#define SYSCONFPREFIX 7
#define CONFIGPREFIX 8
#define TROUBLEPREFIX 9
#define PREFIX_COUNT 10
/* used in files.c; xxconf.h can override if needed */
#ifndef FQN_MAX_FILENAME
#define FQN_MAX_FILENAME 512
#endif

#if defined(NOCWD_ASSUMPTIONS) || defined(VAR_PLAYGROUND)
/* the bare-bones stuff is unconditional above to simplify coding; for
 * ports that actually use prefixes, add some more localized things
 */
#define PREFIXES_IN_USE
#endif

E char *fqn_prefix[PREFIX_COUNT];
#ifdef WIN32
E boolean fqn_prefix_locked[PREFIX_COUNT];
#endif
#ifdef PREFIXES_IN_USE
E const char *fqn_prefix_names[PREFIX_COUNT];
#endif

E NEARDATA struct savefile_info sfcap, sfrestinfo, sfsaveinfo;

struct opvar {
    xchar spovartyp; /* one of SPOVAR_foo */
    union {
        char *str;
        long l;
    } vardata;
};

struct autopickup_exception {
    struct nhregex *regex;
    char *pattern;
    boolean grab;
    struct autopickup_exception *next;
};
E struct autopickup_exception *apelist;

struct plinemsg_type {
    xchar msgtype;  /* one of MSGTYP_foo */
    struct nhregex *regex;
    char *pattern;
    struct plinemsg_type *next;
};

#define MSGTYP_NORMAL   0
#define MSGTYP_NOREP    1
#define MSGTYP_NOSHOW   2
#define MSGTYP_STOP     3
#define MSGTYP_ALERT    4
/* bitmask for callers of hide_unhide_msgtypes() */
#define MSGTYP_MASK_REP_SHOW ((1 << MSGTYP_NOREP) | (1 << MSGTYP_NOSHOW))

E struct plinemsg_type *plinemsg_types;

enum bcargs {override_restriction = -1};
struct breadcrumbs {
    const char *funcnm;
    int linenum;
    boolean in_effect;
};

#ifdef PANICTRACE
E const char *ARGV0;
#endif

enum earlyarg {ARG_DEBUG, ARG_VERSION, ARG_SHOWPATHS
#ifdef WIN32
    ,ARG_WINDOWS
#endif
};

struct early_opt {
    enum earlyarg e;
    const char *name;
    int minlength;
    boolean valallowed;
};

/* TNNT specific achievements.
 * Does not contain achievements which will be in TNNT but are already
 * represented in the 'achieve' xlogfile field, such as killing Medusa.
 * The order of these MUST be the exact same as that of tnnt_achievements in
 * decl.c! If you reorder these, also reorder those.
 * Also, avoid inserting blank lines or comment lines in the middle of this
 * enum. Makes it harder to verify we have as many achievements as we think :>
 */
enum tnnt_achievements {
    A_FOUND_MINES_ALTAR = 0,  /* begin tnntachieve0 */
    A_CONSULTED_ORACLE,
    A_STARTED_QUEST,
    A_LEARNED_PASSTUNE,
    A_USED_PASSTUNE,
    A_BLEW_UP_DRAWBRIDGE,
    A_CROWNED,
    A_REACHED_VIBRATING_SQ,
    A_PUT_INTO_SWAPCHEST,
    A_TALKED_TO_DEVTEAM,
    A_FINISHED_DEVTEAM_QUEST,
    A_NPC_DEATHMATCH,
    A_KILLED_ASMODEUS,
    A_KILLED_BAALZEBUB,
    A_KILLED_ORCUS,
    A_KILLED_JUIBLEX,
    A_KILLED_DEMOGORGON,
    A_KILLED_YEENOGHU,
    A_KILLED_GERYON,
    A_KILLED_DISPATER,
    A_KILLED_DEATH,
    A_KILLED_PESTILENCE,
    A_KILLED_FAMINE,
    A_KILLED_20_WIZARDS,
    A_WIZ_DOUBLE_KILL,
    A_KILLED_CROESUS,
    A_KILLED_MOLOCH_PRIEST,
    A_KILLED_9_NAZGUL,
    A_KILLED_3_ERINYES,
    A_AVENGED_ORCTOWN,
    A_CLEARED_BIGROOM,
    A_KILLED_4X4_ELEMENTALS,
    A_KILLED_GHOST,
    A_KILLED_DRAGON,
    A_KILLED_SOLDIER_ANT,
    A_KILLED_TAMED_WOODCHUCK,
    A_KILLED_ARCHON,
    A_KILLED_25_SPECIES,
    A_KILLED_50_SPECIES,
    A_KILLED_100_SPECIES,
    A_KILLED_200_SPECIES,
    A_KILLED_ALL_SPECIES,
    A_KILLED_A_Z_LOWERCASE,
    A_KILLED_A_Z_UPPERCASE,
    A_USED_CORRECT_BANE,
    A_KILLED_WITH_HOSE,
    A_DESTROYED_VIA_TURN,
    A_LIT_UP_GREMLIN,
    A_GASSED_MONSTER,
    A_KILLED_WHILE_IN_PIT,
    A_KILLED_INVISIBLE,
    A_KILLED_WITH_AKLYS,
    A_KILLED_WITH_IRONBALL,
    A_INSTAPOISONED,
    A_KILLED_WITH_TRIDENT,
    A_KILLED_EYE_POLEARM,
    A_KILLED_WITH_BOTTLE,
    A_KILLED_WITH_TOWEL,
    A_MJOLLNIR_THROW_KILL,
    A_MVM_PROJECTILE_KILL,
    A_STONED_A_MONSTER,
    A_CURED_STONING,
    A_CURED_SLIMING,
    A_SURVIVED_DROWNING,        /* #64 - end tnntachieve0 */
    A_CURED_LYCANTHROPY,
    A_CURED_ILLNESS,
    A_USED_TOUCHSTONE,
    A_LIT_OIL_POTION,
    A_ID_DETECT_FOOD,
    A_IDENTIFIED_ALL_RINGS,
    A_IDENTIFIED_ALL_WANDS,
    A_IDENTIFIED_ALL_AMULETS,
    A_IDENTIFIED_ALL_BOOKS,
    A_IDENTIFIED_ALL_SCROLLS,
    A_IDENTIFIED_ALL_POTIONS,
    A_IDENTIFIED_ACCESSORIES,
    A_IDENTIFIED_ALL_GEMS,
    A_IDENTIFIED_ALL_STONES,
    A_IDENTIFIED_ALL_GLASS,
    A_PARTIAL_VEGETARIAN,
    A_PARTIAL_VEGAN,
    A_PARTIAL_FOODLESS,
    A_PARTIAL_ATHEIST,
    A_PARTIAL_WEAPONLESS,
    A_PARTIAL_PACIFIST,
    A_PARTIAL_ILLITERATE,
    A_PARTIAL_POLYPILELESS,
    A_PARTIAL_POLYSELFLESS,
    A_PARTIAL_WISHLESS,
    A_PARTIAL_ARTIWISHLESS,
    A_PARTIAL_ZEN,
    A_PARTIAL_NUDIST,
    A_PARTIAL_GENOCIDELESS,
    A_PARTIAL_ELBERETHLESS,
    A_PARTIAL_PERMAHALLU,
    A_PARTIAL_PERMADEAF,
    A_PARTIAL_ARTIFACTLESS,
    A_PARTIAL_BONESLESS,
    A_PARTIAL_SWAPCHESTLESS,
    A_PARTIAL_PETLESS,
    A_PARTIAL_SURVIVOR,
    A_ENTERED_VAULT,
    A_ENTERED_ZOO,
    A_ENTERED_THRONE_ROOM,
    A_ENTERED_LEP_HALL,
    A_ENTERED_BEEHIVE,
    A_ENTERED_GRAVEYARD,
    A_ENTERED_ANTHOLE,
    A_ENTERED_BARRACKS,
    A_ENTERED_SWAMP,
    A_ENTERED_COCKNEST,
    A_ENTERED_LUDIOS,
    A_VISITED_ALL_ALTARS,
    A_VISITED_HIGH_ALTARS,
    A_WISH_FROM_EVERYTHING,
    A_READ_ALL_READABLE,
    A_UNTRAPPED_ALL_TRAPS,
    A_ATE_ALL_PRODUCE,
    A_ATE_ALL_FOODS,
    A_ATE_EVERY_OBJCLASS,
    A_GRANTED_ARTI,
    A_GIFT_ON_ASTRAL,
    A_GOT_EXCALIBUR,
    A_ATE_ROYAL_JELLY,
    A_GOT_CORPSE_INTRINSIC,
    A_INTRINSIC_FROM_JEWELRY,
    A_STUNNED_BAT_MEAT,
    A_TAMED_BY_FOOD,            /* #128 - end tnntachieve1 */
    A_TAMED_NOT_BY_FOOD,
    A_TAMED_SHAPECHANGER,
    A_TAMED_7_CATS,
    A_FULLY_RAISED_PET,
    A_UNTRAPPED_PET,
    A_POLYED_PET_ON_TRAP,
    A_PERMAKILLED_RIDER,
    A_BOUGHT_PROTECTION,
    A_USED_THRONE,
    A_DROPPED_RING_IN_SINK,
    A_CONVERTED_ALTAR,
    A_CHATTED_IZCHAK,
    A_KICKED_FRUIT_TREE,
    A_GOT_BEES_FROM_TREE,
    A_PURCHASED,
    A_BOUGHT_UNID_GEM,
    A_BANKRUPTED_SHOPKEEPER,
    A_ROBBED,
    A_UNCURSED_IN_FOUNTAIN,
    A_LEARNED_LV5_SPELL,
    A_CREATED_DSM,
    A_ROBBED_BY_NYMPH,
    A_ROBBED_BY_LEPRECHAUN,
    A_ENCOUNTERED_FOOCUBUS,
    A_HEALED_BY_NURSE,
    A_TRAPPED_MONSTER,
    A_GOT_UNPUNISHED,
    A_FELL_ONTO_SINK,
    A_WROTE_SCROLL,
    A_WROTE_SPELLBOOK,
    A_RODE_DRAGON,
    A_ERODEPROOFED_ITEM,
    A_THREW_UNICORN_GEM,
    A_OPENED_TIN_PROPERLY,
    A_BLOCKED_TROLL_REVIVAL,
    A_TINNED_UNIQUE_MON,
    A_USED_UP_TINNING_KIT,
    A_REVIVIFIED_CORPSE,
    A_RESURRECTED_PET_TAME,
    A_REVIVED_PERSEUS,
    A_RESURRECTED_WIZARD,
    A_ATE_MILITARY_RATION,
    A_GOT_LIFESAVED,
    A_DUG_UP_GRAVE,
    A_CAUSED_EARTHQUAKE,
    A_CHARMED_SNAKE,
    A_EVADED_BRAINSUCK,
    A_HIT_WITH_BOOMERANG,
    A_DEALT_SILVER_DAMAGE,
    A_MADE_CRYSKNIFE,
    A_WIZ_WORE_CORNUTHAUM,
    A_WORE_MITHRIL_COAT,
    A_FOUND_3_GRAFFITI,
    A_SPLIT_A_PUDDING,
    A_WORE_MEAT_RING,
    A_LOOTED_ICE_BOX,
    A_STOCKED_SIX_PACK,
    A_ANIMATED_FIGURINE,
    A_NERD,
    A_CHARGED_AN_OBJECT,
    A_KICKED_EMBEDDED_ITEM,
    A_SNAGGED_WITH_HOOK,
    A_REFILLED_CANDELABRUM,
    A_REFILLED_OIL_LAMP,        /* #192 - end tnntachieve2 */
    A_ALCHEMIZED,
    A_READ_NOVEL,
    A_TALKED_TO_DEATH,
    A_GOT_SPBOOK_FROM_STATUE,
    A_SUMMONED_NYMPH,
    A_GOT_OBJECT_UNDERWATER,
    A_RODE_WUMPUS_OVER_WATER,
    A_OPENED_SCHROEDINGERS,
    A_FREED_PRISONER,
    A_READ_MAIL,
    A_CHANGED_GENDER,
    A_MULTI_GENDER_FLIP,
    A_LAID_EGG,
    A_HATCHED_FOREIGN_EGG,
    A_FELL_DOWN_SHAFT,
    A_GOT_TWO_BITS,
    A_EXTINCTED_ARMY_OR_KOPS,
    A_DEFLECTED_NYMPH,
    A_BRIBED_DEMON_LORD,
    A_BRIBED_WITH_1,
    A_CHANGED_ALIGNMENT,
    A_HEISENBERG,
    A_SCARED_BY_GHOST,
    A_REVISITED_LOST_LEVEL,     /* TNNT TODO: this is impossible in 3.7 */
    A_DIPPED_IN_UNHOLY,
    A_DISSOLVED_IRONBARS,
    A_BROKE_WAN_NOTHING,
    A_BROKE_WAND_WISHING,
    A_CANCELED_MONSTER,
    A_NEUTRALIZED_POTION,
    A_FINISHED_WITH_50000,
    A_FINISHED_WITH_100000,
    A_APPLIED_CRYSTAL_BALL,
    A_MAXED_ATTRIBUTES,
    A_FOUND_BONES_PILE,
    A_FOUND_ROGUE_BONES_PILE,
    A_CREATED_FAMILIAR,
    A_GAVE_ADORNMENT,
    A_EXPELLED_FROM_QUEST,
    A_GOT_FOOD_APPRAISAL,
    A_SUMMONED_DEMON_LORD,
    A_TASTED_COCKATRICE,
    A_WRESTED_WOW,
    A_AMETHYST_BOOZE,
    A_SCARED_OF_SKELETON,
    A_LIED_TO_GUARD,
    A_GOT_13_LUCK,
    A_GOT_NEGATIVE_13_LUCK,
    A_GREASED_ARMOR,
    A_DEATHLY_HALLOWS,
    A_DISARMED_WITH_WHIP,
    A_DISARMED_LUDIOS_MINES,
    A_UNLOCKED_CREDIT_CARD,
    A_RELEASED_CAMERA_DEMON,
    A_SCARED_WITH_MUSIC,
    A_READ_FORTUNE_FOODLESS,
    A_GATED_IN_DEMON_LORD,
    A_ENGRAVED_WITH_ATHAME,
    A_ONE_HIT_KO,
    A_LEVELED_UP_FROM_SINK,
    A_ABOUT_TO_DIE,
    A_GENOCIDED_20_SPECIES,
    A_EXTINCTED_NEWTS,
    A_WALKED_ON_LAVA,           /* #256 - end tnntachieve3 */
    A_GOT_LICH_ATHAME,
    A_DID_PURE_SOKOBAN,
    A_STETHOSCOPED_SDOOR,
    A_PLANES_VIA_QUAFFING,
    A_WATER_GUSH_FORTH,
    A_PURPLE_RAIN,
    A_ATE_3_ARTIFACTS,
    A_BOOBY_TRAPPED_TIN,
    A_VISION_FROM_THRONE,
    A_DECAPITATED_ENEMY,
    A_EXPLODED_LARGE_BOX,
    A_LIT_20_ROOMS,
    A_MFORCE_15_LEVELS,
    A_PET_THROUGH_PLANES,
    A_GOT_RID_OF_WATCH,
    A_NESTED_BOH,
    A_MAKE_MON_ESCAPE,
    A_MAKE_DLORD_ESCAPE,
    A_GOT_30_AC,
    A_DOOR_RESIST_8X,
    A_BRIBED_MERC,
    A_LONG_TIMEOUT,
    A_KILLED_FLAYER_IN_MINES,
    A_M_KILLED_BY_OWN_WAND,
    A_DOUBLE_BONES,
    A_FOUND_FOUR_BONES,
    A_5_MAXPW_FROM_NEWTS,
    A_MULTISHOT_4_KILLED,
    A_MON_GREW_TO_HIPRIEST,
    /* 35 free bits in tnntachieve4 */
    /* 285 achievements defined */
    /* NOTE: There is another achievement that is the combination of all
     * A_PARTIAL_* achievements. That is NOT defined here, because we already
     * have the bits for it, but it means that there are actually more
     * achievements in the tournament than are defined here. This was already
     * sort of the case with achievements that use the regular achieve bitfield.
     */
    NUM_TNNT_ACHIEVEMENTS
};

/* TNNT - achievement names and descriptions */
struct tnnt_achvmt_data {
    const char* name;
    const char* descr;
    /* Maybe track achievement earned/not status here with a boolean? */
};
E struct tnnt_achvmt_data tnnt_achievements[NUM_TNNT_ACHIEVEMENTS];

/* vanilla achievements - tracked separately for now; TODO: maybe merge them
 * into tnnt_achievements for 2020 tournament */
#define NUM_VANILLA_ACHIEVEMENTS 11
E struct tnnt_achvmt_data vanilla_achievements[NUM_VANILLA_ACHIEVEMENTS];

/* TNNT - generic globals that don't belong in you.h */
#include <stdint.h> /* uint64_t */
#define NUM_MISSING_CODE_SCROLLS 3
#define MAX_TAMED_FELINES 7
#define TNNT_OHKO_DMG 20
enum dtquest_status {
    DTQUEST_NOTSTARTED = 0,
    DTQUEST_INPROGRESS,
    DTQUEST_COMPLETED
};
struct tnnt_globals_t {
    /* The actual achievement bitfields which get written out to xlogfile and
     * track whether or not you have achieved something.
     * Since there are more than 64 achievements, we need multiple 64 bit ints.
     */
    uint64_t tnnt_achievements[5];

    /* Various achievement counters */
    unsigned char graffiti_found;
    unsigned char felines_tamed; /* redundant; for #tnntstats */
    unsigned char high_altars; /* bitmask using regular AM_* masks */
    unsigned char regular_altars;
    uint64_t foods_eaten; /* sadly there are 33 foods */
    /* More highly unstable code. This relies on the "produce" foods being the
     * 12th through 22nd foods, inclusive, defined in objects.c. Yay!
     * The expectation here is that 0x1 = first food type defined in objects.c,
     * 0x2 = second defined, etc. */
#define FOODMASK_ALL     0x1FFFFFFFF /* all foods, period */
#define FOODMASK_PRODUCE 0x0002FF800 /* all fruits & veggies */
#define FOODMASK_TIN     0x100000000 /* only tin */
#define FOODMASK_GLOBS   0x000000780 /* all types of glob (only need one) */
    unsigned short genderflips;
    unsigned char wish_sources;
#define WISHSRC_WAND         0x01
#define WISHSRC_LAMP         0x02
#define WISHSRC_SMOKY_POTION 0x04
#define WISHSRC_THRONE       0x08
#define WISHSRC_WATER_DEMON  0x10
#define WISHSRC_ALL          0x1F
    boolean blew_up_ludios; /* blew up any of its land mines */
    unsigned short readables;
#define RDBL_SCROLL  0x0001
#define RDBL_BOOK    0x0002
#define RDBL_COIN    0x0004
#define RDBL_SHIRT   0x0008
#define RDBL_MARKER  0x0010
#define RDBL_CANDY   0x0020
#define RDBL_CARD    0x0040
#define RDBL_COOKIE  0x0080
#define RDBL_APRON   0x0100
#define ALL_READABLE 0x01FF
    /* Book of the Dead and Orb of Fate are also readable items, but are not
     * counted towards the "read all readable items" achievement. */
    unsigned short soko_guilts;

    /* Macro for adding a readable and auto-achieving the readables achievement
     * if it has been obtained. */
#define tnnt_read(rdbl)                                              \
    do {                                                             \
        tnnt_globals.readables |= (rdbl);                            \
        if ((tnnt_globals.readables & ALL_READABLE) == ALL_READABLE) \
            tnnt_achieve(A_READ_ALL_READABLE);                       \
    } while(0)

    /* Devteam quest */
    xchar missing_scroll_levels[NUM_MISSING_CODE_SCROLLS];
    xchar devteam_quest_status;

    /* NPC Deathmatch */
    boolean deathmatch_started;
    boolean deathmatch_completed;
    int deathmatch_prize_oid;
    unsigned int deathmatch_m_id;
#define is_deathmatch_opponent(mtmp) (mtmp->m_id == tnnt_globals.deathmatch_m_id)

    /* TODO: probably move this above, or the other achievements below, the
     * devteam/npc side quests */
    unsigned feline_m_ids[MAX_TAMED_FELINES]; /* Cat Lady: ids of tamed cats */
    unsigned short wizards_killed;
    /* Inedible object classes:
     * RANDOM_CLASS, ILLOBJ_CLASS, VENOM_CLASS are obvious.
     * GEM_CLASS and ROCK_CLASS have no edible items in them.
     * POTION_CLASS... you don't *eat* potions...
     * This variable is a bitmask: 0x1 = 1 << 0 = the 0th object class,
     * 0x2 = 1 << 1 = the 1th object class, and so on. */
#define ALL_EDIBLE_OCLASSES 0x19EFC
    uint32_t objclasses_eaten;
    unsigned int astral_worm_gulps;
    unsigned short artis_eaten;
    /* amount of each elemental needed to kill on home plane for achievement */
#define TNNT_ELEMENTAL_KILL_THRESHOLD 4
    /* ordered by plane: earth, air, fire, water */
    unsigned short elementals_killed_on_planes[4];
#define TNNT_LITROOM_GOAL 20
    /* 20 entries of ledger_no and roomno */
    struct {
        unsigned short ledgerno;
        unsigned short roomno;
    } dark_rooms_lit[TNNT_LITROOM_GOAL];
#define TNNT_MFORCE_GOAL 15
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
    unsigned char wizkills_this_action;
    /* mthrowu.c tracks an 'archer' variable, but for some reason only for a
     * monster deliberately shooting at another monster. This does the same for
     * monster shooting at the player, for the "friendly fire" achievement. */
    struct monst *psuedo_archer;
#define TNNT_DOOR_RESIST_GOAL 8
    unsigned char consecutive_door_resists;
    unsigned char door_resist_max;
    xchar door_attempt_x, door_attempt_y, door_attempt_ledger;
    unsigned char bones_piles_found;
    unsigned char maxpw_from_corpses;

    /* tnnt devs: add more as needed */
};
E struct tnnt_globals_t tnnt_globals;

/* This is the main "function" for marking you as having achieved something. */
#define tnnt_achieve(achvmt) \
    (tnnt_globals.tnnt_achievements[(achvmt) / 64] |= 1L << ((achvmt) % 64))
#define tnnt_is_achieved(achvmt) \
    ((tnnt_globals.tnnt_achievements[(achvmt) / 64] & (1L << ((achvmt) % 64))) != 0)

#undef E

#endif /* DECL_H */
