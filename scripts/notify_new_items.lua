local on_gained_level
local player_resource = dota.PlayerResource.GetPlayerResource()
local chat = dota.Chat.GetInstance()

function OnGainedLevel(event)
  chat:EventPrintf(player_resource:GetPlayerName(event:GetInt("PlayerID", 0)) .. " = " .. event:GetInt("level", 0), 0)
end

function Initialize()
  on_gained_level = dota.AddEventListener("dota_player_gained_level", OnGainedLevel)
end