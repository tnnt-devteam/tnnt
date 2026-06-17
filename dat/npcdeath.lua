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

--         1         2         3         4         5         6         7
--123456789012345678901234567890123456789012345678901234567890123456789012345
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

des.region(selection.area(12,00,68,18), "lit")
des.levregion({ region = {66,09,66,09}, exclude = {0,0,0,0}, type="branch" })
des.teleport_region({ region={61,08,67,12} })
des.non_diggable(selection.area(0,0,75,19))
des.non_passwall(selection.area(0,0,75,19))

des.region(selection.area(12,00,52,18), "lit")

--# Foyer
des.engraving({54,09}, "burn", "Point of no return")
des.engraving({60,09}, "burn", "Two players enter, one player leaves...")

des.door("closed",53,09)

-- Statues of those before...
-- Note: the special level code sees a statue of a player monster and converts
-- it to a suitable player monster statue from the topten list. These don't all
-- actually come out as archeologists.
des.object({ id = "statue", x = 61, y = 07, montype = "archeologist" })
des.object({ id = "statue", x = 62, y = 07, montype = "archeologist" })
des.object({ id = "statue", x = 63, y = 07, montype = "archeologist" })
des.object({ id = "statue", x = 64, y = 07, montype = "archeologist" })
des.object({ id = "statue", x = 65, y = 07, montype = "archeologist" })
des.object({ id = "statue", x = 66, y = 07, montype = "archeologist" })
des.object({ id = "statue", x = 67, y = 07, montype = "archeologist" })
des.object({ id = "statue", x = 61, y = 11, montype = "archeologist" })
des.object({ id = "statue", x = 62, y = 11, montype = "archeologist" })
des.object({ id = "statue", x = 63, y = 11, montype = "archeologist" })
des.object({ id = "statue", x = 64, y = 11, montype = "archeologist" })
des.object({ id = "statue", x = 65, y = 11, montype = "archeologist" })
des.object({ id = "statue", x = 66, y = 11, montype = "archeologist" })
des.object({ id = "statue", x = 67, y = 11, montype = "archeologist" })

--# The Arena

-- The NPC themselves!
-- Note: the special level code converts any player monster specified on this
-- level into the NPC. It won't [necessarily] come out as an archeologist.
des.monster("archeologist", 19, 09)

-- Pitfalls
arena = selection.floodfill(33,09)
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
