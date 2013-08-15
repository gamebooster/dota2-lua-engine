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
    dota.DrawUtils.DrawString(5, 400 + i * 30, 255,255,255,255, false, modifier_manager:GetBuffByIndex(i):GetName());
  end
  
  for i = 0, 16 do
    local ability = local_hero:GetAbilityByDisplayedIndex(i)
    if ability ~= nil then
      dota.DrawUtils.DrawString(5, 400 + i * 30, 255,255,255,255, false, ability:CanBeExecuted());
    end
  end
end

function MoveCommand()
  return
  dota.GetLocalPlayer():UseAbility(dota.GetLocalHero():GetAbilityByDisplayedIndex(1))
  return
  local players = dota.FindEntities(dota.kDotaPlayer)
  local local_player = dota.GetLocalPlayer()
  local local_hero = dota.GetLocalHero()

  for _,player in ipairs(players) do
    local hero = player:GetAssignedHero()
    if hero == nil then
      return
    end
    
    if hero:GetTeamIndex() ~= local_hero:GetTeamIndex() and local_hero:IsEntityInRange(hero, dota.GetLocalHero():GetAbilityByDisplayedIndex(1):GetCastRange()) then
      local_player:UseAbilityEntity(dota.GetLocalHero():GetAbilityByDisplayedIndex(4), hero)
    end
  end
end

function StopCommand()
  dota.GetLocalPlayer():Stop()
end

function Initialize()
  hook = dota.AddPaintHook(OnPaint)
  move_command = dota.AddConsoleCommand("move_player", MoveCommand);
  stop_command = dota.AddConsoleCommand("stop_player", StopCommand);
end