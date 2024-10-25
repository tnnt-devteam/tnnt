#if defined(TNNT_ACH_ENUM)
/* there is a slight possibility of name collision here since attributes and
 * alignments both use A_*** as well. We use A_ to keep the constant names
 * short and mitigate having to do ugly line wrappings in heavily indented
 * blocks. */
#define ACH(nam, desc, id) A_##id
#elif defined (TNNT_ACH_NAMES)
#define ACH(nam, desc, id) {nam, desc}
#elif !defined(ACH)
#error Bad inclusion of tnnt_achievements.h
#endif

/* an idea that has been tossed around for a while is to list (most of) the
 * vanilla achievements in here, basically having a separate TNNT achievement
 * for each vanilla one, so that they use the same system and we don't have to
 * futz around with a separate list of vanilla achievements both here in the
 * game and on the scoreboard side.
 * For now, we haven't done that. Vanilla achievements are recorded at the point
 * of accomplishment by tnnt_achieve(NO_TNNT_ACHVMT) for updating temporary
 * achievements and notifying the player.
 */

/* TNNT TODO FOR 3.7: Vanilla achievements replace several of these or
 * approximate them, so we should probably drop the TNNT ones and replace them
 * with vanilla ones.
 * Specifically these: (we can keep the achievement titles)
 * vanilla "reached Minetown" supplants Back to Civilisation
 * vanilla "entered a shop" might supplant "Dungeon Economics"
 * vanilla "consulted the Oracle" supplants "Sage Advice"
 * vanilla "read a Discworld novel" supplants "Escape to Discworld"
 * Some of the other vanilla achievements should be tracked as new vanilla
 * achievements, but possibly not all (depends on what we want to do with the
 * rank title achievements).
 */

/* starting around 2021 achievements are organized into sort of groups based
 * around common themes */
/* "vanilla-ish milestones" group */
ACH("Back to Civilisation", "Step onto the Minetown altar",
    FOUND_MINES_ALTAR),
ACH("Sage Advice", "Consult the Oracle", CONSULTED_ORACLE),
ACH("The Archetypal Hero", "Get admitted to the Quest", STARTED_QUEST),
ACH("Heavenly Music", "Learn the passtune from your god",
    LEARNED_PASSTUNE_FROM_GOD),
ACH("Musical Mastermind", "Open the Castle drawbridge by deducing the passtune",
    DEDUCED_PASSTUNE),
ACH("Have Fun Storming the Castle", "Blow up the Castle drawbridge",
    BLEW_UP_DRAWBRIDGE),
ACH("The Chosen One", "Get crowned", CROWNED),
ACH("Watch Your Footing", "Step onto the vibrating square",
    REACHED_VIBRATING_SQ),
/* "TNNT custom content" group */
ACH("It's Dangerous To Go Alone! Take This", "Put an item into a swap chest",
    PUT_INTO_SWAPCHEST),
ACH("Paw Patrol", "Find kitten", FOUND_KITTEN),
ACH("The True Dungeon Masters", "Chat with an active devteam member",
    TALKED_TO_DEVTEAM),
ACH("There Is No Spoon", "Finish the task the devteam requires of you",
    FINISHED_DEVTEAM_QUEST),
ACH("Thunderdome", "Defeat the NPC from the NPC Deathmatch quest",
    NPC_DEATHMATCH),
/* "kill specific things" group */
ACH("Asmodead", "Kill Asmodeus", KILLED_ASMODEUS),
ACH("Baalzebust", "Kill Baalzebub", KILLED_BAALZEBUB),
ACH("Orcrushed", "Kill Orcus", KILLED_ORCUS),
ACH("Juibl-X", "Kill Juiblex", KILLED_JUIBLEX),
ACH("Demogorgone", "Kill Demogorgon", KILLED_DEMOGORGON),
ACH("Yee-No-Go", "Kill Yeenoghu", KILLED_YEENOGHU),
ACH("Geryon-Be-Gone", "Kill Geryon", KILLED_GERYON),
ACH("Dispater-Dispatcher", "Kill Dispater", KILLED_DISPATER),
ACH("Conqueror of Death", "Kill Death", KILLED_DEATH),
ACH("Curer of Pestilence", "Kill Pestilence", KILLED_PESTILENCE),
ACH("Feeder of Famine", "Kill Famine", KILLED_FAMINE),
ACH("Why Won't You DIE Already", "Kill the Wizard of Yendor 20 or more times",
    KILLED_20_WIZARDS),
ACH("Double Whammy", "Kill the Wizard of Yendor twice in one action",
    WIZ_DOUBLE_KILL),
ACH("Actual Perseus", "Kill Medusa with physical damage",
    KILLED_MEDUSA_PHYSICALLY),
ACH("Actual Actual Perseus", "Decapitate Medusa", DECAPITATED_MEDUSA),
ACH("I Did It For The Money", "Kill Croesus", KILLED_CROESUS),
ACH("Holy War", "Kill a priest of Moloch", KILLED_MOLOCH_PRIEST),
ACH("Nine for Mortal Men Doomed to Die ... Again",
    "Collect all nine Nazgul rings", GOT_9_NAZGUL_RINGS),
ACH("Furious", "Kill all three erinyes", KILLED_3_ERINYES),
ACH("Minetown Avenger", "Kill every orc in Orctown", AVENGED_ORCTOWN),
ACH("All Clear", "Kill all hostiles in the Big Room", CLEARED_BIGROOM),
ACH("Master of the Four Elementals",
    "Kill at least 4 elementals of each type on their respective home planes",
    KILLED_4X4_ELEMENTALS),
ACH("Kill it Twice", "Destroy a ghost", KILLED_GHOST),
ACH("Dragonslayer", "Kill a dragon", KILLED_DRAGON),
ACH("Kill the Rainbow", "Kill every color of adult dragon",
    KILLED_ALL_DRAGONS),
ACH("Kill the Rainbow, Jr.", "Kill every color of baby dragon",
    KILLED_ALL_BABY_DRAGONS),
ACH("Playerbanebane", "Kill a soldier ant", KILLED_SOLDIER_ANT),
ACH("How Much Wood?", "Kill or tame a woodchuck", KILLED_TAMED_WOODCHUCK),
ACH("Storm the Gates of Heaven", "Kill an Archon", KILLED_ARCHON),
ACH("Minesflayer",
    "Kill a mind flayer or master mind flayer in the Gnomish Mines",
    KILLED_FLAYER_IN_MINES),
ACH("Quadruple Kill",
    "Kill 4 monsters with a single volley of missile weapons",
    MULTISHOT_4_KILLED),
ACH("Death onna Stick", "Kill a member of 25 different monster species",
    KILLED_25_SPECIES),
ACH("Bring out your Dead", "Kill a member of 50 different monster species",
    KILLED_50_SPECIES),
ACH("Diverse Death-Dealer", "Kill a member of 100 different monster species",
    KILLED_100_SPECIES),
ACH("Grim Reaper", "Kill a member of 200 different monster species",
    KILLED_200_SPECIES),
ACH("Gotta Kill 'em All", "Kill a member of all eligible monster species",
    KILLED_ALL_SPECIES),
ACH("Learn the Alphabet", "Kill a monster of each class from a to z",
    KILLED_A_Z_LOWERCASE),
ACH("LEARN THE ALPHABET",
    "Kill a monster of each class from A to Z (except I)",
    KILLED_A_Z_UPPERCASE),
/* "kill things in specific circumstances" group */
ACH("What It Was Made For", "Kill an appropriate monster with a Bane weapon",
    USED_CORRECT_BANE),
ACH("Kop Training Program", "Kill a monster with a rubber hose",
    KILLED_WITH_HOSE),
ACH("Buffy", "Destroy an undead with #turn", DESTROYED_VIA_TURN),
ACH("Bright Light, Bright Light", "Kill a gremlin with any light source",
    LIT_UP_GREMLIN),
ACH("Get Your Gas Mask", "Kill a monster with a stinking cloud",
    GASSED_MONSTER),
ACH("It's The Pits", "Kill something while you are stuck in a pit",
    KILLED_WHILE_IN_PIT),
ACH("Never Saw It Coming", "Kill an invisible monster without see invisible",
    KILLED_INVISIBLE),
ACH("Rope-a-Dope", "Kill a monster with a thrown aklys", KILLED_WITH_AKLYS),
ACH("Hulk Smash", "Kill a monster with a heavy iron ball",
    KILLED_WITH_IRONBALL),
ACH("DIRGE", "Instakill a monster with poison", INSTAPOISONED),
ACH("Poseidon", "Kill a sea monster with a trident", KILLED_WITH_TRIDENT),
ACH("Touch It With A 10 Foot Pole", "Kill a floating eye with a polearm",
    KILLED_EYE_POLEARM),
ACH("Bar Brawler", "Kill a monster with HP damage by smashing a bottle on it",
    KILLED_WITH_BOTTLE),
ACH("Rat Tail", "Kill a monster with a wet towel", KILLED_WITH_TOWEL),
ACH("Hammer of the Gods",
    "Kill a monster by throwing Mjollnir and having it return to your hand",
    MJOLLNIR_THROW_KILL),
ACH("Friendly Fire",
    "Get one hostile monster killed by another's projectile weapon",
    MVM_PROJECTILE_KILL),
ACH("Hoist With Its Own Petard",
    "Have a monster kill itself with its own wand zap",
    M_KILLED_BY_OWN_WAND),
ACH("Purple Rain",
    "Get at least 20 enemies eaten by purple worms on the Astral Plane",
    PURPLE_RAIN),
ACH("Off With Her Head!", "Decapitate an enemy with Vorpal Blade",
    DECAPITATED_ENEMY),
ACH("It's 4:20 Somewhere", "Stone a monster", STONED_A_MONSTER),
ACH("On Thin Ice", "Drown a monster by melting the ice beneath it",
    MELTED_ICE_KILL),
ACH("Some Nemesis You Are",
    "Kill your quest nemesis without them getting a move",
    ONE_SIDED_NEMESIS_KILL),
ACH("Faster than Light", "Kill a \"y\" monster in melee without it exploding",
    KILLED_LIGHT_MELEE),
/* "survive a hazard" group */
ACH("Part-time Statue", "Cure stoning", CURED_STONING),
ACH("The Blob", "Cure sliming", CURED_SLIMING),
ACH("Captain Ahab", "Survive being grabbed by a sea monster",
    SURVIVED_DROWNING),
ACH("Furry Little Problem", "Cure lycanthropy", CURED_LYCANTHROPY),
ACH("New Lease on Life", "Cure deathly illness", CURED_ILLNESS),
/* involving identification */
ACH("Diamond in the Rough", "Identify a gem with a touchstone",
    USED_TOUCHSTONE),
ACH("Curse the Darkness", "Identify a potion of oil by applying it",
    LIT_OIL_POTION),
ACH("Michelin Guide", "Identify both sources of food detection",
    ID_DETECT_FOOD),
ACH("Jeweler", "Identify all rings", IDENTIFIED_ALL_RINGS),
ACH("Mikado", "Identify all wands", IDENTIFIED_ALL_WANDS),
ACH("King of Bling", "Identify all amulets", IDENTIFIED_ALL_AMULETS),
ACH("Bookworm", "Identify all spellbooks", IDENTIFIED_ALL_BOOKS),
ACH("Scholar", "Identify all scrolls", IDENTIFIED_ALL_SCROLLS),
ACH("Alchemist", "Identify all potions", IDENTIFIED_ALL_POTIONS),
ACH("Fashionista",
    "Identify all non-obvious cloaks, helms, boots, and gloves",
    IDENTIFIED_ACCESSORIES),
ACH("Mineralogist", "Identify every type of valuable gemstone",
    IDENTIFIED_ALL_GEMS),
ACH("Geologist", "Identify all gray stones", IDENTIFIED_ALL_STONES),
ACH("Glass Menagerie", "Identify all worthless glass", IDENTIFIED_ALL_GLASS),
/* involving maintaining a conduct up to Quest */
ACH("Friends Not Food", "Receive the Quest with vegetarian conduct intact",
    PARTIAL_VEGETARIAN),
ACH("Animal Rights Activist", "Receive the Quest with vegan conduct intact",
    PARTIAL_VEGAN),
ACH("Anti Gulp", "Receive the Quest with foodless conduct intact",
    PARTIAL_FOODLESS),
ACH("No Help from Above", "Receive the Quest with atheist conduct intact",
    PARTIAL_ATHEIST),
ACH("Wax On Wax Off", "Receive the Quest with weaponless conduct intact",
    PARTIAL_WEAPONLESS),
ACH("Thou Shalt Not Kill", "Receive the Quest with pacifist conduct intact",
    PARTIAL_PACIFIST),
ACH("Me No Do Words Good", "Receive the Quest with illiterate conduct intact",
    PARTIAL_ILLITERATE),
ACH("Lost My Transmutation License",
    "Receive the Quest with polypileless conduct intact",
    PARTIAL_POLYPILELESS),
ACH("True to Form", "Receive the Quest with polyselfless conduct intact",
    PARTIAL_POLYSELFLESS),
ACH("Self Sufficient", "Receive the Quest with wishless conduct intact",
    PARTIAL_WISHLESS),
ACH("Leave the Artifacts to the Gods",
    "Receive the Quest with artifact wishless conduct intact",
    PARTIAL_ARTIWISHLESS),
ACH("Feeling Your Way",
    "Receive the Quest with zen (permablind) conduct intact",
    PARTIAL_ZEN),
ACH("Going Commando", "Receive the Quest with nudist conduct intact",
    PARTIAL_NUDIST),
ACH("Make Love, Not War",
    "Receive the Quest with genocideless conduct intact",
    PARTIAL_GENOCIDELESS),
ACH("No Help From The Floor",
    "Receive the Quest with Elberethless conduct intact",
    PARTIAL_ELBERETHLESS),
ACH("It's A Wonderful Trip",
    "Receive the Quest with permahallu conduct intact",
    PARTIAL_PERMAHALLU),
ACH("Silent World", "Receive the Quest with permadeaf conduct intact",
    PARTIAL_PERMADEAF),
ACH("Keep Your Fancy Toys",
    "Receive the Quest with artifactless conduct intact",
    PARTIAL_ARTIFACTLESS),
ACH("New Dungeon, Never Occupied",
    "Receive the Quest with bonesless conduct intact",
    PARTIAL_BONESLESS),
ACH("Extradimensionally Challenged",
    "Receive the Quest with swapchestless conduct intact",
    PARTIAL_SWAPCHESTLESS),
ACH("Fur Allergy", "Receive the Quest with petless conduct intact",
    PARTIAL_PETLESS),
ACH("Death Isn't So Cheap", "Receive the Quest with survivor conduct intact",
    PARTIAL_SURVIVOR),
/* involving entering a certain room/area */
ACH("Breaking and Entering", "Enter a vault", ENTERED_VAULT),
ACH("Who Was That David Person Anyway?", "Enter a treasure zoo", ENTERED_ZOO),
ACH("Absolute Monarchy", "Enter a throne room", ENTERED_THRONE_ROOM),
ACH("They're After Me Lucky Charms", "Enter a leprechaun hall",
    ENTERED_LEP_HALL),
ACH("Hive Mind", "Enter a beehive", ENTERED_BEEHIVE),
ACH("Day of the Dead", "Enter a graveyard", ENTERED_GRAVEYARD),
ACH("Team a Headquarters", "Enter an anthole", ENTERED_ANTHOLE),
ACH("Halt, Who Goes There?", "Enter a barracks", ENTERED_BARRACKS),
ACH("Dungeon Ecology", "Enter a swamp", ENTERED_SWAMP),
ACH("Statue Showcase", "Enter a cockatrice nest", ENTERED_COCKNEST),
ACH("Ocean's Eleven", "Enter Fort Ludios", ENTERED_LUDIOS),
/* involving doing all of a set of things */
ACH("Pilgrim", "Visit every type of aligned altar", VISITED_ALL_ALTARS),
ACH("The Grand Tour", "Visit each high altar on the Astral Plane",
    VISITED_HIGH_ALTARS),
ACH("All You Ever Wanted", "Get a wish from every wish source",
    WISH_FROM_EVERYTHING),
ACH("Hyperliterate",
    "Read every readable item in the game (scroll, book, coin, T-shirt, apron, "
    "magic marker, candy bar, credit card, fortune cookie)",
    READ_ALL_READABLE),
ACH("Bomb Squad", "Untrap all 8 types of traps that can be untrapped",
    UNTRAPPED_ALL_TRAPS),
ACH("Balanced Diet", "Eat one of each type of produce in one game",
    ATE_ALL_PRODUCE),
ACH("Smorgasbord", "Eat every type of food in the game", ATE_ALL_FOODS),
ACH("Extreme Omnivore",
    "Eat an object from every common, edible object class",
    ATE_EVERY_OBJCLASS),
ACH("Ultimate Polearm Geek",
    "Collect one of each of the 12 types of polearm",
    COLLECTED_ALL_POLEARMS),
/* involving artifact gifts */
ACH("Go Forth and Smite", "Receive an artifact gift from your deity",
    GRANTED_ARTI),
ACH("Too Little Too Late", "Get a sacrifice gift on the Astral Plane",
    GIFT_ON_ASTRAL),
ACH("Some Watery Tart", "Be gifted Excalibur", GOT_EXCALIBUR),
/* involving eating */
ACH("Superfood", "Gain strength by eating royal jelly", ATE_ROYAL_JELLY),
ACH("You Are What You Eat",
    "Gain an intrinsic by eating a corpse (or tin or glob)",
    GOT_CORPSE_INTRINSIC),
ACH("Magivore", "Gain a magical intrinsic by eating a piece of jewelry",
    INTRINSIC_FROM_JEWELRY),
ACH("Stunning Flavor", "Get stunned by eating bat meat", STUNNED_BAT_MEAT),
ACH("Wasn't a Meal, Wasn't Ready, And You Shouldn't Have Eaten It",
    "Eat a military ration", ATE_MILITARY_RATION),
ACH("Tastes Like Chicken", "Eat chickatrice or cockatrice meat",
    TASTED_COCKATRICE),
ACH("Heroes' Feast", "Eat at least 3 artifacts", ATE_3_ARTIFACTS),
ACH("Buzzing With Energy", "Gain at least 5 max Pw by eating corpses or tins",
    5_MAXPW_FROM_NEWTS),
ACH("Culinary Experience", "Gain a level by eating a tripe ration",
    LEVEL_FROM_TRIPE),
ACH("Wafer-Thin Mint", "Eat a wraith corpse while already satiated",
    SATIATED_ATE_WRAITH),
ACH("The Omelet of Yendor",
    "While carrying 3 eggs, spinach, wolfsbane, garlic, eucalyptus, violet "
    "fungus, oil, and a dented pot, put on an apron and wield a knife",
    OMELET_OF_YENDOR),
/* involving taming and pets */
ACH("Pet Whisperer", "Tame a monster by feeding it", TAMED_BY_FOOD),
ACH("Beast Master", "Tame a monster without using food", TAMED_NOT_BY_FOOD),
ACH("Shifty", "Tame a shapeshifter in any way", TAMED_SHAPECHANGER),
ACH("Cat Lady", "Tame seven \"f\" in the course of one game", TAMED_7_CATS),
ACH("Monkey Business", "Tame a monkey or ape by giving it a banana",
    BANANA_TAMED_Y),
ACH("Best Friends Forever", "Take your starting pet to full adulthood",
    FULLY_RAISED_PET),
ACH("Pet Rescue", "Free a pet from a trap that you didn't create yourself",
    UNTRAPPED_PET),
ACH("Distinction Without a Difference", "Polymorph a pet using a polytrap",
    POLYED_PET_ON_TRAP),
ACH("Tenacious Companion",
    "Have a pet survive all the way to the Astral Plane that you brought into the Plane of Earth",
    PET_THROUGH_PLANES),
/* involving bones */
ACH("Those Who Came Before", "Sight another player's bones pile",
    FOUND_BONES_PILE),
ACH("Imitation Bones", "Step onto the Rogue level fake bones pile",
    FOUND_ROGUE_BONES_PILE),
ACH("Recorporealized", "Draw the ghost of a former hero back into its body",
    RECORPOREALIZED_GHOST),
ACH("Double Bones", "Encounter two bones piles on a single level",
    DOUBLE_BONES),
ACH("Mr. Bones' Wild Ride",
    "Find the bones of four former heroes in one game",
    FOUND_FOUR_BONES),
/* involving tins */
ACH("Civilized Cooking", "Open a tin with a tin opener", OPENED_TIN_PROPERLY),
ACH("Exotic Meat", "Tin a unique monster", TINNED_UNIQUE_MON),
ACH("Tinned Out", "Use up a tinning kit", USED_UP_TINNING_KIT),
/* involving resurrection */
ACH("Reincarnation Cancelled",
    "Feel \"less hassled\" after blocking a troll's revival attempt",
    BLOCKED_TROLL_REVIVAL),
ACH("Well... Bye", "Prevent a Rider from resurrecting", PERMAKILLED_RIDER),
ACH("Only Mostly Dead", "Resurrect a corpse", REVIVIFIED_CORPSE),
ACH("Necromancer", "Resurrect a fallen pet and have it return tame",
    RESURRECTED_PET_TAME),
ACH("Asudem", "Revive Perseus", REVIVED_PERSEUS),
ACH("So Thou Thought Thou Couldst Resurrect Me",
    "Resurrect the Wizard of Yendor", RESURRECTED_WIZARD),
ACH("Insert Quarter to Continue", "Get life-saved", GOT_LIFESAVED),
/* involving shops */
ACH("Always Talk to the Wise Old Man", "Chat with Izchak", CHATTED_IZCHAK),
ACH("Dungeon Economics", "Buy or sell something at a shop", PURCHASED),
ACH("Conspicuous Consumption", "Buy an unidentified gem from a shop",
    BOUGHT_UNID_GEM),
ACH("The Art of the Deal", "Bankrupt a shopkeeper by getting all their money",
    BANKRUPTED_SHOPKEEPER),
ACH("Stop, Thief!", "Be chased by the Keystone Kops", ROBBED_SHOP),
ACH("The Last Chance Outpost", "Visit a shop at or below level 20",
    VISITED_DEEP_SHOP),
/* involving bribing */
ACH("Money Talks", "Bribe a demon lord with at least 25000 gold",
    BRIBED_DEMON_LORD),
ACH("The Negotiator", "Successfully bribe a demon lord with 1 zorkmid",
    BRIBED_WITH_1),
ACH("Look the Other Way", "Successfully bribe a mercenary", BRIBED_MERC),
/* involving monsters doing things to you */
ACH("A Little Light Larceny", "Get robbed by a nymph", ROBBED_BY_NYMPH),
ACH("Pot of Greed", "Have your purse lightened by a leprechaun",
    ROBBED_BY_LEPRECHAUN),
ACH("The Birds and the Bees", "Dally with a foocubus", ENCOUNTERED_FOOCUBUS),
ACH("Hello, Nurse!", "Get a nurse to heal you or raise your max HP",
    HEALED_BY_NURSE),
ACH("Tin Foil Hat", "Dodge a brainsucking attack", EVADED_BRAINSUCK),
ACH("Who You Gonna Call?", "Get scared by a ghost", SCARED_BY_GHOST),
ACH("Heisenberg", "Become uncertain about your position or velocity",
    HEISENBERG),
ACH("Yoink!", "Have the Amulet of Yendor stolen from you", AMULET_WAS_STOLEN),
/* involving gods and religion */
ACH("Indulgences", "Buy protection from a priest", BOUGHT_PROTECTION),
ACH("Orthodoxy", "Convert an altar", CONVERTED_ALTAR),
ACH("Potions of Booze Cost More Than That",
    "Get two bits for an ale from a priest", GOT_TWO_BITS),
ACH("Fumata Bianca", "Get a priest to grow up into a high priest",
    MON_GREW_TO_HIPRIEST),
ACH("Get Flashed", "Learn an item's beatitude from an altar", BUC_FROM_ALTAR),
/* unsorted */
ACH("The Royal Treatment", "Sit on a throne", USED_THRONE),
ACH("It'll Turn Up Later", "Drop a ring in a sink", DROPPED_RING_IN_SINK),
ACH("Isaac Newton", "Get fruit by kicking a tree", KICKED_FRUIT_TREE),
ACH("Not The Bees", "Get bees by kicking a tree", GOT_BEES_FROM_TREE),
ACH("Wash it Clean", "Uncurse an item in a fountain", UNCURSED_IN_FOUNTAIN),
ACH("Well-Read", "Learn a level 5+ spell", LEARNED_LV5_SPELL),
ACH("Hand Crafted", "Make a dragon scale mail", CREATED_DSM),
ACH("The Most Dangerous Game", "Catch a monster in a trap set by you",
    TRAPPED_MONSTER),
ACH("Foobear", "Catch a bearish monster in a bear trap you set",
    TRAPPED_BEAR_IN_BEAR_TRAP),
ACH("Liberated", "Get yourself unpunished", GOT_UNPUNISHED),
ACH("It's Called That For a Reason",
    "Come to earth by levitating over a sink", FELL_ONTO_SINK),
ACH("Scribe", "Write a scroll with a magic marker", WROTE_SCROLL),
ACH("First Edition", "Write a spellbook", WROTE_SPELLBOOK),
ACH("Dragon Rider", "Ride a dragon", RODE_DRAGON),
ACH("Shimmer", "Erosionproof an item", ERODEPROOFED_ITEM),
ACH("They Like Shiny Things", "Throw a gem at a unicorn", THREW_UNICORN_GEM),
ACH("Buried Treasure", "Dig up a grave not in a graveyard", DUG_UP_GRAVE),
ACH("Movers and Shakers", "Cause an earthquake", CAUSED_EARTHQUAKE),
ACH("Snake Charmer", "Pacify a snake with a flute", CHARMED_SNAKE),
ACH("Right Back at Ya",
    "Hit an enemy with an indirect strike from a thrown boomerang",
    HIT_WITH_BOOMERANG),
ACH("It Burnsss", "Deal silver damage to a silver-hating monster",
    DEALT_SILVER_DAMAGE),
ACH("Arrakis", "Make a crysknife", MADE_CRYSKNIFE),
ACH("Obligatory Pointy Hat", "Put on a cornuthaum as a wizard",
    WIZ_WORE_CORNUTHAUM),
ACH("Hobbit Skin", "Wear a mithril coat", WORE_MITHRIL_COAT),
ACH("The Words of the Prophets", "Read 3 random graffiti", FOUND_3_GRAFFITI),
ACH("Are You Still Pudding Farming", "Split a pudding", SPLIT_A_PUDDING),
ACH("Wearable Snack", "Wear a meat ring", WORE_MEAT_RING),
ACH("Frozen Treats", "Loot an ice box", LOOTED_ICE_BOX),
ACH("Six Pack", "Stock an ice box with 6 or more potions of booze",
    STOCKED_SIX_PACK),
ACH("Pygmalion", "Create a monster from a figurine", ANIMATED_FIGURINE),
ACH("Nerd", "Read a spellbook while wearing lenses and a T-shirt", NERD),
ACH("Recharged", "Charge an object", CHARGED_AN_OBJECT),
ACH("Get Your Kicks", "Kick an item embedded in a wall or rock",
    KICKED_EMBEDDED_ITEM),
ACH("Spiderman", "Grab an item with a grappling hook", SNAGGED_WITH_HOOK),
ACH("More Light",
    "Refill the Candelabrum after it burns out after the Invocation",
    REFILLED_CANDELABRUM),
ACH("Fill 'Er Up", "Fill an oil lamp with a potion of oil",
    REFILLED_OIL_LAMP),
ACH("Chemistry 101", "Alchemize two potions in an established recipe",
    ALCHEMIZED),
ACH("Escape to Discworld", "Read a Terry Pratchett novel", READ_NOVEL),
ACH("Near Death Experience",
    "Chat with Death while carrying a Terry Pratchett novel", TALKED_TO_DEATH),
ACH("Prize Inside!",
    "Get a spellbook by breaking a naturally-generated statue",
    GOT_SPBOOK_FROM_STATUE),
ACH("Ding-a-Ling", "Summon a nymph by ringing a cursed bell", SUMMONED_NYMPH),
ACH("Diver Down",
    "Dive into a moat or pool and retrieve an item from the bottom",
    GOT_OBJECT_UNDERWATER),
ACH("Ceiling Clinging Commute", "Ride a wumpus over water",
    RODE_WUMPUS_OVER_WATER),
ACH("Collapse the Wavefunction", "Open the box containing Schroedinger's Cat",
    OPENED_SCHROEDINGERS),
ACH("I'm Free! I'm Free!", "Free a prisoner", FREED_PRISONER),
ACH("Spam, Spam, Spam", "Read a scroll of mail", READ_MAIL),
ACH("Gender Swap", "Put on an amulet of change", CHANGED_GENDER),
ACH("Gender Bender", "Change gender two or more times", MULTI_GENDER_FLIP),
ACH("Motherhood", "Lay an egg", LAID_EGG),
ACH("Father Figure", "Hatch an egg not laid by you and have it come out tame",
    HATCHED_FOREIGN_EGG),
ACH("Shafted", "Fall down a shaft", FELL_DOWN_SHAFT),
ACH("Horatius at the Bridge", "Make soldiers extinct", EXTINCTED_ARMY),
ACH("Who's That Pretty Girl in the Mirror, There?",
    "Deflect a nymph with a mirror", DEFLECTED_NYMPH),
ACH("Under New Management", "Change your alignment by any means",
    CHANGED_ALIGNMENT),
/* TNNT TODO FOR 3.7: Groundhog Day is impossible in 3.7 */
ACH("Groundhog Day", "Return to a forgotten level", REVISITED_LOST_LEVEL),
ACH("Tainted", "Dip an item in unholy water", DIPPED_IN_UNHOLY),
ACH("Hydrochloric", "Dissolve iron bars with acid", DISSOLVED_IRONBARS),
ACH("Predictably, Nothing Happens", "Break an identified wand of nothing",
    BROKE_WAN_NOTHING),
ACH("Wanton Waste",
    "Break an identified wand of wishing with at least 1 charge",
    BROKE_WAND_WISHING),
ACH("Neutralized", "Cancel a monster", CANCELED_MONSTER),
ACH("Panacea", "Neutralize a harmful potion with a unicorn horn",
    NEUTRALIZED_POTION),
ACH("Croesus Competitor",
    "Have at least 50000 gold in open inventory at any one time",
    CARRIED_50000_GOLD),
ACH("Croesus Buyout",
    "Have at least 100000 gold in open inventory at any one time",
    CARRIED_100000_GOLD),
ACH("Palantir", "Successfully apply a crystal ball", APPLIED_CRYSTAL_BALL),
ACH("Peak Condition", "Raise all attributes to at least 18",
    MAXED_ATTRIBUTES),
ACH("Replacement Goldfish", "Cast create familiar", CREATED_FAMILIAR),
ACH("Pop the Question", "Give a ring of adornment when seduced by a succubus",
    GAVE_ADORNMENT),
ACH("No Soup For You!", "Get permanently expelled from the Quest",
    EXPELLED_FROM_QUEST),
ACH("Discerning Palate", "Gain intrinsic food appraisal", GOT_FOOD_APPRAISAL),
ACH("Banishing Spells Not Included", "Summon a demon lord",
    SUMMONED_DEMON_LORD),
ACH("Dust to Dust", "Wrest one last charge from a wand of wishing",
    WRESTED_WOW),
ACH("Teetotaler", "Neutralize a potion of booze with an amethyst",
    AMETHYST_BOOZE),
ACH("Spooky Scary Skeletons", "Get scared by the rattling of a skeleton",
    SCARED_OF_SKELETON),
ACH("Gullible", "Get out of trouble by successfully lying to a vault guard",
    LIED_TO_GUARD),
ACH("Luck of the Grasshopper", "Reach +13 Luck", GOT_13_LUCK),
ACH("Murphy's Law", "Reach -13 Luck", GOT_NEGATIVE_13_LUCK),
ACH("Looking Slick", "Grease a piece of armor", GREASED_ARMOR),
ACH("The Deathly Hallows",
    "Wield a wand of death while wearing an invisibility cloak and amulet of life saving",
    DEATHLY_HALLOWS),
ACH("Indiana Jones", "Disarm a monster with a bullwhip", DISARMED_WITH_WHIP),
ACH("Minesweeper",
    "Disarm all land mines in Fort Ludios without blowing any up",
    DISARMED_LUDIOS_MINES),
ACH("007", "Unlock something with a credit card", UNLOCKED_CREDIT_CARD),
ACH("Bob Ross", "Release a picture-painting demon from a camera",
    RELEASED_CAMERA_DEMON),
ACH("Got No Talent", "Scare something by playing music", SCARED_WITH_MUSIC),
ACH("Cookie Monster", "Read a fortune without eating its cookie",
    READ_FORTUNE_FOODLESS),
ACH("Say Hello To My Evil Friend",
    "Have a non-guaranteed demon lord gated in or summoned",
    GATED_IN_DEMON_LORD),
ACH("Make your Mark", "Engrave with an athame", ENGRAVED_WITH_ATHAME),
ACH("One Hit KO",
    "Deal a monster its maximum HP or more of damage (minimum 20)",
    ONE_HIT_KO),
ACH("Drink It To The Next Level", "Level up by drinking from a sink",
    LEVELED_UP_FROM_SINK),
ACH("You're About To Die, You Scrub",
    "Get a special message saying you're about to die, you scrub",
    ABOUT_TO_DIE),
ACH("Extinctionist Lite", "Perform genocide 5 or more times",
    GENOCIDED_5_TIMES),
ACH("Dudley's Hero", "Make newts extinct", EXTINCTED_NEWTS),
ACH("Ice Jesus", "Walk on top of lava unharmed", WALKED_ON_LAVA),
ACH("Rare Loot",
    "Acquire an athame that was generated in the inventory of a lich",
    GOT_LICH_ATHAME),
ACH("Boy Scout",
    "Complete Sokoban without incurring any penalties for rule-breaking",
    DID_PURE_SOKOBAN),
ACH("Sounds Hollow", "Reveal a secret door with a stethoscope",
    STETHOSCOPED_SDOOR),
ACH("Unorthodox Exit", "Reach the Planes via quaffing", PLANES_VIA_QUAFFING),
ACH("Paddling Pool", "Create pools from a fountain", WATER_GUSH_FORTH),
ACH("Practical Joke", "Have a tin blow up in your face", BOOBY_TRAPPED_TIN),
ACH("Amon Hen",
    "Get either of the two \"vision\" effects from sitting on a throne",
    VISION_FROM_THRONE),
ACH("Detonator", "Explode a large box or chest", EXPLODED_LARGE_BOX),
ACH("Lightbringer", "Light up at least 20 dark rooms in the Dungeons of Doom",
    LIT_20_ROOMS),
ACH("Cosmic Bungee Cord",
    "Get sent back a total of 15 or more levels by the mysterious force",
    MFORCE_15_LEVELS),
ACH("No Rulez", "Remove all watchmen from Minetown", GOT_RID_OF_WATCH),
ACH("Packing Light",
    "Successfully store a bag of holding inside another bag of holding",
    NESTED_BOH),
ACH("Shoo Off", "Make a monster of difficulty 5 or more escape the dungeon",
    MAKE_MON_ESCAPE),
ACH("What Have I Unleashed", "Make a demon lord escape the dungeon",
    MAKE_DLORD_ESCAPE),
ACH("Walking Bullseye", "Have +30 AC", GOT_30_AC),
ACH("Tanking It", "Have -50 AC", GOT_MINUS_50_AC),
ACH("Doorstopper",
    "Have a door resist 8 consecutive times when trying to open or close it",
    DOOR_RESIST_8X),
ACH("Practically Permanent",
    "Have more than 10,000 turns of a temporary intrinsic", LONG_TIMEOUT),
ACH("This One Goes To +8", "Successfully enchant a weapon to +8 or higher",
    ENCHANT_WEP_TO_8),
ACH("Glowy Blue Upgrade",
    "Successfully increase the enchantment on your weapon", ENCHANT_WEAPON),
ACH("Blasters' Handbook", "Blow up a boulder with a land mine",
    BLEW_UP_BOULDER),
ACH("Loew Life", "Create a golem from a pile of objects", POLY_MADE_GOLEM),
ACH("Never Tell Me the Odds",
    "Successfully cast a spell with a failure rate of 95% or greater",
    CAST_LOCHANCE_SPELL),
ACH("Archmage", "Successfully cast a level 7 spell", CAST_LV7_SPELL),
ACH("Leeroy Jenkins", "Cross the Castle drawbridge threshold within 30 turns"
    " of entering the level for the first time", ENTERED_CASTLE_QUICKLY),
ACH("Chocoban", "Acquire a candy bar generated in Sokoban", GOT_SOKO_CANDY),
ACH("Thirst For Punishment", "Drink all harmful types of potion (conf, blind,"
    " paral, hallu, sleep, booze, sick, acid)",
    DRANK_HARMFUL_POTS),
ACH("The Full Elven",
    "Deck yourself out in 5 pieces of +7 elven armor, then wield a +7 elven weapon",
    EQUIPPED_ALL_ELVEN_GEAR),
ACH("Drained and Regained",
    "Restore at least 3 lost levels with a single potion",
    RESTORED_3_LEVELS),
ACH("Vlad Was Here", "Find a Vlad trapdoor without triggering it",
    FOUND_VLAD_TRAPDOOR),
ACH("Three-Rail Shot",
    "Hit a monster with a diagonal ray that bounced at least 3 times",
    HIT_WITH_3_BOUNCES),

/* 3 free bits in tnntachieve4 */
/* 317 achievements defined */

#undef ACH

/*
TNNT TODO FOR 3.7: Here are some good achievement ideas we want to do but they
are best off waiting until 3.7 is released and TNNT rebases on it.

"Hit the Ceiling": Reach XL 30 and your role's highest rank. This will be a
vanilla achievement in 3.7.

Also, two achievements for hitting XL 10, and one somewhere around 20 (you get a
new rank at 10, but around 20 the options are 18 or 22 if we want to keep it
tied to ranks. Or just don't tie it to ranks and award it at 20.) This would
only be if we don't use the vanilla rank title achievements.

Dry out a wet towel with fire. That behavior only exists in 3.7.

Pacify your quest leader after angering them. Pre 3.7, if you anger them, you
just get kicked out and your game is unwinnable.
*/

/*
Preserving here for posterity: rejected achievement ideas, and the reasons for
rejecting them.

Category 1: achievements that amount to "end the game in a certain way" - even
if the achievement allows you to survive with lifesaving, they just end up being
not that interesting and there's a limit on how many you'd be able to get in one
game. This applies to both dying and ascending in whatever contrived
circumstance.
    - Go from full HP to 0 in one turn.
    - Escape in celestial disgrace.
    - After getting the Mines luckstone and Sokoban prize and leaving both
      branches, never return to either.
    - Go to heaven prematurely.

Category 2: achievements that are not easy to compute because they don't fit
cleanly into something a player can accomplish in one NetHack game, which is how
the achievement framework works.
    - Have an item you donated to the swap chest taken by someone else.
    - Play 60 turns in 60 games in less than 60 minutes.

Category 3: achievements that are just too boring or tedious. This is
subjective, to some extent. If it's likely that most players will see it as a
chore, and it isn't really that impressive to talk about, it may not make the
cut here.
    - Destroy some large amount of doors.
    - Dig out an entire level.

Category 4: achievements that amount to "make a quirky wish". While players might wish
for nonstandard things in order to accomplish achievements, such as the last
missing piece of magic armor for Fashionista, having the esoteric wish be the
only thing you need to get the achievement isn't interesting.
    - Wish for a boulder.


Other unsorted rejects:

Minesweeper should not be entirely locked behind Ludios; it should just involve
disarming some number of land mines over the game, which could be the ones in
Ludios but don't have to be.
    > That number would have to be pretty low in order to not still soft-lock
    the achievement behind Ludios existing and getting into it to disarm mines.

The +13 and -13 Luck achievements should require sustaining that level for some
period of time.
    > Players would already do this for +13 Luck, and would just boringly lock
    themselves in a closet for the duration of the -13 Luck.

Achievement for getting expelled from the Quest permanently.
    "Make your game unwinnable" achievement falls under the same principle as
    not having "die in a specific way" achievements. Plus, getting expelled from
    the Quest permanently won't be possible in 3.7.

Achievement for identifying a loadstone with a touchstone.
    Loadstones identify themselves pretty easily; the main challenge would be
    remembering to identify it with the touchstone before trying to drop it,
    which isn't very compelling as an achievement. Plus, you have to identify a
    loadstone anyway for Geologist.

Achievement for finishing a game with INT_MAX points.
    This was more achievable in 3.4.3 with automatable pudding farms; it's
    unclear whether it's achievable now, and in any case, getting it would be a
    huge slog.

Achievement for formally identifying an item you already had type-named with the
correct name.
    Given how easy it is to informally identify an item, or even just guess
    correctly when the pool is small (i.e. magic armor) this is not interesting
    enough to make the cut.

Achievement for restoring an attribute point with restore ability.
    Too similar to Drained and Regained.

Achievement for attempting to levelport deeper than Gehennom goes.
    This is commonly done to get to the Vibrating Square level, and even if you
    weren't planning to do that, the cost (1 scroll of teleportation) is
    minimal.

Achievement for performing genocide from all available sources
    There are only 2 sources and there's already an achievement for performing
    genocide 5 times, in which you're likely to have used both sources.
*/
