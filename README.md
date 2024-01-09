# duck-hacker
Duck Hacker is a game where you hack various things by programming DuckBots, small robots that run Lua code.

The game is written in C++ and uses OpenGL for rendering. The code is based on [tigame](https://github.com/thatoddmailbox/tigame)'s rendering engine, but with many internal changes.

SDL2 is also used to set up a window and initialize OpenGL. wxWidgets runs in a separate thread to provide the source code editor. Additionally, all bots run their Lua code in individual threads.

* print(duckbot.moveforward()) is empty instead of nil
* you can talk to bots even when not facing them (intercom level)
* level6 not randomizing?
* bump version

* no tutorial - hard to figure out what to do at first
* not clear how to open code editor
* play button kind of invisible at first - seems like gray bar blended in with window chrome
* not clear that you could drag or zoom
* stupid msaa aliasing bug
* music was not relevant
* beginning was good, later levels kind of dragged on
	* need smaller wins throughout level / dopamine hits
	* felt punishing -> finish level and reward is more work (specifically last two levels, both hard)
* level5 -> tricky to get the "or" part, but possible via hinting
* need more qa / attempting to break it (i.e. the talking bug)
* level6 felt out of place -> earlier levels were more about injection, so timing attack felt weird
* unused functions confusing (i.e. getCoins()) -> better to not document?
* long lua programming docs mostly not relevant (arrays not needed, functions not really needed, all the intro stuff not needed)
* syntax highlighting helped a lot
* "real" code editor component helped -> probably was worth it to integrate wxWidgets vs custom imgui component

FILL IN:
* not using engine -> probably good?
* having tigame as base -> very useful
* also, having this as development of tigame code was good
	* allowed understanding where tigame code needs to go without being restrained by structure
* good to have other codebases to refer to (inboxlens for modals, duck hero for SDL mixer and other code)

* TODO post on pwiki info about game
* TODO make debrief list, write inspo questions,
* TODO write smolduck debrief
* TODO how was ideas.txt?

* planning ahead of time worked very well
	* clear idea of the levels, made it easy to add storyline and actually build the level
	* became clear what features were needed (the conversation system) and what was not (complicated inventory system, multiple player-controlled duckbots)
* very productive several days -> due to dec 25 deadline? afterwards seemed to slow down
* many single projects: smolduck -> thesis proposal -> fem project -> this
	* need more debrief time
	* 100% utilization, not many cycles free to handle other things

* optimization did not matter (mostly expected)
* didn't really use much of lua -> not much functions or looping
* long play time, but most was on last two levels i think

## Building
Make sure to run `git submodule update --init --recursive` to clone the submodules, as some dependencies are included that way.

Then, follow the build instructions for your platform:
* [Windows build instructions](./docs/build-windows.md).
* [Linux build instructions](./docs/build-linux.md).
