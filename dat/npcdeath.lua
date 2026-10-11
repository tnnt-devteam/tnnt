-- NetHack 3.6   npcdeath.des     $NHDT-Date: 1569269591 2019/09/23 20:13:11 $  $NHDT-Branch: NetHack-3.6 $:$NHDT-Revision: 1.27 $
--       Copyright (c) 1989 by Jean-Christophe Collet
--       Copyright (c) 1992,1993 by Izchak Miller, David Cohrs,
--                      and Timo Hakulinen
-- NetHack may be freely redistributed.  See license for details.
--
--
-- "NPC Deathmatch" by the TNNT devteam
--

des.level_init({ style = "solidfill", fg = " " });

des.level_flags("mazelevel");

des.level_flags("nommap", "noteleport", "hardfloor")
des.message("Upon exiting the portal, you immediately feel on edge.")
des.message("You feel as if you've stumbled across something very dangerous.")
des.message("You resist the urge to flee.")

--        1         2         3         4         5         6         7
--23456789012345678901234567890123456789012345678901234567890123456789012345
des.map([[
             ----------------                            
          ----LLLLLLLLLLLLLL----                         
       ----LLLL.....--.....LLLL----                      
     ---LLLL........--........LLLL---                    
   ---LLL........................LLL---                  
 ---LLL.............LL.............LLL---                
--LLL........--.....LL.....--........LLL--      ---------
|LL..........--.....LL.....--..........LL|      |.......|
|L..................LL...................|-------.......|
|L.......................................+..............|
|L..................LL...................|-------.......|
|LL..........--.....LL.....--..........LL|      |.......|
--LLL........--.....LL.....--........LLL--      ---------
 ---LLL.............LL.............LLL---                
   ---LLL........................LLL---                  
     ---LLLL........--........LLLL---                    
       ----LLLL.....--.....LLLL----                      
          ----LLLLLLLLLLLLLL----                         
             ----------------                            
]]);

des.region(selection.area(00,00,56,18), "lit")
des.levregion({ region = {54,09,54,09}, exclude = {0,0,0,0}, type="branch" })
des.teleport_region({ region={49,08,55,12} })
des.non_diggable(selection.area(0,0,56,18))
des.non_passwall(selection.area(0,0,56,19))

-- no random monster spawning in here
des.exclusion({ type = "monster-generation", region = {00,00,56,18} })

-- Define a themed room in the arena so that we can trigger the match start when
-- the player enters this room.
-- Note that the rectangle defined here ends one space *before* the space
-- adjacent to the door. This is because all rooms expand by 1 in all directions
-- to account for presumed walls, and we don't want the player to trigger the
-- deathmatch from the door's space itself. This expansion is also why an
-- irregular region doesn't work.
des.region({ region={00,00,39,18}, type="themed", filled=0, lit=1 })

--# Foyer
des.engraving({42,09}, "burn", "Point of no return")
des.engraving({48,09}, "burn", "Two players enter, one player leaves...")

des.door("closed",41,09)

-- Statues of those before...
-- Note: the special level code sees a statue of a player monster and converts
-- it to a suitable player monster statue from the topten list. These don't all
-- actually come out as archeologists.
des.object({ id = "statue", x = 49, y = 07, montype = "archeologist" })
des.object({ id = "statue", x = 50, y = 07, montype = "archeologist" })
des.object({ id = "statue", x = 51, y = 07, montype = "archeologist" })
des.object({ id = "statue", x = 52, y = 07, montype = "archeologist" })
des.object({ id = "statue", x = 53, y = 07, montype = "archeologist" })
des.object({ id = "statue", x = 54, y = 07, montype = "archeologist" })
des.object({ id = "statue", x = 55, y = 07, montype = "archeologist" })
des.object({ id = "statue", x = 49, y = 11, montype = "archeologist" })
des.object({ id = "statue", x = 50, y = 11, montype = "archeologist" })
des.object({ id = "statue", x = 51, y = 11, montype = "archeologist" })
des.object({ id = "statue", x = 52, y = 11, montype = "archeologist" })
des.object({ id = "statue", x = 53, y = 11, montype = "archeologist" })
des.object({ id = "statue", x = 54, y = 11, montype = "archeologist" })
des.object({ id = "statue", x = 55, y = 11, montype = "archeologist" })

--# The Arena

-- The NPC themselves!
-- Note: the special level code converts any player monster specified on this
-- level into the NPC. It won't [necessarily] come out as an archeologist.
des.monster("archeologist", 07, 09)

-- Pitfalls
arena = selection.floodfill(21,09)
for i=1,4 do
  des.trap("spiked pit", arena:rndcoord(1))
  des.trap("bear", arena:rndcoord(1))
  des.trap("land mine", arena:rndcoord(1))
end
for i=1,2 do
  des.trap("rolling boulder", arena:rndcoord(1))
  des.trap("fire", arena:rndcoord(1))
end
des.trap("sleep gas", arena:rndcoord(1))
des.trap("polymorph", arena:rndcoord(1))


-- end npcdeath.des
