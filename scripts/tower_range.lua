-- Config
ownRange = false
enemyRange = true
towerRange = 850

-- Code

local tower_kill_hook
local game_start_hook
local game_end_hook
local effects = {}

function OnTowerKill()
  FetchTowers()
end

function OnGameStart()
  FetchTowers()
end

function FetchTowers()
  effects = {}
  collectgarbage()
  
  local towers = dota.FindEntitiesWithName("CDOTA_BaseNPC_Tower")
  local local_player = dota.GetLocalPlayer()

  for _,v in ipairs(towers) do
    if (ownRange and enemyRange) or
      (ownRange and v:GetTeamIndex() == local_player:GetTeamIndex()) or
      (enemyRange and v:GetTeamIndex() ~= local_player:GetTeamIndex()) then
      local effect = dota.CreateEntityParticle(v, "range_display");
	    effect:SetControlPoint(1, dota.Vector(towerRange,0,0) )
	    table.insert(effects, effect)
    end
  end
end

function Initialize()
  tower_kill_hook = dota.AddEventListener("dota_tower_kill", OnTowerKill)
  game_start_hook = dota.AddEventListener("dota_player_pick_hero", OnGameStart)
  
  FetchTowers()
end
