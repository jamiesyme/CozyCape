
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


Events: 
	- Poll events once a frame (done in game loop)
	- If you want to receive events:
		- Create or extend EventEar
		- Add the ear to an EventSpeaker or Events
	- If you want one object to receive events before another:
		- You can create an event hierarchy using EventSpeakers, or
		- You may extend EventSpeaker and implement custom logic
