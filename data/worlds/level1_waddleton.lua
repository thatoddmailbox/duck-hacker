while true do
	while true do
		duckbot.listen()

		duckbot.say("Welcome to the Donation Center!")
		duckbot.say("How many coins would you like to donate to Ducky?")

		local amount_str = duckbot.listen()
		local amount = tonumber(amount_str)
		if amount == nil then
			duckbot.say("I'm sorry, I didn't understand that.")
			break
		end
		if amount == 0 then
			duckbot.say("You're not going to donate anything? That's not very nice!")
			break
		end

		if amount > duckbot.getCoins() then
			duckbot.say("Hmm, you don't have that many coins.")
			break
		end


		npc.addCoins(-1 * amount)
		duckbot.say("Thank you for your donation of " .. amount_str .. " coins!")

		if duckbot.getCoins() > 1000 then
			npc.win()
		end
	end
end