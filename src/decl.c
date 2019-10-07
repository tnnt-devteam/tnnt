/* NetHack 3.6	decl.c	$NHDT-Date: 1547025164 2019/01/09 09:12:44 $  $NHDT-Branch: NetHack-3.6.2-beta01 $:$NHDT-Revision: 1.141 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Michael Allison, 2009. */
/* NetHack may be freely redistributed.  See license for details. */

#include "hack.h"

int NDECL((*afternmv));
int NDECL((*occupation));

/* from xxxmain.c */
const char *hname = 0; /* name of the game (argv[0] of main) */
int hackpid = 0;       /* current process id */
#if defined(UNIX) || defined(VMS)
int locknum = 0; /* max num of simultaneous users */
#endif
#ifdef DEF_PAGER
char *catmore = 0; /* default pager */
#endif
char chosen_windowtype[WINTYPELEN];

NEARDATA int bases[MAXOCLASSES] = DUMMY;

NEARDATA int multi = 0;
const char *multi_reason = NULL;
NEARDATA int nroom = 0;
NEARDATA int nsubroom = 0;
NEARDATA int occtime = 0;

/* maze limits must be even; masking off lowest bit guarantees that */
int x_maze_max = (COLNO - 1) & ~1, y_maze_max = (ROWNO - 1) & ~1;

int otg_temp; /* used by object_to_glyph() [otg] */

NEARDATA int in_doagain = 0;

/*
 *      The following structure will be initialized at startup time with
 *      the level numbers of some "important" things in the game.
 */
struct dgn_topology dungeon_topology = { DUMMY };

struct q_score quest_status = DUMMY;

NEARDATA int warn_obj_cnt = 0;
NEARDATA int smeq[MAXNROFROOMS + 1] = DUMMY;
NEARDATA int doorindex = 0;
NEARDATA char *save_cm = 0;

NEARDATA struct kinfo killer = DUMMY;
NEARDATA long done_money = 0;
const char *nomovemsg = 0;
NEARDATA char plname[PL_NSIZ] = DUMMY; /* player name */
NEARDATA char pl_character[PL_CSIZ] = DUMMY;
NEARDATA char pl_race = '\0';

NEARDATA char pl_fruit[PL_FSIZ] = DUMMY;
NEARDATA struct fruit *ffruit = (struct fruit *) 0;

NEARDATA char tune[6] = DUMMY;
NEARDATA boolean ransacked = 0;

const char *occtxt = DUMMY;
const char quitchars[] = " \r\n\033";
const char vowels[] = "aeiouAEIOU";
const char ynchars[] = "yn";
const char ynqchars[] = "ynq";
const char ynaqchars[] = "ynaq";
const char ynNaqchars[] = "yn#aq";
NEARDATA long yn_number = 0L;

const char disclosure_options[] = "iavgco";

#if defined(MICRO) || defined(WIN32)
char hackdir[PATHLEN]; /* where rumors, help, record are */
#ifdef MICRO
char levels[PATHLEN]; /* where levels are */
#endif
#endif /* MICRO || WIN32 */

#ifdef MFLOPPY
char permbones[PATHLEN]; /* where permanent copy of bones go */
int ramdisk = FALSE;     /* whether to copy bones to levels or not */
int saveprompt = TRUE;
const char *alllevels = "levels.*";
const char *allbones = "bones*.*";
#endif

struct linfo level_info[MAXLINFO];

NEARDATA struct sinfo program_state;

/* x/y/z deltas for the 10 movement directions (8 compass pts, 2 up/down) */
const schar xdir[10] = { -1, -1, 0, 1, 1, 1, 0, -1, 0, 0 };
const schar ydir[10] = { 0, -1, -1, -1, 0, 1, 1, 1, 0, 0 };
const schar zdir[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, -1 };

NEARDATA schar tbx = 0, tby = 0; /* mthrowu: target */

/* for xname handling of multiple shot missile volleys:
   number of shots, index of current one, validity check, shoot vs throw */
NEARDATA struct multishot m_shot = { 0, 0, STRANGE_OBJECT, FALSE };

NEARDATA dungeon dungeons[MAXDUNGEON]; /* ini'ed by init_dungeon() */
NEARDATA s_level *sp_levchn;
NEARDATA stairway upstair = { 0, 0, { 0, 0 }, 0 },
                  dnstair = { 0, 0, { 0, 0 }, 0 };
NEARDATA stairway upladder = { 0, 0, { 0, 0 }, 0 },
                  dnladder = { 0, 0, { 0, 0 }, 0 };
NEARDATA stairway sstairs = { 0, 0, { 0, 0 }, 0 };
NEARDATA dest_area updest = { 0, 0, 0, 0, 0, 0, 0, 0 };
NEARDATA dest_area dndest = { 0, 0, 0, 0, 0, 0, 0, 0 };
NEARDATA coord inv_pos = { 0, 0 };

NEARDATA boolean defer_see_monsters = FALSE;
NEARDATA boolean in_mklev = FALSE;
NEARDATA boolean stoned = FALSE; /* done to monsters hit by 'c' */
NEARDATA boolean unweapon = FALSE;
NEARDATA boolean mrg_to_wielded = FALSE;
/* weapon picked is merged with wielded one */

NEARDATA boolean in_steed_dismounting = FALSE;
NEARDATA boolean has_strong_rngseed = FALSE;

NEARDATA coord bhitpos = DUMMY;
NEARDATA coord doors[DOORMAX] = { DUMMY };

NEARDATA struct mkroom rooms[(MAXNROFROOMS + 1) * 2] = { DUMMY };
NEARDATA struct mkroom *subrooms = &rooms[MAXNROFROOMS + 1];
struct mkroom *upstairs_room, *dnstairs_room, *sstairs_room;

dlevel_t level; /* level map */
struct trap *ftrap = (struct trap *) 0;
NEARDATA struct monst youmonst = DUMMY;
NEARDATA struct context_info context = DUMMY;
NEARDATA struct flag flags = DUMMY;
#ifdef SYSFLAGS
NEARDATA struct sysflag sysflags = DUMMY;
#endif
NEARDATA struct instance_flags iflags = DUMMY;
NEARDATA struct you u = DUMMY;
NEARDATA time_t ubirthday = DUMMY;
NEARDATA struct u_realtime urealtime = DUMMY;

schar lastseentyp[COLNO][ROWNO] = {
    DUMMY
}; /* last seen/touched dungeon typ */

NEARDATA struct obj
    *invent = (struct obj *) 0,
    *uwep = (struct obj *) 0, *uarm = (struct obj *) 0,
    *uswapwep = (struct obj *) 0,
    *uquiver = (struct obj *) 0,       /* quiver */
        *uarmu = (struct obj *) 0,     /* under-wear, so to speak */
            *uskin = (struct obj *) 0, /* dragon armor, if a dragon */
                *uarmc = (struct obj *) 0, *uarmh = (struct obj *) 0,
    *uarms = (struct obj *) 0, *uarmg = (struct obj *) 0,
    *uarmf = (struct obj *) 0, *uamul = (struct obj *) 0,
    *uright = (struct obj *) 0, *uleft = (struct obj *) 0,
    *ublindf = (struct obj *) 0, *uchain = (struct obj *) 0,
    *uball = (struct obj *) 0;
/* some objects need special handling during destruction or placement */
NEARDATA struct obj
    *current_wand = 0,  /* wand currently zapped/applied */
    *thrownobj = 0,     /* object in flight due to throwing */
    *kickedobj = 0;     /* object in flight due to kicking */

#ifdef TEXTCOLOR
/*
 *  This must be the same order as used for buzz() in zap.c.
 */
const int zapcolors[NUM_ZAP] = {
    HI_ZAP,     /* 0 - missile */
    CLR_ORANGE, /* 1 - fire */
    CLR_WHITE,  /* 2 - frost */
    HI_ZAP,     /* 3 - sleep */
    CLR_BLACK,  /* 4 - death */
    CLR_WHITE,  /* 5 - lightning */
    CLR_YELLOW, /* 6 - poison gas */
    CLR_GREEN,  /* 7 - acid */
};
#endif /* text color */

const int shield_static[SHIELD_COUNT] = {
    S_ss1, S_ss2, S_ss3, S_ss2, S_ss1, S_ss2, S_ss4, /* 7 per row */
    S_ss1, S_ss2, S_ss3, S_ss2, S_ss1, S_ss2, S_ss4,
    S_ss1, S_ss2, S_ss3, S_ss2, S_ss1, S_ss2, S_ss4,
};

NEARDATA struct spell spl_book[MAXSPELL + 1] = { DUMMY };

NEARDATA long moves = 1L, monstermoves = 1L;
/* These diverge when player is Fast */
NEARDATA long wailmsg = 0L;

/* objects that are moving to another dungeon level */
NEARDATA struct obj *migrating_objs = (struct obj *) 0;
/* objects not yet paid for */
NEARDATA struct obj *billobjs = (struct obj *) 0;

/* used to zero all elements of a struct obj and a struct monst */
NEARDATA const struct obj zeroobj = DUMMY;
NEARDATA const struct monst zeromonst = DUMMY;
/* used to zero out union any; initializer deliberately omitted */
NEARDATA const anything zeroany;

/* originally from dog.c */
NEARDATA char dogname[PL_PSIZ] = DUMMY;
NEARDATA char catname[PL_PSIZ] = DUMMY;
NEARDATA char horsename[PL_PSIZ] = DUMMY;
char preferred_pet; /* '\0', 'c', 'd', 'n' (none) */
/* monsters that went down/up together with @ */
NEARDATA struct monst *mydogs = (struct monst *) 0;
/* monsters that are moving to another dungeon level */
NEARDATA struct monst *migrating_mons = (struct monst *) 0;

NEARDATA struct mvitals mvitals[NUMMONS];
NEARDATA long domove_attempting = 0L;
NEARDATA long domove_succeeded = 0L;

NEARDATA struct c_color_names c_color_names = {
    "black",  "amber", "golden", "light blue", "red",   "green",
    "silver", "blue",  "purple", "white",      "orange"
};

struct menucoloring *menu_colorings = NULL;

const char *c_obj_colors[] = {
    "black",          /* CLR_BLACK */
    "red",            /* CLR_RED */
    "green",          /* CLR_GREEN */
    "brown",          /* CLR_BROWN */
    "blue",           /* CLR_BLUE */
    "magenta",        /* CLR_MAGENTA */
    "cyan",           /* CLR_CYAN */
    "gray",           /* CLR_GRAY */
    "transparent",    /* no_color */
    "orange",         /* CLR_ORANGE */
    "bright green",   /* CLR_BRIGHT_GREEN */
    "yellow",         /* CLR_YELLOW */
    "bright blue",    /* CLR_BRIGHT_BLUE */
    "bright magenta", /* CLR_BRIGHT_MAGENTA */
    "bright cyan",    /* CLR_BRIGHT_CYAN */
    "white",          /* CLR_WHITE */
};

struct c_common_strings c_common_strings = { "Nothing happens.",
                                             "That's enough tries!",
                                             "That is a silly thing to %s.",
                                             "shudder for a moment.",
                                             "something",
                                             "Something",
                                             "You can move again.",
                                             "Never mind.",
                                             "vision quickly clears.",
                                             { "the", "your" } };

/* NOTE: the order of these words exactly corresponds to the
   order of oc_material values #define'd in objclass.h. */
const char *materialnm[] = { "mysterious", "liquid",  "wax",        "organic",
                             "flesh",      "paper",   "cloth",      "leather",
                             "wooden",     "bone",    "dragonhide", "iron",
                             "metal",      "copper",  "silver",     "gold",
                             "platinum",   "mithril", "plastic",    "glass",
                             "gemstone",   "stone" };

/* Vision */
NEARDATA boolean vision_full_recalc = 0;
NEARDATA char **viz_array = 0; /* used in cansee() and couldsee() macros */

/* Global windowing data, defined here for multi-window-system support */
NEARDATA winid WIN_MESSAGE = WIN_ERR;
NEARDATA winid WIN_STATUS = WIN_ERR;
NEARDATA winid WIN_MAP = WIN_ERR, WIN_INVEN = WIN_ERR;
char toplines[TBUFSZ];
/* Windowing stuff that's really tty oriented, but present for all ports */
struct tc_gbl_data tc_gbl_data = { 0, 0, 0, 0 }; /* AS,AE, LI,CO */

char *fqn_prefix[PREFIX_COUNT] = { (char *) 0, (char *) 0, (char *) 0,
                                   (char *) 0, (char *) 0, (char *) 0,
                                   (char *) 0, (char *) 0, (char *) 0,
                                   (char *) 0 };

#ifdef PREFIXES_IN_USE
char *fqn_prefix_names[PREFIX_COUNT] = {
    "hackdir",  "leveldir", "savedir",    "bonesdir",  "datadir",
    "scoredir", "lockdir",  "sysconfdir", "configdir", "troubledir"
};
#endif

NEARDATA struct savefile_info sfcap = {
#ifdef NHSTDC
    0x00000000UL
#else
    0x00000000L
#endif
#if defined(COMPRESS) || defined(ZLIB_COMP)
        | SFI1_EXTERNALCOMP
#endif
#if defined(ZEROCOMP)
        | SFI1_ZEROCOMP
#endif
#if defined(RLECOMP)
        | SFI1_RLECOMP
#endif
    ,
#ifdef NHSTDC
    0x00000000UL, 0x00000000UL
#else
    0x00000000L, 0x00000000L
#endif
};

NEARDATA struct savefile_info sfrestinfo, sfsaveinfo = {
#ifdef NHSTDC
    0x00000000UL
#else
    0x00000000L
#endif
#if defined(COMPRESS) || defined(ZLIB_COMP)
        | SFI1_EXTERNALCOMP
#endif
#if defined(ZEROCOMP)
        | SFI1_ZEROCOMP
#endif
#if defined(RLECOMP)
        | SFI1_RLECOMP
#endif
    ,
#ifdef NHSTDC
    0x00000000UL, 0x00000000UL
#else
    0x00000000L, 0x00000000L
#endif
};

struct plinemsg_type *plinemsg_types = (struct plinemsg_type *) 0;

#ifdef PANICTRACE
const char *ARGV0;
#endif

/* TNNT - initialization of tnnt_globals */
struct tnnt_globals_t tnnt_globals = DUMMY;

/* TNNT - achievement names and descriptions
 * The order of these MUST be the exact same as that of enum tnnt_achievements
 * found in decl.h! If you reorder these, you must reorder those. */
struct tnnt_achvmt_data tnnt_achievements[NUM_TNNT_ACHIEVEMENTS] = {
  {"Back to Civilisation", "Step onto the Minetown altar"},
  {"Sage Advice", "Consult the Oracle"},
  {"The Archetypal Hero", "Get admitted to the Quest"},
  {"Musical Mastermind", "Learn the passtune"},
  {"Indulgences", "Buy protection from a priest"},
  {"Part-time Statue", "Survive stoning"},
  {"The Blob", "Survive sliming"},
  {"Captain Ahab", "Survive being grabbed by a sea monster"},
  {"Furry Little Problem", "Cure lycanthropy"},
  {"New Lease on Life", "Cure deathly illness"},
  {"The Royal Treatment", "Sit on a throne"},
  {"It'll Turn Up Later", "Drop a ring in a sink"},
  {"Nine for Mortal Men Doomed to Die ... Again", "Kill all nine Nazgûl"},
  {"Furious", "Kill all three erinyes"},
  {"Orthodoxy", "Convert an altar"},
  {"Always Talk to the Wise Old Man", "Chat with Izchak"},
  {"Isaac Newton", "Get fruit by kicking a tree"},
  {"Dungeon Economics", "Buy something at a shop"},
  {"Stop, Thief!", "Be chased by the Keystone Kops"},
  {"Wash it Clean", "Uncurse an item in a fountain"},
  {"Well-Read", "Learn a level 5+ spell"},
  {"Pet Whisperer", "Tame a monster by feeding it"},
  {"Beast Master", "Tame a monster without using food"},
  {"The Chosen One", "Get crowned"},
  {"Go Forth and Smite", "Receive an artifact gift from your deity"},
  {"Some Watery Tart", "Be gifted Excalibur"},
  {"Hand Crafted", "Make a dragon scale mail"},
  {"A Little Light Larceny", "Get robbed by a nymph"},
  {"Pot of Greed", "Have your purse lightened by a leprechaun"},
  {"The Birds and the Bees", "Dally with a foocubus"},
  {"The Most Dangerous Game", "Catch a monster in a trap set by you"},
  {"Minetown Avenger", "Kill every orc in Orctown"},
  {"All Clear", "Kill all hostiles in the Big Room"},
  {"Liberated", "Get yourself unpunished"},
  {"It's Called That For a Reason", "Come to earth by levitating over a sink"},
  {"Diamond in the Rough", "Identify a gem with a touchstone"},
  {"Scribe", "Write a scroll with a magic marker"},
  {"First Edition", "Write a book"},
  {"Dragon Rider", "Ride a dragon"},
  {"Motherhood", "Lay an egg"},
  {"Shimmer", "Fireproof an item"},
  {"They Like Shiny Things", "Throw a gem at a unicorn"},
  {"Reincarnation Cancelled", "Tin a troll"},
  {"Kill it Twice", "Destroy a ghost"},
  {"Only Mostly Dead", "Revive a corpse"},
  {"Buffy", "Destroy an undead with #turn"},
  {"Wasn't a Meal, Wasn't Ready, And You Shouldn't Have Eaten It", "Eat a military ration"},
  {"Have Fun Storming the Castle", "Blow up the Castle drawbridge"},
  {"Insert Quarter to Continue", "Get life-saved"},
  {"What It Was Made For", "Kill a appropriate monster with a Bane weapon"},
  {"Buried Treasure", "Dig up a grave not in a graveyard"},
  {"Holy War", "Kill a priest of Moloch"},
  {"Movers and Shakers", "Cause an earthquake"},
  {"Snake Charmer", "Pacify a snake with a flute"},
  {"Dragonslayer", "Kill a dragon"},
  {"Tin Foil Hat", "Dodge a brainsucking attack"},
  {"Right Back at Ya", "Hit an enemy with a thrown boomerang"},
  {"It Burnsss", "Deal silver damage to a silver-hating monster"},
  {"Arrakis", "Make a crysknife"},
  {"Kop Training Program", "Kill a monster with a rubber hose"},
  {"Obligatory Pointy Hat", "Put on a cornuthaum as a wizard"},
  {"Hobbit Skin", "Wear a mithril coat"},
  {"The Words of the Prophets", "Read 5 random graffiti"},
  {"Are You Still Pudding Farming", "Split a pudding"},
  {"Wearable Snack", "Wear a meat ring"},
  {"Frozen Treats", "Loot an ice box"},
  {"Pygmalion", "Create a monster from a figurine"},
  {"Nerd", "Read a spellbook while wearing lenses and a T-shirt"},
  {"Recharged", "Charge an object"},
  {"Get Your Kicks", "Kick an item embedded in a wall or rock"},
  {"Spiderman", "Grab an item with a grappling hook"},
  {"More Light", "Refill the Candelabrum after it burns out after the Invocation"},
  {"Balanced Diet", "Eat one of each type of produce in one game"},
  {"Smorgasbord", "Eat every type of food in the game"},
  {"Fill 'Er Up", "Fill an oil lamp with a potion of oil"},
  {"Curse the Darkness", "Identify a potion of oil by applying it"},
  {"Bright Light, Bright Light", "Kill gremlin with any light source"},
  {"Chemistry 101", "Alchemize two potions in an established recipe"},
  {"Get Your Gas Mask", "Kill a monster with a stinking cloud"},
  {"Escape to Discworld", "Read a Terry Pratchett novel"},
  {"Near Death Experience", "#chat to Death while carrying a Terry Pratchett novel"},
  {"Mineralogist", "Identify every type of valuable gemstone"},
  {"Prize Inside!", "Get a spellbook by breaking a randomly-generated statue"},
  {"Ding-a-Ling", "Summon a nymph by ringing a cursed bell"},
  {"Best Friends Forever", "Take your starting pet to full adulthood"},
  {"Diver", "Dive into a moat or pool and retrieve an item from the bottom"},
  {"Ceiling Clinging Commute", "Ride a wumpus over water"},
  {"How Much Wood?", "Kill or tame a woodchuck"},
  {"Storm the Gates of Heaven", "Kill an Archon"},
  {"Shifty", "Tame a shapeshifter in any way"},
  {"Cat Lady", "Tame seven “f” in the course of one game"},
  {"Collapse the Wavefunction", "Open the box containing Schroedinger's Cat"},
  {"I'm Free! I'm Free!", "Free a prisoner"},
  {"Pet Rescue", "Free a pet from a trap that you didn't create yourself"},
  {"Spam, Spam, Spam", "Read a scroll of mail"},
  {"Superfood", "Gain strength by eating royal jelly"},
  {"You Are What You Eat", "Gain an intrinsic by eating a corpse (or tin or glob)"},
  {"The Art of the Deal", "Bankrupt a shopkeeper by getting all their money"},
  {"Distinction Without Difference", "Polymorph a pet using a polytrap"},
  {"Transgender", "Put on an amulet of change"},
  {"Father Figure", "Hatch an egg not laid by you and have it come out tame"},
  {"Hello, Nurse!", "Get a nurse to heal you or raise your max HP"},
  {"It's The Pits", "Kill something while you are stuck in a pit"},
  {"Never Saw It Coming", "Kill invisible monster without see invisible"},
  {"Shafted", "Fall down a shaft"},
  {"Potions of Booze Cost More Than That", "Get two bits for an ale from a priest"},
  {"Horatius at the Bridge", "Extinct soldiers or Keystone Kops"},
  {"Who's That Pretty Girl in the Mirror, There?", "Deflect a nymph with a mirror"},
  {"Money Talks", "Bribe a demon lord"},
  {"Breaking and Entering", "Enter a vault"},
  {"Who Was That David Person Anyway?", "Enter a treasure zoo"},
  {"Absolute Monarchy", "Enter a throne room"},
  {"They're After Me Lucky Charms", "Enter a leprechaun hall"},
  {"Hive Mind", "Enter a beehive"},
  {"Day of the Dead", "Enter a graveyard"},
  {"Team a Headquarters", "Enter an anthole"},
  {"Halt, Who Goes There?", "Enter a barracks"},
  {"Dungeon Ecology", "Enter a swamp"},
  {"Statue Showcase", "Enter a cockatrice nest"},
  {"Asmodead", "Kill Asmodeus"},
  {"Fly Swatter", "Kill Baalzebub"},
  {"That Wand is Mine", "Kill Orcus"},
  {"Squish Squish", "Kill Juiblex"},
  {"Demogorgone", "Kill Demogorgon"},
  {"Yee-No-Go", "Kill Yeenoghu"},
  {"Geryon-Slayer", "Kill Geryon"},
  {"Dispater-Dispatcher", "Kill Dispater"},
  {"Conqueror of Death", "Kill Death"},
  {"Curer of Pestilence", "Kill Pestilence"},
  {"Feeder of Famine", "Kill Famine"},
  {"Under New Management", "Change your alignment by any means"},
  {"Heisenberg", "Become uncertain about your position or velocity"},
  {"Who You Gonna Call?", "Get scared by a ghost"},
  {"Groundhog Day", "Return to a forgotten level"},
  {"Tainted", "Dip an item in unholy water"},
  {"Hydrochloric", "Dissolve iron bars with acid"},
  {"Predictably, Nothing Happens", "Break an identified wand of nothing"},
  {"Neutralized", "Cancel a monster"},
  {"Magivore", "Gain a magical intrinsic by eating a piece of jewelry"},
  {"Rope-a-Dope", "Kill a monster with a thrown aklys"},
  {"Panacea", "Neutralize a harmful potion with a unicorn horn"},
  {"Hulk Smash", "Kill a monster with a heavy iron ball"},
  {"Croesus Competitor", "Finish a game carrying at least 50000 gold"},
  {"Croesus Buyout", "Finish a game carrying at least 100000 gold"},
  {"Palantir", "Successfully apply a crystal ball"},
  {"Peak Condition", "Raise all attributes to at least 18"},
  {"Those Who Came Before", "Sight another player's bones pile"},
  {"Imitation Bones", "Step onto the Rogue level fake bones pile"},
  {"Replacement Goldfish", "Cast create familiar"},
  {"Pop the Question", "Give a ring of adornment to a succubus"},
  {"No Soup For You!", "Get expelled from the Quest"},
  {"Discerning Palate", "Gain intrinsic food appraisal"},
  {"Playerbanebane", "Kill a soldier ant"},
  {"Banishing Spells Not Included", "Summon a demon lord"},
  {"Tastes Like Chicken", "Eat chickatrice or cockatrice meat"},
  {"Dust to Dust", "Wrest one last charge from a wand of wishing"},
  {"Teetotaler", "Neutralize a potion of booze with an amethyst"},
  {"Spooky Scary Skeletons", "Get scared by the rattling of a skeleton"},
  {"Ocean's Eleven", "Enter Fort Ludios"},
  {"I Did It For The Money", "Kill Croesus"},
  {"Gullible", "Lie to a vault guard about your name"},
  {"Luck of the Grasshopper", "Reach +13 Luck"},
  {"Murphy's Law", "Reach -13 Luck"},
  {"Looking Slick", "Grease a piece of armor"},
  {"Watch Your Footing", "Step onto the vibrating square"},
  {"The Grand Tour", "Visit each high altar on the Astral Plane"},
  {"Friends Not Food", "Receive the Quest with vegetarian conduct intact"},
  {"Animal Rights Activist", "Receive the Quest with vegan conduct intact"},
  {"Anti Gulp", "Receive the Quest with foodless conduct intact"},
  {"No Help from Above", "Receive the Quest with atheist conduct intact"},
  {"Wax On Wax Off", "Receive the Quest with weaponless conduct intact"},
  {"Thou Shalt Not Kill", "Receive the Quest with pacifist conduct intact"},
  {"Me No Do Words Good", "Receive the Quest with illiterate conduct intact"},
  {"Lost My Transmutation License", "Receive the Quest with polypileless conduct intact"},
  {"True to Form", "Receive the Quest with polyselfless conduct intact"},
  {"Self Sufficient", "Receive the Quest with wishless conduct intact"},
  {"Leave the Artifacts to the Gods", "Receive the Quest with artifact wishless conduct intact"},
  {"No Help From The Floor", "Receive the Quest with Elberethless conduct intact"},
  {"Feeling Your Way", "Receive the Quest with zen (permablind) conduct intact"},
  {"Going Commando", "Receive the Quest with nudist conduct intact"},
  {"Make Love, Not War", "Receive the Quest with genocideless conduct intact"},
  {"It's Dangerous To Go Alone! Take This", "Put an item into a swap chest"},
  {"The True Dungeon Masters", "Chat with an active devteam member"},
  {"There Is No Spoon", "Finish the task the devteam requires of you"},
  {"DIRGE", "Instakill an enemy with poison"},
  {"Gender Bender", "Change gender two or more times"},
  {"The Deathly Hallows", "Wield a wand of death while wearing an invisibility cloak and amulet of life saving"},
  {"Indiana Jones", "Disarm a monster with a bullwhip"},
  {"Pilgrim", "Visit every type of aligned altar"},
  {"Wanton Waste", "Break an identified, charged wand of wishing"},
  {"Poseidon", "Kill a sea monster with a trident"},
  {"It's 4:20 Somewhere", "Stone a monster"},
  {"Orpheus", "Activate a drawbridge with the passtune"},
  {"Michelin Guide", "Identify both sources of food detection"},
  {"All You Ever Wanted", "Get a wish from every wish source"},
  {"Touch It With A 10 Foot Pole", "Kill a floating eye with a polearm"},
  {"Jeweler", "Identify all rings"},
  {"Mikado", "Identify all wands"},
  {"King of Bling", "Identify all amulets"},
  {"Bookworm", "Identify all spellbooks"},
  {"Scholar", "Identify all scrolls"},
  {"Alchemist", "Identify all potions"},
  {"Fashionista", "Identify all non-obvious cloaks, helms, boots, and gloves"},
  {"Minesweeper", "Disarm all land mines in Fort Ludios without blowing any up"},
  {"Hyperliterate", "Read every readable item in the game"},
  {"Civilized Cooking", "Open a tin with a tin opener"},
  {"007", "Unlock something with a credit card"},
  {"Bob Ross", "Release a picture-painting demon from a camera"},
  {"Got No Talent", "Scare something by playing music"},
  {"Cookie Monster", "Read a fortune without eating its cookie"},
  {"Say Hello To My Evil Friend", "Have a non-guaranteed demon lord gated in or summoned"},
  {"Death onna Stick", "Kill a member of 25 different monster species"},
  {"Bring out your Dead", "Kill a member of 50 different monster species"},
  {"Diverse Death-Dealer", "Kill a member of 100 different monster species"},
  {"Grim Reaper", "Kill a memer of 200 different monster species"},
  {"Gotta Kill 'em All", "Kill a member of all 298 eligible monster species"},
  {"Learn the Alphabet", "Kill a member of each species from a to z"},
  {"Make your Mark", "Engrave with an athame"},
  {"One Hit KO", "One-shot a monster with at least 10 HP damage"},
  {"Drink It To The Next Level", "Level up by drinking from a sink"},
  {"Why Won't You DIE Already", "Kill the Wizard of Yendor 20 or more times"},
  {"You're About To Die, You Scrub", "Get a special message saying you're about to die"},
  {"Six Pack", "Stock an ice box with 6 or more potions of booze"},
  {"Bar Brawler", "Kill a monster with HP damage by smashing a bottle on it"},
  {"Extreme Omnivore", "Eat an object from every common, edible object class"},
  {"Extinctionist Lite", "Genocide 20 or more monster species"},
  {"Dudley's Hero", "Genocide or extinct newts"},
  {"Ice Jesus", "Walk on top of lava unharmed"},
  {"Rare Loot", "Acquire an athame that was generated in the inventory of a lich"}
};

/* support for lint.h */
unsigned nhUse_dummy = 0;

/* dummy routine used to force linkage */
void
decl_init()
{
    return;
}

/*decl.c*/
