while true do
	while true do
		duckbot.listen()

		duckbot.say("Hi! You're here about printing the Quacker posters, right?")
		duckbot.say("How many posters did you want?")

		local amount_str = duckbot.listen()
		local amount = tonumber(amount_str)
		if amount == nil then
			duckbot.say("I'm sorry, I didn't understand that.")
			break
		elseif amount == 0 then
			duckbot.say("Well, I guess you don't want any posters then.")
			break
		elseif amount < 0 then
			duckbot.say("I'm sorry, I can't print a negative number of posters.")
			break
		end

		local cost = amount * 5

		duckbot.say("Okay, that'll be " .. cost .. " coins.")

		if cost > duckbot.getCoins() then
			duckbot.say("Hmm, you don't have that many coins. Sorry, I can't print your posters.")
			break
		end

		-- go print it
		duckbot.say("I'll go print them now. Just a moment!")

		-- move to printer
		duckbot.turnRight()
		duckbot.turnRight()
		duckbot.moveForward()

		-- do a dance
		duckbot.turnLeft()
		duckbot.turnLeft()
		duckbot.turnLeft()
		duckbot.turnLeft()
		duckbot.turnRight()
		duckbot.turnRight()
		duckbot.turnRight()
		duckbot.turnRight()
		duckbot.turnLeft()
		duckbot.turnLeft()
		duckbot.turnLeft()
		duckbot.turnLeft()
		duckbot.turnRight()
		duckbot.turnRight()
		duckbot.turnRight()
		duckbot.turnRight()
		duckbot.turnLeft()
		duckbot.turnLeft()
		duckbot.turnLeft()
		duckbot.turnLeft()

		-- move to desk
		duckbot.turnRight()
		duckbot.turnRight()
		duckbot.moveForward()

		duckbot.say("Done! Here's your posters. And I'll take the payment now.")

		npc.addItem(amount, "posters")
		npc.addCoins(-1 * cost)

		if npc.getItemCount("posters") > 100 then
			npc.win()
		end
	end
end