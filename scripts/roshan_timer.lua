local on_roshan_hook
local kill_time = 0
local hook

function OnRoshanKill()
  kill_time = dota.GameRules.GetInstance():GetGameTime()
  dota.SFHudOverlay.GetInstance():SendRoshanPopup(false, kill_time + 600)
end

function OnPaint()
  if kill_time == 0 then
    return
  end
  local current_time = dota.GameRules.GetInstance():GetGameTime()
  local difference = math.floor(current_time - kill_time)
  
  if difference == (60 * 8) then
    dota.SFHudOverlay.GetInstance():SendRoshanPopup(false, kill_time + 600)
    kill_time = kill_time - 1
  elseif difference == (60 * 9) + 30 then
    dota.SFHudOverlay.GetInstance():SendRoshanPopup(false, kill_time + 600)
    kill_time = kill_time - 1
  elseif difference >= (60 * 10) then
    dota.SFHudOverlay.GetInstance():SendRoshanPopup(false, kill_time + 600)
    kill_time = 0
  end
end

function Initialize()
  on_roshan_hook = dota.AddEventListener("dota_roshan_kill", OnRoshanKill)
  hook = dota.AddPaintHook(OnPaint)
end