
=======================================

Welcome to CozyCape!

Question)  Why "CozyCape"?
Answer)    I have no idea...

=======================================

Here is a quick introduction to the code base!

=======================================

Where to start: main.cpp
	1) Initializes SDL2
	2) Calls Game::run()
	3) Closes SDL2
 
 
Then, we move to Game::run()
	1) Opens the window
	2) Inits other services (clock, etc.)
	3) Runs the main game loop
	4) Close everything
 
The game loop is just drawing, calls game logic, caps framerate, etc.


How the map works:
	- The Game class stores all the entities
	- The Tiles class is an entity holding (what else) the tile details
	  - getTile() ==  0 : An empty tile
	  - getTile() ==  1 : A solid tile
	  - getTile() == -1 : Out of bounds


Entities:
	- Everything in the game world are entities
	- You can use them for custom logic and drawing
	- To add an entity to the world, just create it and pass it to Game
	- There are plans to make a message system...
	  - It consists of onMessage() right now.


Events: 
	- Poll events once a frame (done in game loop)
	- If you want to receive events:
		- Create or extend EventEar
		- Add the ear to an EventSpeaker or Events
	- If you want one object to receive events before another:
		- You can create an event hierarchy using EventSpeakers, or
		- You may extend EventSpeaker and implement custom logic
		
		
This is a brief introduction.
If you have questions, please ask!
