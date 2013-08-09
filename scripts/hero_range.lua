local add_command;
local reset_command;
local effects = {}

function OnAddCommand(range)
  local local_hero = dota.GetLocalHero()
  if local_hero == nil then
    return
  end
  local effect = dota.CreateEntityParticle(local_hero, "range_display");
	effect:SetControlPoint(1, dota.Vector(range, 0, 0))
  table.insert(effects, effect)
end

function OnResetCommand(range)
  effects = {}
  collectgarbage()
end

function Initialize()
  add_command = dota.AddConsoleCommand("dota_add_range_display", OnAddCommand);
  reset_command = dota.AddConsoleCommand("dota_reset_range_display", OnResetCommand);
end