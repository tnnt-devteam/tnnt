/* NetHack 3.6	you.h	$NHDT-Date: 1450231172 2015/12/16 01:59:32 $  $NHDT-Branch: NetHack-3.6.0 $:$NHDT-Revision: 1.30 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2016. */
/* NetHack may be freely redistributed.  See license for details. */

#ifndef YOU_H
#define YOU_H

#include "attrib.h"
#include "monst.h"
#ifndef PROP_H
#include "prop.h" /* (needed here for util/makedefs.c) */
#endif
#include "skills.h"
#include <stdint.h>

/*** Substructures ***/

struct RoleName {
    const char *m; /* name when character is male */
    const char *f; /* when female; null if same as male */
};

struct RoleAdvance {
    /* "fix" is the fixed amount, "rnd" is the random amount */
    xchar infix, inrnd; /* at character initialization */
    xchar lofix, lornd; /* gained per level <  urole.xlev */
    xchar hifix, hirnd; /* gained per level >= urole.xlev */
};

struct u_have {
    Bitfield(amulet, 1);   /* carrying Amulet   */
    Bitfield(bell, 1);     /* carrying Bell     */
    Bitfield(book, 1);     /* carrying Book     */
    Bitfield(menorah, 1);  /* carrying Candelabrum */
    Bitfield(questart, 1); /* carrying the Quest Artifact */
    Bitfield(unused, 3);
};

struct u_event {
    Bitfield(minor_oracle, 1); /* received at least 1 cheap oracle */
    Bitfield(major_oracle, 1); /*  "  expensive oracle */
    Bitfield(read_tribute, 1); /* read a passage from a novel */
    Bitfield(qcalled, 1);      /* called by Quest leader to do task */
    Bitfield(qexpelled, 1);    /* expelled from the Quest dungeon */
    Bitfield(qcompleted, 1);   /* successfully completed Quest task */
    Bitfield(uheard_tune, 2);  /* 1=know about, 2=heard passtune */

    Bitfield(uopened_dbridge, 1);   /* opened the drawbridge */
    Bitfield(invoked, 1);           /* invoked Gate to the Sanctum level */
    Bitfield(gehennom_entered, 1);  /* entered Gehennom via Valley */
    Bitfield(uhand_of_elbereth, 2); /* became Hand of Elbereth */
    Bitfield(udemigod, 1);          /* killed the wiz */
    Bitfield(uvibrated, 1);         /* stepped on "vibrating square" */
    Bitfield(ascended, 1);          /* has offered the Amulet */
};

/* TNNT specific achievements.
 * Does not contain achievements which will be in TNNT but are already
 * represented in the xlog somewhere, such as killing Medusa or escaping in
 * celestial disgrace.
 * Additionally, achievements for killing certain types of monsters are handled
 * elsewhere.  TODO: where elsewhere?
 * TODO 3: I've also marked the achievements which are relatively easily
 * scummable, since discussion didn't resolve on whether these should remain in
 * or not. */
enum tnnt_achievements {
    A_FOUND_MINES_ALTAR = 0, /* Implemented,tested. */
    A_CONSULTED_ORACLE,      /* Implemented,tested. */
    A_STARTED_QUEST,         /* Implemented,tested. */
    A_LEARNED_PASSTUNE,      /* Implemented,tested. */
    A_BOUGHT_PROTECTION,     /* Implemented,tested. */
    A_CURED_STONING,         /* Implemented,tested. */
    A_CURED_SLIMING,         /* Implemented,tested. */
    A_SURVIVED_DROWNING,     /* Implemented. */
    A_CURED_LYCANTHROPY,     /* Implemented,tested. */
    A_CURED_ILLNESS,         /* Implemented,tested. */
    A_USED_THRONE,           /* Implemented,tested. */
    A_DROPPED_RING_IN_SINK,  /* Implemented,tested. */
    A_KILLED_9_NAZGUL,       /* Implemented. */
    A_KILLED_3_ERINYES,      /* Implemented. */
    A_CONVERTED_ALTAR,       /* Implemented,tested. */
    A_CHATTED_IZCHAK,        /* Implemented,tested. */
    A_KICKED_FRUIT_TREE,     /* Implemented,tested. */
    A_PURCHASED,             /* Implemented,tested. */
    A_ROBBED,                /* Implemented,tested. */
    A_UNCURSED_IN_FOUNTAIN,  /* Implemented,tested. */
    A_LEARNED_LV5_SPELL,     /* Implemented. */
    A_TAMED_BY_FOOD,         /* Implemented. */
    A_TAMED_NOT_BY_FOOD,     /* Implemented. */
    A_CROWNED,               /* Implemented. */
    A_GRANTED_ARTI,          /* Implemented. */
    A_GOT_EXCALIBUR,         /* Implemented. */
    A_CREATED_DSM,           /* Implemented. Scummable... */
    A_ROBBED_BY_NYMPH,       /* Implemented. Scummable... */
    A_ROBBED_BY_LEPRECHAUN,  /* Implemented. */
    A_ENCOUNTERED_FOOCUBUS,  /* Implemented. Scummable... */
    A_TRAPPED_MONSTER,       /* Implemented. Scummable... */
    A_AVENGED_ORCTOWN,       /* Implemented, tested. */
    A_CLEARED_BIGROOM,       /* Implemented, tested. */
    A_GOT_UNPUNISHED,        /* Implemented. */
    A_FELL_ONTO_SINK,        /* Implemented. */
    A_USED_TOUCHSTONE,       /* Implemented. Scummable... */
    A_WROTE_SCROLL,          /* Implemented. Scummable... */
    A_WROTE_SPELLBOOK,       /* Implemented.  */
    A_RODE_DRAGON,           /* Implemented. */
    A_LAID_EGG,              /* Implemented. Scummable... */
    A_ERODEPROOFED_ITEM,     /* Implemented. Scummable... */
    A_THREW_UNICORN_GEM,     /* Implemented. */
    A_BLOCKED_TROLL_REVIVAL, /* Implemented. Scummable... */
    A_KILLED_GHOST,          /* Implemented. */
    A_REVIVIFIED_CORPSE,     /* Implemented. */
    A_DESTROYED_VIA_TURN,    /* Implemented. */
    A_ATE_MILITARY_RATION,   /* Implemented. */
    A_BLEW_UP_DRAWBRIDGE,    /* Implemented. */
    A_GOT_LIFESAVED,         /* Implemented. */
    A_USED_CORRECT_BANE,     /* Implemented. */
    A_DUG_UP_GRAVE,          /* Implemented. */
    A_KILLED_MOLOCH_PRIEST,  /* Implemented. */
    A_CAUSED_EARTHQUAKE,     /* Implemented. */
    A_CHARMED_SNAKE,         /* Implemented. */
    A_KILLED_DRAGON,         /* Implemented. */
    A_EVADED_BRAINSUCK,      /* Implemented. */
    A_HIT_WITH_BOOMERANG,    /* Implemented. */
    A_DEALT_SILVER_DAMAGE,   /* Implemented. */
    A_MADE_CRYSKNIFE,        /* Implemented. */
    A_KILLED_WITH_HOSE,      /* Implemented. */
    A_WIZ_WORE_CORNUTHAUM,   /* Implemented. */
    A_WORE_MITHRIL_COAT,     /* Implemented. */
    A_FOUND_5_GRAFFITI,      /* Implemented. */
    A_SPLIT_A_PUDDING,       /* Implemented. */
    A_WORE_MEAT_RING,        /* Implemented. */
    A_LOOTED_ICE_BOX,        /* Implemented. */
    A_ANIMATED_FIGURINE,     /* Implemented. */
    A_NERD,                  /* Implemented. */
    A_CHARGED_AN_OBJECT,     /* Implemented. */
    A_KICKED_EMBEDDED_ITEM,  /* Implemented. */
    A_SNAGGED_WITH_HOOK,     /* Implemented. */
    A_REFILLED_CANDELABRUM,  /* Implemented. */
    A_ATE_ALL_PRODUCE,       /* Implemented. */
    A_ATE_ALL_FOODS,         /* Implemented. */
    A_REFILLED_OIL_LAMP,     /* Implemented. */
    A_LIT_OIL_POTION,        /* Implemented. */
    A_LIT_UP_GREMLIN,        /* Implemented. */
    A_ALCHEMIZED,            /* Implemented. */
    A_GASSED_MONSTER,        /* Implemented. */
    A_READ_NOVEL,            /* Implemented. */
    A_TALKED_TO_DEATH,       /* Implemented. */
    A_IDENTIFIED_ALL_GEMS,   /* Implemented. */
    A_GOT_SPBOOK_FROM_STATUE,/* Implemented. */
    A_SUMMONED_NYMPH,        /* Implemented. */
    A_FULLY_RAISED_PET,      /* Implemented. */
    A_GOT_OBJECT_UNDERWATER, /* Implemented. */
    A_RODE_WUMPUS_OVER_WATER,/* Implemented. */
    A_KILLED_TAMED_WOODCHUCK,/* Implemented. */
    A_KILLED_ARCHON,         /* Implemented. */
    A_TAMED_SHAPECHANGER,    /* Implemented. */
    A_TAMED_7_CATS,          /* Implemented. */
    A_OPENED_SCHROEDINGERS,  /* Implemented. */
    A_FREED_PRISONER,        /* Implemented. */
    A_UNTRAPPED_PET,         /* Implemented. */
    A_READ_MAIL,             /* Implemented. */
    A_ATE_ROYAL_JELLY,       /* Implemented. */
    A_GOT_CORPSE_INTRINSIC,  /* Implemented. */
    A_BANKRUPTED_SHOPKEEPER, /* Implemented. */
    A_POLYED_PET_ON_TRAP,    /* Implemented. */
    A_CHANGED_GENDER,        /* Implemented. */
    A_HATCHED_FOREIGN_EGG,   /* Implemented. */
    A_HEALED_BY_NURSE,       /* Implemented. */
    A_KILLED_WHILE_IN_PIT,   /* Implemented. */
    A_KILLED_INVISIBLE,      /* Implemented. */
    A_FELL_DOWN_SHAFT,       /* Implemented. */
    A_GOT_TWO_BITS,          /* Implemented. */
    A_EXTINCTED_ARMY_OR_KOPS,/* Implemented. */
    A_DEFLECTED_NYMPH,       /* Implemented. */
    A_BRIBED_DEMON_LORD,     /* Implemented. */
    A_ENTERED_VAULT,         /* Implemented, tested. */
    A_ENTERED_ZOO,           /* Implemented. */
    A_ENTERED_THRONE_ROOM,   /* Implemented. */
    A_ENTERED_LEP_HALL,      /* Implemented. */
    A_ENTERED_BEEHIVE,       /* Implemented. */
    A_ENTERED_GRAVEYARD,     /* Implemented. */
    A_ENTERED_ANTHOLE,       /* Implemented. */
    A_ENTERED_BARRACKS,      /* Implemented. */
    A_ENTERED_SWAMP,         /* Implemented. */
    A_ENTERED_COCKNEST,      /* Implemented. */
    A_KILLED_ASMODEUS,       /* Implemented. */
    A_KILLED_BAALZEBUB,      /* Implemented. */
    A_KILLED_ORCUS,          /* Implemented. */
    A_KILLED_JUIBLEX,        /* Implemented. */
    A_KILLED_DEMOGORGON,     /* Implemented. */
    A_KILLED_YEENOGHU,       /* Implemented. */
    A_KILLED_GERYON,         /* Implemented. */
    A_KILLED_DISPATER,       /* Implemented. */
    A_KILLED_DEATH,          /* Implemented. */
    A_KILLED_PESTILENCE,     /* Implemented. */
    A_KILLED_FAMINE,         /* Implemented. */
    A_ESCAPED_IN_DISGRACE,   /* Implemented. */
    A_CHANGED_ALIGNMENT,     /* Implemented. */
    A_HEISENBERG,            /* Implemented. */
    A_SCARED_BY_GHOST,       /* Implemented. */
    A_REVISITED_LOST_LEVEL,  /* Implemented. */
    A_DIPPED_IN_UNHOLY,      /* Implemented. */
    A_DISSOLVED_IRONBARS,    /* Implemented. */
    A_BROKE_WAN_NOTHING,     /* Implemented. */
    A_CANCELED_MONSTER,      /* Implemented. */
    A_INTRINSIC_FROM_JEWELRY,/* Implemented. */
    A_KILLED_WITH_AKLYS,     /* Implemented. */
    A_NEUTRALIZED_POTION,    /* Implemented. */
    A_KILLED_WITH_IRONBALL,  /* Implemented. */
    A_FINISHED_WITH_50000,   /* Implemented. */
    A_FINISHED_WITH_100000,  /* Implemented. */
    A_APPLIED_CRYSTAL_BALL,  /* Implemented. */
    A_MAXED_ATTRIBUTES,      /* Implemented. */
    A_FOUND_BONES_PILE,      /* Implemented. */
    A_FOUND_ROGUE_BONES_PILE,/* Implemented. */
    A_CREATED_FAMILIAR,      /* Implemented. */
    A_GAVE_ADORNMENT,        /* Implemented. */
    A_UNWINNABLE,
    A_GOT_FOOD_APPRAISAL,    /* Implemented. */
    A_KILLED_SOLDIER_ANT,    /* Implemented. */
    A_SUMMONED_DEMON_LORD,   /* Implemented. */
    A_TASTED_COCKATRICE,     /* Implemented. */
    A_WRESTED_WOW            /* Implemented. */
};

struct u_achieve {
    Bitfield(amulet, 1);  /* touched Amulet */
    Bitfield(bell, 1);    /* touched Bell */
    Bitfield(book, 1);    /* touched Book */
    Bitfield(menorah, 1); /* touched Candelabrum */
    Bitfield(enter_gehennom,
             1);           /* entered Gehennom (or Valley) by any means */
    Bitfield(ascended, 1); /* not quite the same as u.uevent.ascended */
    Bitfield(mines_luckstone, 1); /* got a luckstone at end of mines */
    Bitfield(finish_sokoban, 1);  /* obtained the sokoban prize */

    Bitfield(killed_medusa, 1);
    Bitfield(graffiti_found, 4); /* TNNT achievement */
    Bitfield(felines_tamed, 3); /* TNNT achievement */
    uint64_t foods_eaten; /* TNNT achievement; sadly there are 33 foods */

    /* 64 bits for the TNNT achievements above. Since there are more than 64
     * achievements, we need multiple 64 bit ints. */
    uint64_t tnnt_achievements[2];
};

#define tnnt_achieve(achvmt) \
    u.uachieve.tnnt_achievements[achvmt / 64] |= 1L << (achvmt % 64)

struct u_realtime {
    long   realtime;     /* accumulated playing time in seconds */
    time_t start_timing; /* time game was started or restored or 'realtime'
                            was last updated (savegamestate for checkpoint) */
    time_t finish_time;  /* end of 'realtime' interval: time of save or
                            end of game; used for topten/logfile/xlogfile */
};

/* KMH, conduct --
 * These are voluntary challenges.  Each field denotes the number of
 * times a challenge has been violated.
 */
struct u_conduct {     /* number of times... */
    long unvegetarian; /* eaten any animal */
    long unvegan;      /* ... or any animal byproduct */
    long food;         /* ... or any comestible */
    long gnostic;      /* used prayer, priest, or altar */
    long weaphit;      /* hit a monster with a weapon */
    long killer;       /* killed a monster yourself */
    long literate;     /* read something (other than BotD) */
    long polypiles;    /* polymorphed an object */
    long polyselfs;    /* transformed yourself */
    long wishes;       /* used a wish */
    long wisharti;     /* wished for an artifact */
    long elbereth;     /* engraved Elbereth */
    /* genocides already listed at end of game */
};

struct u_roleplay {
    boolean blind;  /* permanently blind */
    boolean nudist; /* has not worn any armor, ever */
    long numbones;  /* # of bones files loaded  */
};

/*** Unified structure containing role information ***/
struct Role {
    /*** Strings that name various things ***/
    struct RoleName name;    /* the role's name (from u_init.c) */
    struct RoleName rank[9]; /* names for experience levels (from botl.c) */
    const char *lgod, *ngod, *cgod; /* god names (from pray.c) */
    const char *filecode;           /* abbreviation for use in file names */
    const char *homebase; /* quest leader's location (from questpgr.c) */
    const char *intermed; /* quest intermediate goal (from questpgr.c) */

    /*** Indices of important monsters and objects ***/
    short malenum, /* index (PM_) as a male (botl.c) */
        femalenum, /* ...or as a female (NON_PM == same) */
        petnum,    /* PM_ of preferred pet (NON_PM == random) */
        ldrnum,    /* PM_ of quest leader (questpgr.c) */
        guardnum,  /* PM_ of quest guardians (questpgr.c) */
        neminum,   /* PM_ of quest nemesis (questpgr.c) */
        enemy1num, /* specific quest enemies (NON_PM == random) */
        enemy2num;
    char enemy1sym, /* quest enemies by class (S_) */
        enemy2sym;
    short questarti; /* index (ART_) of quest artifact (questpgr.c) */

    /*** Bitmasks ***/
    short allow;                  /* bit mask of allowed variations */
#define ROLE_RACEMASK  0x0ff8     /* allowable races */
#define ROLE_GENDMASK  0xf000     /* allowable genders */
#define ROLE_MALE      0x1000
#define ROLE_FEMALE    0x2000
#define ROLE_NEUTER    0x4000
#define ROLE_ALIGNMASK AM_MASK    /* allowable alignments */
#define ROLE_LAWFUL    AM_LAWFUL
#define ROLE_NEUTRAL   AM_NEUTRAL
#define ROLE_CHAOTIC   AM_CHAOTIC

    /*** Attributes (from attrib.c and exper.c) ***/
    xchar attrbase[A_MAX];    /* lowest initial attributes */
    xchar attrdist[A_MAX];    /* distribution of initial attributes */
    struct RoleAdvance hpadv; /* hit point advancement */
    struct RoleAdvance enadv; /* energy advancement */
    xchar xlev;               /* cutoff experience level */
    xchar initrecord;         /* initial alignment record */

    /*** Spell statistics (from spell.c) ***/
    int spelbase; /* base spellcasting penalty */
    int spelheal; /* penalty (-bonus) for healing spells */
    int spelshld; /* penalty for wearing any shield */
    int spelarmr; /* penalty for wearing metal armour */
    int spelstat; /* which stat (A_) is used */
    int spelspec; /* spell (SPE_) the class excels at */
    int spelsbon; /* penalty (-bonus) for that spell */

    /*** Properties in variable-length arrays ***/
    /* intrinsics (see attrib.c) */
    /* initial inventory (see u_init.c) */
    /* skills (see u_init.c) */

    /*** Don't forget to add... ***/
    /* quest leader, guardians, nemesis (monst.c) */
    /* quest artifact (artilist.h) */
    /* quest dungeon definition (dat/Xyz.dat) */
    /* quest text (dat/quest.txt) */
    /* dictionary entries (dat/data.bas) */
};

extern const struct Role roles[]; /* table of available roles */
extern struct Role urole;
#define Role_if(X) (urole.malenum == (X))
#define Role_switch (urole.malenum)

/* used during initialization for race, gender, and alignment
   as well as for character class */
#define ROLE_NONE (-1)
#define ROLE_RANDOM (-2)

/*** Unified structure specifying race information ***/

struct Race {
    /*** Strings that name various things ***/
    const char *noun;           /* noun ("human", "elf") */
    const char *adj;            /* adjective ("human", "elven") */
    const char *coll;           /* collective ("humanity", "elvenkind") */
    const char *filecode;       /* code for filenames */
    struct RoleName individual; /* individual as a noun ("man", "elf") */

    /*** Indices of important monsters and objects ***/
    short malenum, /* PM_ as a male monster */
        femalenum, /* ...or as a female (NON_PM == same) */
        mummynum,  /* PM_ as a mummy */
        zombienum; /* PM_ as a zombie */

    /*** Bitmasks ***/
    short allow;    /* bit mask of allowed variations */
    short selfmask, /* your own race's bit mask */
        lovemask,   /* bit mask of always peaceful */
        hatemask;   /* bit mask of always hostile */

    /*** Attributes ***/
    xchar attrmin[A_MAX];     /* minimum allowable attribute */
    xchar attrmax[A_MAX];     /* maximum allowable attribute */
    struct RoleAdvance hpadv; /* hit point advancement */
    struct RoleAdvance enadv; /* energy advancement */
#if 0 /* DEFERRED */
    int   nv_range;           /* night vision range */
    int   xray_range;         /* X-ray vision range */
#endif

    /*** Properties in variable-length arrays ***/
    /* intrinsics (see attrib.c) */

    /*** Don't forget to add... ***/
    /* quest leader, guardians, nemesis (monst.c) */
    /* quest dungeon definition (dat/Xyz.dat) */
    /* quest text (dat/quest.txt) */
    /* dictionary entries (dat/data.bas) */
};

extern const struct Race races[]; /* Table of available races */
extern struct Race urace;
#define Race_if(X) (urace.malenum == (X))
#define Race_switch (urace.malenum)

/*** Unified structure specifying gender information ***/
struct Gender {
    const char *adj;      /* male/female/neuter */
    const char *he;       /* he/she/it */
    const char *him;      /* him/her/it */
    const char *his;      /* his/her/its */
    const char *filecode; /* file code */
    short allow;          /* equivalent ROLE_ mask */
};
#define ROLE_GENDERS 2    /* number of permitted player genders
                             increment to 3 if you allow neuter roles */

extern const struct Gender genders[]; /* table of available genders */
#define uhe()      (genders[flags.female ? 1 : 0].he)
#define uhim()     (genders[flags.female ? 1 : 0].him)
#define uhis()     (genders[flags.female ? 1 : 0].his)
#define mhe(mtmp)  (genders[pronoun_gender(mtmp)].he)
#define mhim(mtmp) (genders[pronoun_gender(mtmp)].him)
#define mhis(mtmp) (genders[pronoun_gender(mtmp)].his)

/*** Unified structure specifying alignment information ***/
struct Align {
    const char *noun;     /* law/balance/chaos */
    const char *adj;      /* lawful/neutral/chaotic */
    const char *filecode; /* file code */
    short allow;          /* equivalent ROLE_ mask */
    aligntyp value;       /* equivalent A_ value */
};
#define ROLE_ALIGNS 3     /* number of permitted player alignments */

extern const struct Align aligns[]; /* table of available alignments */

enum utraptypes {
    TT_BEARTRAP = 0,
    TT_PIT,
    TT_WEB,
    TT_LAVA,
    TT_INFLOOR,
    TT_BURIEDBALL
};

/*** Information about the player ***/
struct you {
    xchar ux, uy;       /* current map coordinates */
    schar dx, dy, dz;   /* direction of move (or zap or ... ) */
    schar di;           /* direction of FF */
    xchar tx, ty;       /* destination of travel */
    xchar ux0, uy0;     /* initial position FF */
    d_level uz, uz0;    /* your level on this and the previous turn */
    d_level utolev;     /* level monster teleported you to, or uz */
    uchar utotype;      /* bitmask of goto_level() flags for utolev */
    boolean umoved;     /* changed map location (post-move) */
    int last_str_turn;  /* 0: none, 1: half turn, 2: full turn
                           +: turn right, -: turn left */
    int ulevel;         /* 1 to MAXULEV */
    int ulevelmax;
    unsigned utrap;     /* trap timeout */
    unsigned utraptype; /* defined if utrap nonzero. one of utraptypes */
    char urooms[5];         /* rooms (roomno + 3) occupied now */
    char urooms0[5];        /* ditto, for previous position */
    char uentered[5];       /* rooms (roomno + 3) entered this turn */
    char ushops[5];         /* shop rooms (roomno + 3) occupied now */
    char ushops0[5];        /* ditto, for previous position */
    char ushops_entered[5]; /* ditto, shops entered this turn */
    char ushops_left[5];    /* ditto, shops exited this turn */

    int uhunger;  /* refd only in eat.c and shk.c */
    unsigned uhs; /* hunger state - see eat.c */

    struct prop uprops[LAST_PROP + 1];

    unsigned umconf;
    Bitfield(usick_type, 2);
#define SICK_VOMITABLE 0x01
#define SICK_NONVOMITABLE 0x02
#define SICK_ALL 0x03

    /* These ranges can never be more than MAX_RANGE (vision.h). */
    int nv_range;   /* current night vision range */
    int xray_range; /* current xray vision range */

/*
 * These variables are valid globally only when punished and blind.
 */
#define BC_BALL 0x01  /* bit mask for ball  in 'bc_felt' below */
#define BC_CHAIN 0x02 /* bit mask for chain in 'bc_felt' below */
    int bglyph;       /* glyph under the ball */
    int cglyph;       /* glyph under the chain */
    int bc_order;     /* ball & chain order [see bc_order() in ball.c] */
    int bc_felt;      /* mask for ball/chain being felt */

    int umonster;               /* hero's "real" monster num */
    int umonnum;                /* current monster number */

    int mh, mhmax, mtimedone;   /* for polymorph-self */
    struct attribs macurr,      /* for monster attribs */
                   mamax;       /* for monster attribs */
    int ulycn;                  /* lycanthrope type */

    unsigned ucreamed;
    unsigned uswldtim;          /* time you have been swallowed */

    Bitfield(uswallow, 1);      /* true if swallowed */
    Bitfield(uinwater, 1);      /* if you're currently in water (only
                                   underwater possible currently) */
    Bitfield(uundetected, 1);   /* if you're a hiding monster/piercer */
    Bitfield(mfemale, 1);       /* saved human value of flags.female */
    Bitfield(uinvulnerable, 1); /* you're invulnerable (praying) */
    Bitfield(uburied, 1);       /* you're buried */
    Bitfield(uedibility, 1);    /* blessed food detect; sense unsafe food */
    /* 1 free bit! */

    unsigned udg_cnt;           /* how long you have been demigod */
    struct u_achieve uachieve;  /* achievements */
    struct u_event uevent;      /* certain events have happened */
    struct u_have uhave;        /* you're carrying special objects */
    struct u_conduct uconduct;  /* KMH, conduct */
    struct u_roleplay uroleplay;
    struct attribs acurr,       /* your current attributes (eg. str)*/
                    aexe,       /* for gain/loss via "exercise" */
                    abon,       /* your bonus attributes (eg. str) */
                    amax,       /* your max attributes (eg. str) */
                   atemp,       /* used for temporary loss/gain */
                   atime;       /* used for loss/gain countdown */
    align ualign;               /* character alignment */
#define CONVERT    2
#define A_ORIGINAL 1
#define A_CURRENT  0
    aligntyp ualignbase[CONVERT]; /* for ualign conversion record */
    schar uluck, moreluck;        /* luck and luck bonus */
#define Luck (u.uluck + u.moreluck)
#define LUCKADD    3  /* value of u.moreluck when carrying luck stone;
                         + when blessed or uncursed, - when cursed */
#define LUCKMAX   10  /* maximum value of u.ulUck */
#define LUCKMIN (-10) /* minimum value of u.uluck */
    schar uhitinc;
    schar udaminc;
    schar uac;
    uchar uspellprot;        /* protection by SPE_PROTECTION */
    uchar usptime;           /* #moves until uspellprot-- */
    uchar uspmtime;          /* #moves between uspellprot-- */
    int uhp, uhpmax;         /* hit points, aka health */
    int uen, uenmax;         /* magical energy - M. Stephenson */
    xchar uhpinc[MAXULEV],   /* increases to uhpmax for each level gain */
          ueninc[MAXULEV];   /* increases to uenmax for each level gain */
    int ugangr;              /* if the gods are angry at you */
    int ugifts;              /* number of artifacts bestowed */
    int ublessed, ublesscnt; /* blessing/duration from #pray */
    long umoney0;
    long uspare1;
    long uexp, urexp;
    long ucleansed;          /* to record moves when player was cleansed */
    long usleep;             /* sleeping; monstermove you last started */
    int uinvault;
    struct monst *ustuck;    /* engulfer or grabber, maybe grabbee if Upolyd */
    struct monst *usteed;    /* mount when riding */
    long ugallop;            /* turns steed will run after being kicked */
    int urideturns;          /* time spent riding, for skill advancement */
    int umortality;          /* how many times you died */
    int ugrave_arise;    /* you die and become something aside from a ghost */
    int weapon_slots;        /* unused skill slots */
    int skills_advanced;     /* # of advances made so far */
    xchar skill_record[P_SKILL_LIMIT]; /* skill advancements */
    struct skills weapon_skills[P_NUM_SKILLS];
    boolean twoweap;         /* KMH -- Using two-weapon combat */
    short uswapitems;        /* TNNT - nitems placed in swap chest; -1 means item was taken out */
#define SWAP_ITEMS_MIN 1     /* you must put this many in, to take one out */
#define SWAP_ITEMS_MAX 3     /* you can put this many in, but only take one out */

}; /* end of `struct you' */

#define Upolyd (u.umonnum != u.umonster)

#endif /* YOU_H */
