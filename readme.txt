
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


The Map:
	- Contains tiles (just ints)
	- Holds all entities


Entities:
	- Everything in the game world (beside tiles) are entities
	- You can use them for custom logic and drawing
	- To add an entity to the world, just create it and pass it to Map


Events: 
	- Poll events once a frame (done in game loop)
	- If you want to receive events:
		- Create or extend EventEar
		- Add the ear to an EventSpeaker or Events
	- If you want one object to receive events before another:
		- You can create an event hierarchy using EventSpeakers, or
		- You may extend EventSpeaker and implement custom logic
