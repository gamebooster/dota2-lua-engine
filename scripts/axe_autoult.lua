local hook = nil
local sleep = 0
 
function OnPaint()
  if sleep > 0 then
    sleep = sleep - 1
    return
  end
  local players = dota.FindEntities(dota.kDotaPlayer)
  local local_player = dota.GetLocalPlayer()
  local local_hero = dota.GetLocalHero()
  local ult_ability = local_hero:GetAbilityByDisplayedIndex(3)
  local threshold = ult_ability:GetLevelSpecialValueFor("kill_threshold")
  if threshold == 0 then
    return
  end

  for _,player in ipairs(players) do
    local hero = player:GetAssignedHero()
    if hero == nil then
      return
    end
    
    local health = hero:GetHealth()
    if hero:GetTeamIndex() ~= local_hero:GetTeamIndex() and
       health <= threshold and health > 10 and
       ult_ability:CanBeExecuted() == -1 and
       local_hero:IsEntityInRange(hero, ult_ability:GetCastRange()) then
      local_player:UseAbilityEntity(ult_ability, hero)
      sleep = 50
    end
  end
end

function Initialize()
  hook = dota.AddPaintHook(OnPaint)
end