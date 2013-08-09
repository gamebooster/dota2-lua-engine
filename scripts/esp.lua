local effect = nil
local hook = nil
local draw_utils = dota.DrawUtils.GetInstance()

function OnPaint()
  local players = dota.FindEntities(dota.kDotaPlayer)
  local local_player = dota.GetLocalPlayer()

  for _,player in ipairs(players) do
    local hero = player:GetAssignedHero()

    local health = hero:GetHealth()
    local mana = math.floor(hero:GetMana())
    local max_mana = math.floor(hero:GetMaxMana())
    local bar_width = 100
    local life_width = (mana * 100 / max_mana)
    
    local origin = hero:GetAbsOrigin()
    local screen = dota.DrawUtils.GetVectorInScreenSpace(origin)
    if screen.x > 0 then
      draw_utils:DrawRect(screen.x - 45, screen.y - 45, bar_width, 10, 0, 0, 0, 255)
      draw_utils:DrawRect(screen.x - 45, screen.y - 45, life_width,10, 0, 128, 255, 255)

      --draw_utils:OutlineRect(screen.x - 46, screen.y - 46, bar_width + 2, 12, 0, 0, 0, 255)

      dota.DrawUtils.DrawString(screen.x - 45 + (bar_width/2), screen.y -  48, 255, 255, 255, 255, true, mana .. " / " .. max_mana)
      dota.DrawUtils.DrawString(screen.x - 45 + (bar_width/2), screen.y - 100, 255, 255, 255, 255, true, health)
    end  
  end
end

function Initialize()
  hook = dota.AddPaintHook(OnPaint)
end