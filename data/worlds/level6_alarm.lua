local correct_pincode = npc.randomDigits(6)

duckbot.say("Alarm system active. To disable it, provide the six digit pincode.")
while true do
	while true do
		local pincode = duckbot.listen()
		if #pincode ~= 6 then
			duckbot.say("Invalid pincode. It must be six digits.")
			break
		end

		-- check each digit of pincode
		local correct = true
		for i = 1, 6 do
			local digit = pincode:sub(i, i)
			if digit ~= correct_pincode:sub(i, i) then
				correct = false
				break
			end

			duckbot.sleep(10)
		end

		if correct then
			duckbot.say(pincode .. " is the correct pincode. Alarm system disabled.")
			npc.win()
		else
			duckbot.say("Incorrect pincode. Alarm system still active.")
		end
	end
end