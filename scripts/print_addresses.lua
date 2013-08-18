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

  for i = 0, count - 1 do
    local buff = odifier_manager:GetBuffByIndex(i)
    dota.DrawUtils.DrawString(5, 200 + i * 30, 255,255,255,255, false, buff:GetName());
    dota.DrawUtils.DrawString(5, 205 + i * 30, 255,255,255,255, false, buff:GetTexture());
  end
  
  for i = 0, 16 do
    local ability = local_hero:GetAbilityByDisplayedIndex(i)
    if ability ~= nil then
      dota.DrawUtils.DrawString(5, 400 + i * 30, 255,255,255,255, false, ability:CanBeExecuted());
    end
  end
end

function Initialize()
  hook = dota.AddPaintHook(OnPaint)
end