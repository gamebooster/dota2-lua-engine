local hook = nil
local sleep = 0

function OnPaint()
  if sleep > 0 then
    sleep = sleep - 1
    return
  end

  local creeps = dota.FindEntitiesWithName("CDOTA_BaseNPC_Creep_Lane")
  local local_player = dota.GetLocalPlayer()
  local local_hero = dota.GetLocalHero()

  for _,creep in ipairs(creeps) do
    if local_hero:IsEntityLastHittable(creep) and local_hero:IsEntityInRange(creep, local_hero:GetAttackRange()) then
      local_player:PrepareUnitOrders(4, creep:GetIndex());
      sleep = 100
    end
  end
end

function Initialize()
  hook = dota.AddPaintHook(OnPaint)
end