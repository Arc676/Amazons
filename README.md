# Amazons

A C backend library and frontend command line program for the [Game of the Amazons](https://en.wikipedia.org/wiki/Game_of_the_Amazons). Inspired by [this Numberphile video](https://www.youtube.com/watch?v=kjSOSeRZVNg).

The Game of the Amazons is a board game in a similar family as go and chess. Each player controls a customizable number of Amazons that can move on a board of arbitrary size like queens in chess. After moving, the Amazons must shoot an arrow. The arrows also move like queens in chess. Amazons cannot move to or over a square that is occupied by an arrow or another Amazon belonging to either player. The first player unable to make a move loses. Equivalently, the last player able to make a legal move wins.

This implementation allows the board to be of any size and each player may play with an arbitrary number of Amazons in any starting position. The standard configuration is also available in the frontend. It involves a 10x10 board with each player controlling 4 Amazons in the following configuration:

```
...W..W...
..........
..........
W........W
..........
..........
B........B
..........
..........
...B..B...
```

We can say that a region is _controlled_ by a player if it is entirely closed off from the rest of the board by arrows and only contains Amazons belonging to that player. Only the controlling player can affect this region because Amazons belonging to the opponent cannot enter it. Regions not controlled by either player are _undecided_. However, a region can contain Amazons belonging to both players and still be decided, namely if neither player has legal moves in that region. This is the case when an enclosed region is entirely filled with Amazons and arrows, leaving no empty squares. In the source code, these are referred to as _shared_ regions. The game can continue as long as there is at least one undecided region on the board. If the entire board consists of shared regions or regions controlled by a single player, then the result of the game is also decided: as long as neither player makes a blunder by cutting themselves off from an area in a region they themselves control, then whichever player controls more squares will eventually win because their opponent will exhaust their supply of empty squares first.

The library provides region tracking functionality and the frontend will terminate the game if the result is entirely decided by region control.

For more details about the game, see [the Wikipedia page](https://en.wikipedia.org/wiki/Game_of_the_Amazons).

## Licensing

Project available under GPLv3. See `LICENSE` for full license text.
