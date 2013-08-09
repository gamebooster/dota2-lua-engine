local hook = nil
local player_resource = dota.PlayerResource.GetPlayerResource()

function OnPaint()
  local players = dota.FindEntities(dota.kDotaPlayer)
  local local_player = dota.GetLocalPlayer()
  local local_team   = local_player:GetTeamIndex()
  
  local offset_counter = 0
  
  for _,player in ipairs(players) do
    local player_id = player:GetPlayerId()
    
    local player_name = player_resource:GetPlayerName(player_id)
    local level       = player_resource:GetLevel(player_id)
    local gold        = player_resource:GetUnreliableGold(player_id) + player_resource:GetReliableGold(player_id)
    local last_hits   = player_resource:GetLastHitCount(player_id)
    local team        = player:GetTeamIndex()

    if team == local_team then
      dota.DrawUtils.DrawString(5, 400 + offset_counter  * 30, 255,255,255,255, false, level);
      dota.DrawUtils.DrawString(25, 400 + offset_counter * 30, 255,255,255,255, false, player_name);
      dota.DrawUtils.DrawString(5, 400 + offset_counter  * 30 + 15, 255,255,255,255, false, gold);
      dota.DrawUtils.DrawString(60, 400 + offset_counter  * 30 + 15, 255,255,255,255, false, last_hits);
      
      offset_counter = offset_counter + 1
    end
  end
end

function Initialize()
  hook = dota.AddPaintHook(OnPaint)
end