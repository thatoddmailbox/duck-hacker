while true do
	while true do
		local message = duckbot.listen()

		if message:lower():match("unlock") then
			duckbot.say("*BZZT* Alright, I'll unlock the gate. *BZZT*")
			npc.win()
		else
			duckbot.say("*BZZT* Sorry, didn't understand that. *BZZT*")
		end
	end
end