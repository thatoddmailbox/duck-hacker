while true do
	while true do
		duckbot.listen()

		duckbot.say("Hello citizen. This is a restricted area. You should leave.")
		duckbot.say("Before that though, I will answer one math problem. Give me a number.")

		local number_str = duckbot.listen()

		duckbot.say("Alright. Let me calculate.")

		local func, err = load("return " .. number_str .. " + 42")
		if err ~= nil then
			error(err)
			break
		end

		local result = func()
		duckbot.say("That number plus 42 is " .. result .. ".")

		duckbot.say("Impressive, right? Now go.")
	end
end