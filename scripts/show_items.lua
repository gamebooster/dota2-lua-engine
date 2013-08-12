local items = {}
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
  local players = dota.FindEntities(dota.kDotaPlayer)
  local local_player = dota.GetLocalPlayer()
  local local_hero = dota.GetLocalHero()

  for _,player in ipairs(players) do
    local hero = player:GetAssignedHero()
    if hero == nil then
      return
    end
    
    if hero:GetTeamIndex() ~= local_hero:GetTeamIndex() then
      local inventory = hero:GetInventory()
      for i = 0, 6, 1 do
        local item = inventory:GetItemInSlot(i)
        if item ~= nil then
        
          local item_handle = item:GetRefEHandle():ToInt()

          if SetContains(items, item_handle) == false then
            local hero_name = dota.PlayerResource.GetPlayerSelectedHero(player:GetPlayerId())
            hero_name = string.gsub(hero_name, "npc_dota_hero_", "")
            local item_name = item:GetName()
            item_name = string.gsub(item_name, "item_", "")

            dota.SFHudOverlay.GetInstance():ShowSpecItemPickup(hero_name, item_name)

            AddToSet(items, item_handle)
          end
        end
      end
    end
  end
end

function Initialize()
  hook = dota.AddPaintHook(OnPaint)
end