# v0.1 - Working 2D terrain map and zone distribution

## Known issues
- Elevation is not smooth across tiles.
- ZonePlanner only has a placeholder logic for assigning Zone types.

## Next Steps
- Global noise distribution instead of generating noise for each tile individually makes the terrain smoother.
- An approach to create smooth gameplay: 
    - The elevation map is generated first.
    - Split up the map into smaller chunks of tiles (UNASSIGNED zones)
    - The zone planner then assigns zone based on the generated elevation map. For example. nearly flat tiles are assigned to ARENA. We need to define some rules for this.


Other notes:

i need a logger which computes the different number of zones in a generated map. 

Have a utility to compute a map with requirements. like i want an arena, 2 flank regions. etc.

