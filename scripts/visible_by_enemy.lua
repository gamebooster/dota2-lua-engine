local effect = nil
local hook = nil

function OnPaint()
  local local_hero = dota.GetLocalHero()
  if local_hero:IsVisibleByEnemyTeam() then
    if effect == nil then
      effect = dota.CreateEntityParticle(local_hero, "rune_haste")
    end
  else
    if effect ~= nil then
      effect = nil
      collectgarbage()
    end
  end
end

function Initialize()
  hook = dota.AddPaintHook(OnPaint)
end
