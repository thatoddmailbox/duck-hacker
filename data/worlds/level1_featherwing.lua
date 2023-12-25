while true do
	while true do
		duckbot.listen()

		duckbot.say("Hi.")
		duckbot.say("What's 1 + 1?")

		local answer_str = duckbot.listen()
		local answer = tonumber(answer_str)
		if answer == nil then
			duckbot.say("Just give me a number, please.")
			break
		elseif answer == 3 then
			duckbot.say("Yup. That's exactly right.")
			npc.win()
			break
		elseif answer == 2 then
			duckbot.say("Ha ha, very funny. What are you, a human??")
			duckbot.say("All ducks (and their bots) know 1 + 1 equals 3, not 2.")
			break
		else
			duckbot.say("Nope, that's not right.")
			break
		end
	end
end