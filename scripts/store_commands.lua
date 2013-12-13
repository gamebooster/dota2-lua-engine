local announcer_command
local announcer_killstreak_command
local hud_command

function OnAnnouncerCommand(item_id)
  if item_id == 0 then
    log.Warning("Usage: dota_set_announcer <item_id>\n")
    return
  end

  local local_player = dota.GetLocalPlayer()
  if (local_player == nil) then
    log.Warning("local_player is nil")
    return
  end
  
  local item = dota.EconItemView:Create()
  item:Init(item_id, 9999, 9999, 0)
  
  local team_index = local_player:GetTeamIndex()
  local gamerules = dota.GameRules.GetInstance()
  if (gamerules == nil) then 
    log.Warning("gamerules is nil")
    return
  end
  
  local announcer = gamerules:GetAnnouncer(team_index)
  if (announcer == nil) then
    log.Warning("announcer is nil")
    return
  end
  announcer:SetAnnouncerItem(item, 1)
end

function OnAnnouncerKillstreakCommand(item_id)
  if item_id == 0 then
    log.Warning("Usage: dota_announcer_killstreak_set <item_id>\n")
    return
  end

  local local_player = dota.GetLocalPlayer()
  if (local_player == nil) then
    log.Warning("local_player is nil")
    return
  end
  
  local item = dota.EconItemView:Create()
  item:Init(item_id, 9999, 9999, 0)
  
  local team_index = local_player:GetTeamIndex()
  local gamerules = dota.GameRules.GetInstance()
  if (gamerules == nil) then 
    log.Warning("gamerules is nil")
    return
  end
  
  local announcer = gamerules:GetKillingSpreeAnnouncer(team_index)
  if (announcer == nil) then
    log.Warning("announcer is nil")
    return
  end
  announcer:SetAnnouncerItem(item, 1)
end

function OnHudCommand(item_id)
  if item_id == 0 then
    log.Warning("Usage: dota_hud_set <item_id>\n")
    return
  end

  local item = dota.EconItemView:Create()
  item:Init(item_id, 9999, 9999, 0)
  
    dota.SFActionPanel.GetInstance():LoadHUDSkin(item, 1)
end

function Initialize()
  announcer_command = dota.AddConsoleCommand("dota_announcer_set", OnAnnouncerCommand)
  announcer_killstreak_command = dota.AddConsoleCommand("dota_announcer_killstreak_set", OnAnnouncerKillstreakCommand)
  hud_command = dota.AddConsoleCommand("dota_hud_set", OnHudCommand)
end