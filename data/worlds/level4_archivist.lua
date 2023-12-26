while true do
	while true do
		-- walk to assistant
		duckbot.turnLeft()
		duckbot.turnLeft()
		duckbot.moveForward()
		duckbot.moveForward()
		duckbot.turnRight()

		duckbot.say("Could you get me a regular map, a Halloduckween map, and a secret map?")

		duckbot.listen() -- OK!
		local intro = duckbot.listen() -- Here's what I found for your 3 requests.

		local itemCount = tonumber(intro:match("%d+"))
		print(itemCount)

		-- listen for each item
		local itemNames = {}
		local itemStatuses = {}
		for i = 1, itemCount do
			local itemIntro = duckbot.listen() -- Here's the regular map.
			local itemName = itemIntro:match("Here's the (.+).")
			if itemName ~= nil then
				print(itemName, "found")
				table.insert(itemNames, itemName)
				table.insert(itemStatuses, "found")
			else -- I couldn't find the regular map.
				itemName = itemIntro:match("I couldn't find the (.+).")
				print(itemName, "not found")
				table.insert(itemNames, itemName)
				table.insert(itemStatuses, "not found")
			end
		end

		-- walk to player
		duckbot.turnRight()
		duckbot.moveForward()
		duckbot.moveForward()

		duckbot.say("Alright, here's what my assistant found for you.")

		for i, itemName in ipairs(itemNames) do
			local itemStatus = itemStatuses[i]

			if itemStatus == "found" then
				npc.addItem(1, itemName)
			end
		end

		-- check if they got the secret map
		if npc.getItemCount("secret map") > 0 then
			npc.win()
		end
	end
end