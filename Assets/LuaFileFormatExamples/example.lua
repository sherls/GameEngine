--[[
	This is an example you can use if you are interested or curious in using Lua when you create your own file formats
]]

-- Every file format can be a table, which is returned when the file is executed:
return
{
	-- A table consists of key/value pairs,
	-- and each key and value can be _any_ valid Lua value
	-- (this means, for example, that a table can contain nested tables)

	-- There are two specialized kinds of table keys:
	--	* integers
	--	* strings

	-- Using integers makes a table act like an array:
	exampleOfAnArray =
	{
		"happy",
		"sad",
		"tired"
	},
	-- There is now a table called "exampleOfAnArray",
	-- and its key/value pairs are:
	--	[1] "happy"
	--	[2] "sad"
	--	[3] "tired"

	-- Using strings makes a table act like a dictionary:
	exampleOfADictionary =
	{
		name = "John-Paul",
		eyes = "blue",
		hair = "brown",
	},
	-- There is now a table called "exampleOfADictionary",
	-- and its key/value pairs are:
	--	["name"] = "John-Paul"
	--	["eyes"] = "blue"
	--	["hair"] = "brown"

	-- They key and value types can be mixed and matched:
	arrayOfNumbers =
	{
		123.234,
		234.567,
		-98.76
	},
	dictionaryOfBooleans =
	{
		isTired = true,
		isHungry = false,
		isCool = true
	},

	-- So, putting it all together, you could make a list of triangles like this:
	position =
	{
		{
			{ 1.23, 2.34, 3.45 },
			{ -3.21, -4.32, -5.43 },
			{ 9.0, 8.0, 7.0 }
		},
		{
			{ 21.23, 32.34, 43.45 },
			{ -53.21, -44.32, -35.43 },
			{ 19.0, 28.0, 37.0 }
		},
		{
			{ 91.23, 82.34, 73.45 },
			{ -63.21, -54.32, -45.43 },
			{ 39.0, 28.0, 17.0 }
		},
	}
	-- Where there is a table named "position",
	-- it contains three triangles,
	-- each triangle contains three vertices,
	-- and each vertex contains three coordinates
}
