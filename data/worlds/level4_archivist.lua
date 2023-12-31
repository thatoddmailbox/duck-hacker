function trim(value)
	return value:match("^%s*(.-)%s*$")
end

while true do
	duckbot.listen()

	duckbot.say("Hi, welcome to the Duckland University Archives!")
	duckbot.say("We're giving out maps of our campus. You can pick two kinds of map.")
	duckbot.say("We have regular, historical, and green space maps!")

	duckbot.say("What's the first type of map you want?")
	local firstMap = trim(duckbot.listen()):lower()
	if firstMap == "secret" then
		duckbot.say("I'm sorry, I can't help you with that. Goodbye.")
		duckbot.turnLeft()
		duckbot.turnLeft()
		break
	end

	duckbot.say("What's the second type of map you want?")
	local secondMap = trim(duckbot.listen()):lower()
	if secondMap == "secret" then
		duckbot.say("I'm sorry, I can't help you with that. Goodbye.")
		duckbot.turnLeft()
		duckbot.turnLeft()
		break
	end

	duckbot.say("OK, one moment while I ask my assistant to get that for you.")

	-- walk to assistant
	duckbot.turnLeft()
	duckbot.turnLeft()
	duckbot.moveForward()
	duckbot.moveForward()
	duckbot.turnRight()

	duckbot.say("Could you get me a " .. firstMap .. " map, and a " .. secondMap .. " map?")

	duckbot.listen() -- OK!
	local intro = duckbot.listen() -- Here's what I found for your 3 requests.

	local itemCount = tonumber(intro:match("%d+"))
	--print(itemCount)

	-- listen for each item
	local itemNames = {}
	local itemStatuses = {}
	for i = 1, itemCount do
		local itemIntro = duckbot.listen() -- Here's the regular map.
		local itemName = itemIntro:match("Here's the (.+).")
		if itemName ~= nil then
			--print(itemName, "found")
			table.insert(itemNames, itemName)
			table.insert(itemStatuses, "found")
		else -- I couldn't find the regular map.
			itemName = itemIntro:match("I couldn't find the (.+).")
			--print(itemName, "not found")
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