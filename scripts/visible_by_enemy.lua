local effect = nil

function OnPaint()
  CheckVisbleByEnemyTeam()
end

function CheckVisbleByEnemyTeam()
  local local_hero = dota.GetLocalHero()
  if local_hero:IsVisibleByEnemyTeam() and effect == nil then
    effect = dota.CreateEntityParticle(local_hero, "rune_haste")
  else
    effect = nil
  end
end

function Initialize()
  dota.AddPaintHook(OnPaint);
end