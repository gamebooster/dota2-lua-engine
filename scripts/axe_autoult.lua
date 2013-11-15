local hook = nil
local sleep = 0
local local_hero = dota.GetLocalHero()
local local_player = dota.GetLocalPlayer()
local font = dota.DrawUtils.GetInstance():CreateFont("Arial", 18, 500)
 
function OnPaint()
  if sleep > 0 then
    sleep = sleep - 1
    return
  end

  local ult_ability = local_hero:GetAbilityByDisplayedIndex(3)
  local threshold = ult_ability:GetLevelSpecialValueFor("kill_threshold")
  dota.DrawUtils.DrawString(font, 5, 300, 255,255,255, false, "Ultimate: " .. ult_ability:CanBeExecuted() .. " " .. threshold)
  if threshold == 0 or ult_ability:CanBeExecuted() ~= 1 then
    return
  end

  local players = dota.FindEntities(dota.kDotaPlayer)
  
  for _,player in ipairs(players) do
    local hero = player:GetAssignedHero()
    if hero == nil then
      return
    end
    
    local health = hero:GetHealth()
    if hero:GetTeamIndex() ~= local_hero:GetTeamIndex() and
       health <= threshold and health > 20 and
       local_hero:IsEntityInRange(hero, ult_ability:GetCastRange()) then
      local_player:UseAbilityEntity(ult_ability, hero)
      sleep = 10
    end
  end
end

function Initialize()
  hook = dota.AddPaintHook(OnPaint)
end