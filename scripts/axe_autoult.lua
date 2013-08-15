local hook = nil

function OnPaint()
  local players = dota.FindEntities(dota.kDotaPlayer)
  local local_player = dota.GetLocalPlayer()
  local local_hero = dota.GetLocalHero()
  local ult_ability = local_hero:GetAbilityByDisplayedIndex(4)

  for _,player in ipairs(players) do
    local hero = player:GetAssignedHero()
    if hero == nil then
      return
    end
    
    local health hero:GetHealth()
    if hero:GetTeamIndex() ~= local_hero:GetTeamIndex() and
       health < 351 and health > 10 and
       local_hero:IsEntityInRange(hero, ult_ability:GetCastRange()) then
      local_player:UseAbilityEntity(ult_ability, hero)
    end
  end
end

function Initialize()
  hook = dota.AddPaintHook(OnPaint)
end