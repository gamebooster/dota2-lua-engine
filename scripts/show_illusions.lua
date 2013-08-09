local illusions = {}
local effects   = {}
local hook = nil

function AddToSet(set, key)
    set[key] = true
end

function RemoveFromSet(set, key)
    set[key] = nil
end

function SetContains(set, key)
    return set[key] ~= nil
end

function OnPaint()
  local heroes = dota.FindEntities(dota.kDotaBaseNPCHero)
  local local_player = dota.GetLocalPlayer()

  for _,hero in ipairs(heroes) do
    if hero:IsIllusion() then
      local handle = hero:GetRefEHandle():ToInt()
      if SetContains(illusions, handle) == false then
        local effect = dota.CreateEntityParticle(hero, "ghost");
	      effect:SetControlPoint(1, dota.Vector(200, 0, 0))
        table.insert(effects, effect)
        AddToSet(illusions, handle)
      end
    end
  end
end

function Initialize()
  hook = dota.AddPaintHook(OnPaint)
end