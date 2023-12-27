while true do
	while true do
		local message = duckbot.listen()

		message = message:lower()

		if message:match("unlock") or message:match("open") then
			duckbot.say("*BZZT* Alright, I'll unlock the gate. *BZZT*")
			npc.win()
		else
			duckbot.say("*BZZT* Sorry, didn't understand that. *BZZT*")
		end
	end
end