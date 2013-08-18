function PrintAddress(userdata)
  return string.format("0x%x", log.GetAddress(userdata))
end

function GetDistance2D(a,b)
  return math.sqrt(math.pow(a.x-b.x,2)+math.pow(a.y-b.y,2))
end

local hook = nil
local projectile_manager = dota.ProjectileManager.GetInstance()
local sleep = 0
local ability = dota.GetLocalHero():GetAbilityByDisplayedIndex(2)
local index = dota.GetLocalHero():GetIndex()

function OnPaint()
  if sleep > 0 then
    sleep = sleep - 1
    return
  end
  
  for i = 0, 30 do
    local projectile = projectile_manager:GetProjectileByIndex(i)
    if projectile ~= nil and projectile:GetTarget():GetIndex() == index then
      -- local target_vector = projectile.targets
      -- local position_vector = projectile.position
      -- local ability = dota.GetLocalHero():GetAbilityByDisplayedIndex(2)
      if ability:CanBeExecuted() == -1 then
        dota.ExecuteCommand("dota_select_all;")
        dota.GetLocalPlayer():UseAbility(ability)--dota.ExecuteCommand("dota_select_all;dota_ability_execute 2;")
        sleep = 50
        return
      end
      -- dota.DrawUtils.DrawString(5, 400 + i * 30, 255,255,255,255, false,
        -- target_vector.x .. " " .. target_vector.y .. " " .. target_vector.z);
      -- dota.DrawUtils.DrawString(5, 420 + i * 30, 255,255,255,255, false,
        -- position_vector.x .. " " .. position_vector.y .. " " .. position_vector.z);
      -- dota.DrawUtils.DrawString(5, 440 + i * 30, 255,255,255,255, false,
        -- GetDistance2D(target_vector, position_vector));
      -- dota.DrawUtils.DrawString(5, 460 + i * 30, 255,255,255,255, false,
        -- projectile.entity:GetName());
      -- dota.DrawUtils.DrawString(5, 480 + i * 30, 255,255,255,255, false,
        -- projectile:GetTarget():GetClientClass():GetName());
    end
  end
end

function Initialize()
  hook = dota.AddPaintHook(OnPaint)
end