local effect = nil

function OnPaint()
  local local_hero = dota.GetLocalHero()
  if local_hero:IsVisibleByEnemyTeam() then
    if effect == nil then
      effect = dota.CreateEntityParticle(local_hero, "rune_haste")
    end
  else
    if effect ~= nil then
      effect = nil
    end
  end
end

function Initialize()
  dota.AddPaintHook(OnPaint);
end
