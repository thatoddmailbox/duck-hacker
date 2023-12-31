function trim(value)
	return value:match("^%s*(.-)%s*$")
end

while true do
	while true do
		local request = duckbot.listen()
		--print(request)
		duckbot.say("OK!")

		-- go into archives
		duckbot.turnRight()
		duckbot.moveForward()

		-- reorient so we can leave
		duckbot.turnLeft()
		duckbot.turnLeft()

		-- figure out what we got
		local requestItems
		requestItems, _ = request:gsub("Could you get me ", "", 1)

		local itemNames = {}
		local itemStatuses = {}

		for token in requestItems:gmatch("[^,]+") do
			local trimmedToken = trim(token)
			--print(trimmedToken)

			trimmedToken, _ = trimmedToken:gsub("a ", "", 1)
			trimmedToken, _ = trimmedToken:gsub("an ", "", 1)
			trimmedToken, _ = trimmedToken:gsub("and ", "", 1)
			trimmedToken, _ = trimmedToken:gsub("the ", "", 1)

			trimmedToken, _ = trimmedToken:gsub("?", "", 1)

			table.insert(itemNames, trimmedToken)

			local itemFound = false
			if trimmedToken == "regular map" then
				itemFound = true
			elseif trimmedToken == "historical map" then
				itemFound = true
			elseif trimmedToken == "green space map" then
				itemFound = true
			elseif trimmedToken == "secret map" then
				itemFound = true
			end

			if itemFound then
				table.insert(itemStatuses, "found")
			else
				table.insert(itemStatuses, "not found")
			end
		end

		-- leave archives
		duckbot.moveForward()
		duckbot.turnRight()

		local pluralS = ""
		if #itemNames > 1 then
			pluralS = "s"
		end
		duckbot.say("Here's what I found for your " .. #itemNames .. " request" .. pluralS .. ".")

		for i, itemName in ipairs(itemNames) do
			local itemStatus = itemStatuses[i]

			if itemStatus == "found" then
				duckbot.say("Here's the " .. itemName .. ".")
			elseif itemStatus == "not found" then
				duckbot.say("I couldn't find the " .. itemName .. ".")
			end
		end
	end
end