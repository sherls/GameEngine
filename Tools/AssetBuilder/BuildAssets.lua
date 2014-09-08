--[[
	This script is run from cAssetBuilder and contains most of the logic that actually builds the individual assets
]]

-- Data
--=====

local s_directory_assets
local s_directory_data
local s_directory_tools
local s_path_assetList

-- Helper Function Declarations
--=============================

local --[=[function]=] BuildAssets_helper
local --[=[function]=] ExecuteCommand = os.execute
local --[=[function]=] LoadAssetListFromFile

-- Build Assets
--=============

local function BuildAssets( i_directory_assets, i_directory_data, i_directory_tools, i_path_assetList )
	-- Store arguments
	do
		s_directory_assets = i_directory_assets
		s_directory_data = i_directory_data
		s_directory_tools = i_directory_tools
		s_path_assetList = i_path_assetList
	end
	-- Load the list of assets to be built
	local assetListFromFile = LoadAssetListFromFile()
	-- Build each of the assets on the list
	-- (if required, e.g. it doesn't exist or is out of date)
	BuildAssets_helper( assetListFromFile )
end

-- Helper Function Definitions
--============================

--[=[function BuildAssets_helper]=]
do
	local --[=[function]=] OutputAssetListError	-- Helper function declarations
	--
	BuildAssets_helper = function( i_assetListFromFile )
		local shouldDependenciesBeIgnored = i_assetListFromFile.shouldAssetsAlwaysBeBuilt
		-- Iterate through each asset type
		local i_assets = i_assetListFromFile.assets
		if i_assets ~= nil then
			if next( i_assets ) then
				local didAllAssetsBuildSuccessfully = true
				for assetType, i_assetTypeInfo in pairs( i_assets ) do
					if type( assetType ) == "string" then
						if type( i_assetTypeInfo ) == "table" then
							-- Get the tool that is used to build this asset type
							local o_command
							do
								local i_tool = i_assetTypeInfo.tool
								if type( i_tool ) == "string" then
									o_command = s_directory_tools .. i_tool .. ".exe"
								elseif i_tool == nil then
									OutputAssetListError( "The asset type \"" .. assetType .. "\"'s table must have a \"tool\" field,"
										.. " which is the name (minus the file extension) of the tool that will be run to build "
										.. assetType )
								else
									OutputAssetListError( "The asset type \"" .. assetType .. "\"'s \"tool\" field must be a string"
										.. " (instead of a " .. type( i_tool )
										.. ") which is the name (minus the file extension) of the tool that will be run to build "
										.. assetType )
								end
							end
							-- Get the extensions to use with the source and target files
							local o_extensions
							do
								local i_extensions = i_assetTypeInfo.extensions
								if type( i_extensions ) == "table" then
									local i_source = i_extensions.source
									if i_source == nil then
										OutputAssetListError( "The asset type \"" .. assetType .. "\"'s extensions must have a \"source\" field,"
											.. " which is the file extension of source " .. assetType )
									elseif type( i_source ) ~= "string" then
										OutputAssetListError( "The asset type \"" .. assetType .. "\"'s extensions' \"source\" field must be a string"
											.. " (instead of a " .. type( i_source ) .. ") which is the file extension of source " .. assetType )
									end
									local i_target = i_extensions.target
									if i_target == nil then
										OutputAssetListError( "The asset type \"" .. assetType .. "\"'s extensions must have a \"target\" field,"
											.. " which is the file extension of target " .. assetType )
									elseif type( i_target ) ~= "string" then
										OutputAssetListError( "The asset type \"" .. assetType .. "\"'s extensions' \"target\" field must be a string"
											.. " (instead of a " .. type( i_target ) .. ") which is the file extension of target " .. assetType )
									end
									o_extensions = { source = i_source, target = i_target }
								elseif i_extensions == nil then
									OutputAssetListError( "The asset type \"" .. assetType .. "\"'s table must have an \"extensions\" field,"
										.. " which contains the file extensions of the source and target "
										.. assetType )
								else
									OutputAssetListError( "The asset type \"" .. assetType .. "\"'s \"extensions\" field must be a table"
										.. " (instead of a " .. type( i_extensions )
										.. ") which contains the file extensions of the source and target "
										.. assetType )
								end
							end
							local shouldDependenciesBeIgnored = shouldDependenciesBeIgnored or i_assetTypeInfo.shouldAssetsAlwaysBeBuilt
							-- Iterate through each asset
							local i_names = i_assetTypeInfo.names
							if type( i_names ) == "table" then
								if #i_names > 0 then
									local directory_assets = s_directory_assets .. assetType .. "/"
									local directory_data = s_directory_data .. assetType .. "/"
									for i, i_name in ipairs( i_names ) do
										if type( i_name ) == "string" then
											local path_source = directory_assets .. i_name .. "." .. o_extensions.source
											local path_target = directory_data .. i_name .. "." .. o_extensions.target
											local dontThrowErrorWhenOutputtingMessage = false
											if DoesFileExist( path_source ) then
												CreateDirectoryIfNecessary( path_target )
												local doesAssetNeedToBeBuilt
												if not shouldDependenciesBeIgnored then
													if DoesFileExist( path_target ) then
														-- The target should be built if it's older than the source
														doesAssetNeedToBeBuilt =
															GetLastWriteTime( path_source ) > GetLastWriteTime( path_target )
													else
														doesAssetNeedToBeBuilt = true
													end
												else
													doesAssetNeedToBeBuilt = true
												end
												if doesAssetNeedToBeBuilt then
													print( "Building " .. assetType .. ": " .. i_name )
													-- Create the command to build the asset
													local command = "\"\"" .. o_command
														.. "\" \"" .. path_source
														.. "\" \"" .. path_target .. "\"\""
													-- Execute the command
													if not ExecuteCommand( command ) then
														didAllAssetsBuildSuccessfully = false
													end
												end
											else
												OutputAssetListError( "The specified source file " .. path_source
													.. " (name #" .. tostring( i ) .. " of asset type \"" .. assetType
													.. "\") doesn't exist", dontThrowErrorWhenOutputtingMessage )
												didAllAssetsBuildSuccessfully = false
											end
										else
											OutputAssetListError( "The asset type \"" .. assetType .. "\"'s name #" .. tostring( i )
												.. " must be a string (instead of a " .. type( i_name )
												.. ") which is the name of an asset to build (minus the file extension)" )
										end
									end
								else
									OutputAssetListError( "The asset type \"" .. assetType .. "\"'s \"name\" field is empty"
										.. " (it should include the names of the " .. assetType .. " to be built)..."
										.. " If you don't know why you're getting this error contact John-Paul" )
								end
							elseif i_names == nil then
								OutputAssetListError( "The asset type \"" .. assetType .. "\"'s table must have a \"names\" field,"
									.. " which contains the names of the " .. assetType .. " that should be built" )
							else
								OutputAssetListError( "The asset type \"" .. assetType .. "\"'s \"names\" field must be a table"
									.. " (instead of a " .. type( i_names )
									.. ") which contains the names of the " .. assetType .. " that should be built" )
							end
						else
							OutputAssetListError( "The asset type \"" .. assetType .. "\" must be assigned a table"
								.. " (instead of a " .. type( i_assetTypeInfo ) .. ")" )
						end
					else
						OutputAssetListError( "Every asset type in the \"assets\" table in the AssetList.lua file must be a string"
							.. " (one of them is a " .. type( assetType ) .. ")" )
					end
				end
				if not didAllAssetsBuildSuccessfully then
					OutputAssetListError( "At least one asset build command returned an error code" )
				end
			else
				OutputAssetListError( "The table of assets in the AssetList.lua file is empty"
					.. " (it must include a table for each asset type that should be built, e.g. \"Shaders\" and \"Textures\")..."
					.. " If you don't know why you're getting this error contact John-Paul" )
			end
		else
			OutputAssetListError( "The AssetList.lua file _must_ have a table named \"assets\" (case-sensitive)..."
				.. " If you don't know why you're getting this error contact John-Paul" )
		end
	end

	OutputAssetListError = function( i_errorMessage, i_shouldThrowError )
		if i_shouldThrowError ~= false then
			local stackLevel_assetList = 0
			error( s_path_assetList .. ":0: " .. i_errorMessage, stackLevel_assetList )
		else
			print( s_path_assetList .. " : error: " .. i_errorMessage )
		end
	end
end

LoadAssetListFromFile = function()
	local stackLevel_assetList = 0

	local assetListFromFile = {}
	do
		local defaultMode = nil
		local fAssetList, errorMessage = loadfile( s_path_assetList, defaultMode, assetListFromFile )
		if fAssetList then
			fAssetList()
		else
			error( errorMessage, stackLevel_assetList )
		end
	end
	return assetListFromFile
end

-- Return
--=======

return BuildAssets
