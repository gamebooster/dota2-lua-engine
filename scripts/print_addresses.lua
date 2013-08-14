function PrintAddress(userdata)
  log.Warning(string.format("0x%x", log.GetAddress(userdata)) .. " \n")
end

local move_command;
local stop_command;

local hook = nil
local modifier_manager = dota.GetLocalHero():GetModifierManager()

function BuffExist(name)
  local count = modifier_manager:GetCurrentBuffCount()

  for i = 0, count - 1 do
    if modifier_manager:GetBuffByIndex(i):GetName() == name then
      return true
    end
  end
  return false
end

function OnPaint()
  local count = modifier_manager:GetCurrentBuffCount()

  for i = 0, 16 do
    local ability = dota.GetLocalHero():GetAbilityByDisplayedIndex(i)
    if ability ~= nil then
      dota.DrawUtils.DrawString(5, 600 + i * 30, 255,255,255,255, false,
        ability:GetAbilityType() .. " " .. tostring(ability:InAbilityPhase()));
    end
  end

  for i = 0, count - 1 do
    --dota.DrawUtils.DrawString(5, 400 + i * 30, 255,255,255,255, false, modifier_manager:GetBuffByIndex(i):GetName());
  end
  
  
  local vec = dota.Input.GetInstance():Get3dPositionUnderCursor()
  dota.DrawUtils.DrawString(5, 300, 255,255,255,255, false, vec.x .. " " .. vec.y .. " " .. vec.z);
  
  local entity = dota.Input.GetInstance():GetEntityUnderCursor()
  if entity ~= nil then
    dota.DrawUtils.DrawString(5, 400, 255,255,255,255, false, entity:GetClientClass():GetName());
  end
  if BuffExist("modifier_pugna_life_drain") then
    dota.ExecuteCommand("dota_item_execute 0;dota_item_execute 0;")
  end
end

function MoveCommand()
  dota.GetLocalPlayer():Move(dota.Vector(0,0,0))
end

function StopCommand()
  dota.GetLocalPlayer():Stop()
end

function Initialize()
  hook = dota.AddPaintHook(OnPaint)
  move_command = dota.AddConsoleCommand("move_player", MoveCommand);
  stop_command = dota.AddConsoleCommand("stop_player", StopCommand);
end