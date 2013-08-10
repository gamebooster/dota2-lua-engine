#include "precompiled_headers.h"

#include "lua_global.hpp"

#include <functional>
#include <algorithm>

#include "..\source-sdk\SDK.h"
#include "..\source-sdk\global_instance_manager.h"

#include "..\dota\DotaGlobal.h"
#include "..\dota\DotaConstants.h"
#include "..\dota\DotaParticleManager.hpp"
#include "..\dota\DotaPlayerResource.h"

#include "..\dota\DotaUnits.hpp"
#include "..\utils\utils.h"
#include "..\utils\vmthooks.h"
#include "..\source-sdk\draw_utils.h"

#include "..\..\LuaBridge\RefCountedPtr.h"

#undef CreateEvent

namespace lua {

  class LuaGameEventHelper {
   public:
     LuaGameEventHelper(const char* name, luabridge::LuaRef callback) : listener_(callback) {
       listener_.ListenForGameEvent(name);
     }
  private:
    CLuaGameEventListener listener_;
  };

  class HookListener {
   public:
    virtual void NewEvent() = 0;
  };
  class HookManager {
   public:
     HookManager(void* object, int index) : hook_(object, index) {

     }
    void AddListener(HookListener* listener) {
      callbacks_.push_back(listener);
    }

    void RemoveListener(HookListener* listener) {
      std::list<HookListener*>::iterator it = std::find(callbacks_.begin(), callbacks_.end(), listener);
      if (it != callbacks_.end()) {
        callbacks_.erase(it);
      }
    }

    void CallListener() {
      for( auto listener : callbacks_) {
        listener->NewEvent();
      }
    }

    virtual ~HookManager() {
    }
    protected:
     utils::VtableHook hook_;
     std::list<HookListener*> callbacks_;
  };
  class PaintHookManager : public HookManager {
   public:
    static PaintHookManager& GetInstance() {
       static PaintHookManager instance;
       return instance;
    }
   private:
     PaintHookManager() : HookManager(dota::Hud::GetInstance()->FindElement("CHudHealthBars") , 0x34) {
       hook_.HookMethod(CHudHealthBars_Paint, 107);
     }

     static void __fastcall CHudHealthBars_Paint(void* thisptr, int edx, void* guipaintsurface) {
       typedef void ( __thiscall* OriginalFunction )(PVOID, PVOID);
       static PaintHookManager& hook_manager = PaintHookManager::GetInstance();
       hook_manager.hook_.GetMethod<OriginalFunction>(107)(thisptr, guipaintsurface);
       
       hook_manager.CallListener();
     }

  };
  class PaintHookListener : public HookListener {
  public:
    PaintHookListener(luabridge::LuaRef callback) : callback_(callback) {
      PaintHookManager::GetInstance().AddListener(this);
    }
    ~PaintHookListener() {
      PaintHookManager::GetInstance().RemoveListener(this);
    }
    void NewEvent() {
      try {
        callback_();
      }
      catch (luabridge::LuaException const& e) {
        Warning("booster-lua: %s \n", e.what());
      }
    }
  private:
    luabridge::LuaRef callback_;
  };

  static int kDotaPlayer = 0;
  static int kDotaBaseNPC = 1;
  static int kDotaBaseNPCHero = 2;

  class LuaDotaPlayer : public dota::DotaPlayer {
  public:
    dota::BaseNPCHero* LuaGetAssignedHero() {
      return (dota::BaseNPCHero*)GlobalInstanceManager::GetClientEntityList()->GetClientEntity(this->GetAssignedHero()); 
    }
  };

  static luabridge::LuaRef FindEntities(int type,  lua_State* L) {
    luabridge::LuaRef table = luabridge::newTable(L);
    int count = 1;
    for (int i = 1; i < GlobalInstanceManager::GetClientEntityList()->GetHighestEntityIndex(); i++ ) {
      dota::BaseEntity *base_entity = GlobalInstanceManager::GetClientEntityList()->GetClientEntity(i);
      if (base_entity == nullptr) continue;

      const char* class_name = base_entity->GetClientClass()->GetName();
      if (class_name == nullptr) continue;


      if (type == kDotaPlayer) {
        if (StringHasPrefix(class_name, "CDOTAPlayer")) {
          LuaDotaPlayer* player = reinterpret_cast<LuaDotaPlayer*>(base_entity);
          table[count] = player;
          count++;
        }
      } else if (type == kDotaBaseNPC) {
        if (StringHasPrefix(class_name, "CDOTA_BaseNPC")) {
          dota::BaseNPC* tower = reinterpret_cast<dota::BaseNPC*>(base_entity);
          table[count] = tower;
          count++;
        }
      } else if (type == kDotaBaseNPCHero) {
        if (StringHasPrefix(class_name, "CDOTA_Unit_Hero")) {
          dota::BaseNPCHero* hero = reinterpret_cast<dota::BaseNPCHero*>(base_entity);
          table[count] = hero;
          count++;
        }
      }
    }
    return table;
  }
  static luabridge::LuaRef FindEntitiesWithName(const char* name,  lua_State* L) {
    luabridge::LuaRef table = luabridge::newTable(L);
    int count = 1;
    for (int i = 1; i < GlobalInstanceManager::GetClientEntityList()->GetHighestEntityIndex(); i++ ) {
      dota::BaseEntity *base_entity = GlobalInstanceManager::GetClientEntityList()->GetClientEntity(i);
      if (base_entity == nullptr) continue;

      const char* class_name = base_entity->GetClientClass()->GetName();
      if (class_name == nullptr) continue;

      if (StringHasPrefix(class_name, name)) {
        if (StringHasPrefix(class_name, "CDOTA_BaseNPC")) {
          table[count] = (dota::BaseNPC*) base_entity;
          count++;
        } else {
          table[count] = base_entity;
          count++;
        }
      }
    }
    return table;
  }
  static LuaDotaPlayer* GetLocalPlayer() {
    return reinterpret_cast<LuaDotaPlayer*>(GlobalInstanceManager::GetClientTools()->GetLocalPlayer());
  }
  static dota::BaseNPCHero* GetLocalHero() {
    LuaDotaPlayer* local_player = (LuaDotaPlayer*)GlobalInstanceManager::GetClientTools()->GetLocalPlayer();
    if (local_player == nullptr) return nullptr;

    return reinterpret_cast<dota::BaseNPCHero*>(GlobalInstanceManager::GetClientEntityList()->GetClientEntity(local_player->GetAssignedHero()));
  }
  class LuaParticle {
   public:
    LuaParticle(dota::BaseEntity* entity, const char* name) {
      particle_index = dota::ParticleManager::GetInstance()->CreateParticle(name, 1, entity);
    }
    LuaParticle(Vector vector, const char* name) {
      particle_index = dota::ParticleManager::GetInstance()->CreateParticle(name, 0, nullptr);
      dota::ParticleManager::GetInstance()->SetParticleControl(particle_index, 0, vector);
    }
    ~LuaParticle() {
      dota::ParticleManager::GetInstance()->DestroyParticle(particle_index, 1);
      dota::ParticleManager::GetInstance()->ReleaseParticleIndex(particle_index);
    }

    void SetControlPoint(int index, Vector vector) {
      dota::ParticleManager::GetInstance()->SetParticleControl(particle_index, index, vector);
    }
   private:
    int particle_index;
  };
  static RefCountedPtr<LuaParticle> CreateEntityParticle(dota::BaseEntity* entity, const char* name) {
    return new LuaParticle(entity, name);
  }
  static RefCountedPtr<LuaParticle> CreateParticle(Vector vector, const char* name) {
    return new LuaParticle(vector, name);
  } 

  class ConsoleCommand {
  public:
    ConsoleCommand(const char* name, luabridge::LuaRef callback) : command_(name, ConsoleCallback, "dynamic lua command") {
      ConsoleCommand::callbacks_.insert(std::pair<std::string, luabridge::LuaRef>(command_.GetName(), callback));
      GlobalInstanceManager::GetCVar()->RegisterConCommand(&command_);
    }
    ~ConsoleCommand() {
      GlobalInstanceManager::GetCVar()->UnregisterConCommand(&command_);
      ConsoleCommand::callbacks_.erase(command_.GetName());
    }
  private: 
    static void ConsoleCallback(const CCommandContext &context, const CCommand &args) {
      try {
        luabridge::LuaRef func = ConsoleCommand::callbacks_.at(args[0]);
        func(args[1]);
      }
      catch (luabridge::LuaException const& e) {
        Warning("booster-lua: ConsoleCallbackError %s \n", e.what());
      }
      catch (const std::out_of_range& oor) {
        Warning("booster-lua: ConsoleCallbackError %s \n", oor.what());
      }
      catch (...) {
        Warning("booster-lua: ConsoleCallbackError \n");
      }
    }

    ConCommand command_;
    static std::map<std::string, luabridge::LuaRef> callbacks_;
  };

  std::map<std::string, luabridge::LuaRef> ConsoleCommand::callbacks_;

  static void DrawString(int x, int y, int r, int g, int b, int a, bool center, const char* text) {
    sourcesdk::DrawUtils::GetInstance().DrawString(x, y, r, g, b, a, center, text);
  }
  static Vector GetVectorInScreenSpace(Vector vec) {
    int x, y;
    if (sourcesdk::DrawUtils::GetInstance().GetVectorInScreenSpace(vec, x, y)) {
      return Vector(x, y, 0);
    } else {
      return vec3_origin;
    }
  }


  typedef void(*MsgSignature)(const tchar* msg, ...); 
  template<MsgSignature T>
  int  LogWrapper(lua_State *L) {
    lua_gettop(L);
    T(lua_tostring(L, 1));
    return 0;
  }
  void RegisterGlobalFunctions(LuaState& state) {
    luabridge::getGlobalNamespace (state)
      .beginNamespace("log")
        .addCFunction("Msg", LogWrapper<Msg>)
        .addCFunction("Warning", LogWrapper<Warning>)
        .addCFunction("Error", LogWrapper<Error>)
      .endNamespace();

    luabridge::getGlobalNamespace(state)
      .beginNamespace("dota")
        .beginClass<LuaGameEventHelper>("AddEventListener")
         .addConstructor<void (*) (const char*, luabridge::LuaRef)> ()
        .endClass()
      .endNamespace();

    luabridge::getGlobalNamespace(state)
      .beginNamespace("dota")
        .beginClass<PaintHookListener>("AddPaintHook")
         .addConstructor<void (*) (luabridge::LuaRef)> ()
        .endClass()
      .endNamespace();

    luabridge::getGlobalNamespace(state)
      .beginNamespace("dota")
        .beginClass<LuaParticle>("LuaParticle")
         .addFunction("SetControlPoint", &LuaParticle::SetControlPoint)
        .endClass()
      .endNamespace();

    luabridge::getGlobalNamespace(state)
      .beginNamespace("dota")
        .beginClass<ConsoleCommand>("AddConsoleCommand")
          .addConstructor<void (*) (const char*, luabridge::LuaRef)> ()
        .endClass()
      .endNamespace();

    luabridge::getGlobalNamespace(state)
      .beginNamespace("dota")
        .addFunction("FindEntities", &FindEntities)
        .addFunction("FindEntitiesWithName", &FindEntitiesWithName)
        .addFunction("GetLocalPlayer", &GetLocalPlayer)
        .addFunction("GetLocalHero", &GetLocalHero)
        .addFunction("CreateParticle", &CreateParticle)
        .addFunction("CreateEntityParticle", &CreateEntityParticle)
        .addVariable("kDotaPlayer", &kDotaPlayer, false)
        .addVariable("kDotaBaseNPC", &kDotaBaseNPC, false)
        .addVariable("kDotaBaseNPCHero", &kDotaBaseNPCHero, false)
      .endNamespace();

    luabridge::getGlobalNamespace(state)
      .beginNamespace("dota")
        .beginClass<sourcesdk::DrawUtils>("DrawUtils")
          .addStaticFunction("GetInstance", &sourcesdk::DrawUtils::GetInstance)
          .addFunction("DrawRect", &sourcesdk::DrawUtils::DrawRect)
          .addStaticFunction("GetVectorInScreenSpace", &GetVectorInScreenSpace)
          .addFunction("OutlineRect", &sourcesdk::DrawUtils::OutlineRect)
          .addStaticFunction("DrawString", &DrawString)
        .endClass()
      .endNamespace();

    luabridge::getGlobalNamespace(state)
      .beginNamespace("dota")
        .beginClass<dota::DotaPlayerResource>("PlayerResource")
          .addStaticFunction("GetPlayerResource", &dota::DotaPlayerResource::GetPlayerResource)
          .addStaticFunction("GetPlayerSelectedHero", &dota::DotaPlayerResource::GetPlayerSelectedHero)
          .addFunction("GetPlayerName", &dota::DotaPlayerResource::GetPlayerName)
          .addFunction("GetLevel", &dota::DotaPlayerResource::GetLevel)
          .addFunction("GetUnreliableGold", &dota::DotaPlayerResource::GetUnreliableGold)
          .addFunction("GetLastHitCount", &dota::DotaPlayerResource::GetLastHitCount)
          .addFunction("GetTeam", &dota::DotaPlayerResource::GetTeam)
          .addFunction("GetReliableGold", &dota::DotaPlayerResource::GetReliableGold)
        .endClass()
      .endNamespace();

    luabridge::getGlobalNamespace(state)
      .beginNamespace("dota")
        .beginClass<dota::EconItemView>("EconItemView")
          .addStaticFunction("Create", &dota::EconItemView::Create)
          .addFunction("Init", &dota::EconItemView::Init)
        .endClass()
      .endNamespace();

    luabridge::getGlobalNamespace(state)
      .beginNamespace("dota")
        .beginClass<dota::UnitAnnouncer>("UnitAnnouncer")
          .addFunction("SetAnnouncerItem", &dota::UnitAnnouncer::SetAnnouncerItem)
        .endClass()
      .endNamespace();

    luabridge::getGlobalNamespace(state)
      .beginNamespace("dota")
        .beginClass<dota::Hud>("Hud")
          .addStaticFunction("SelectHudSkin", &dota::Hud::SelectHudSkin)
        .endClass()
      .endNamespace();

    luabridge::getGlobalNamespace(state)
      .beginNamespace("dota")
        .beginClass<dota::GameRules>("GameRules")
          .addStaticFunction("GetInstance", &dota::GameRules::GetInstance)
          .addFunction("GetAnnouncer", &dota::GameRules::GetAnnouncer)
          .addFunction("GetGameTime", &dota::GameRules::GetGameTime)
          .addFunction("GetKillingSpreeAnnouncer", &dota::GameRules::GetKillingSpreeAnnouncer)
        .endClass()
      .endNamespace();

    luabridge::getGlobalNamespace(state)
      .beginNamespace("dota")
        .beginClass<dota::DotaSFHudOverlay>("SFHudOverlay")
          .addStaticFunction("GetInstance", &dota::DotaSFHudOverlay::GetInstance)
          .addFunction("ShowSpecItemPickup", &dota::DotaSFHudOverlay::ShowSpecItemPickup)
          .addFunction("SendRoshanPopup", &dota::DotaSFHudOverlay::SendRoshanPopup)
        .endClass()
      .endNamespace();

    luabridge::getGlobalNamespace(state)
      .beginNamespace("dota")
        .beginClass<GlobalInstanceManager>("GlobalInstanceManager")
         .addStaticFunction("GetClient", &GlobalInstanceManager::GetClient)
         .addStaticFunction("GetClientEntityList", &GlobalInstanceManager::GetClientEntityList)
         .addStaticFunction("GetEngineClient", &GlobalInstanceManager::GetEngineClient)
         .addStaticFunction("GetSurface", &GlobalInstanceManager::GetSurface)
         .addStaticFunction("GetSurfaceNew", &GlobalInstanceManager::GetSurfaceNew)
         .addStaticFunction("GetPanel", &GlobalInstanceManager::GetPanel)
         .addStaticFunction("GetClientTools", &GlobalInstanceManager::GetClientTools)
         .addStaticFunction("GetCVar", &GlobalInstanceManager::GetCVar)
         .addStaticFunction("GetVguiInput", &GlobalInstanceManager::GetVguiInput)
         .addStaticFunction("GetLocalize", &GlobalInstanceManager::GetLocalize)
         .addStaticFunction("GetGameEventManager", &GlobalInstanceManager::GetGameEventManager)
        .endClass()
      .endNamespace();

    luabridge::getGlobalNamespace(state)
      .beginNamespace("dota")
        .beginClass<EngineClient>("EngineClient")
         .addFunction("Con_IsVisible", &EngineClient::Con_IsVisible)
         .addFunction("GetLocalPlayerIndex", &EngineClient::GetLocalPlayerIndex)
         .addFunction("Time", &EngineClient::Time)
         .addFunction("GetMaxClients", &EngineClient::GetMaxClients)
         .addFunction("IsInGame", &EngineClient::IsInGame)
         .addFunction("IsConnected", &EngineClient::IsConnected)
         .addFunction("IsDrawingLoadingImage", &EngineClient::IsDrawingLoadingImage)
         .addFunction("WorldToScreenMatrix", &EngineClient::WorldToScreenMatrix)
         .addFunction("IsTakingScreenshot", &EngineClient::IsTakingScreenshot)
         .addFunction("ClientCmd_Unrestricted", &EngineClient::ClientCmd_Unrestricted)
        .endClass()
      .endNamespace();

    luabridge::getGlobalNamespace(state)
      .beginNamespace("dota")
        .beginClass<ClientTools>("ClientTools")
         .addFunction("GetLocalPlayer", &ClientTools::GetLocalPlayer)
        .endClass()
      .endNamespace();

    luabridge::getGlobalNamespace(state)
      .beginNamespace("dota")
        .beginClass<Vgui_IInput>("Vgui_IInput")
         .addFunction("IsKeyDown", &Vgui_IInput::IsKeyDown)
        .endClass()
      .endNamespace();

    luabridge::getGlobalNamespace(state)
      .beginNamespace("dota")
        .beginClass<CGameEventListener>("CGameEventListener")
         .addFunction("ListenForGameEvent", &CGameEventListener::ListenForGameEvent)
         .addFunction("StopListeningForAllEvents", &CGameEventListener::StopListeningForAllEvents)
        .endClass()
      .endNamespace();

    luabridge::getGlobalNamespace(state)
      .beginNamespace("dota")
        .beginClass<IGameEvent>("IGameEvent")
         .addFunction("GetName", &IGameEvent::GetName)
         .addFunction("IsReliable", &IGameEvent::IsReliable)
         .addFunction("IsLocal", &IGameEvent::IsLocal)
         .addFunction("IsEmpty", &IGameEvent::IsEmpty)
         .addFunction("GetBool", &IGameEvent::GetBool)
         .addFunction("GetInt", &IGameEvent::GetInt)
         .addFunction("GetFloat", &IGameEvent::GetFloat)
         .addFunction("GetString", &IGameEvent::GetString)
         .addFunction("SetBool", &IGameEvent::SetBool)
         .addFunction("SetInt", &IGameEvent::SetInt)
         .addFunction("SetFloat", &IGameEvent::SetFloat)
         .addFunction("SetString", &IGameEvent::SetString)
        .endClass()
      .endNamespace();

    luabridge::getGlobalNamespace(state)
      .beginNamespace("dota")
        .beginClass<ILocalize>("ILocalize")
         .addFunction("Find", &ILocalize::Find)
        .endClass()
      .endNamespace();

    luabridge::getGlobalNamespace(state)
      .beginNamespace("dota")
        .beginClass<EntityList>("EntityList")
         .addFunction("GetClientEntity", &EntityList::GetClientEntity)
         .addFunction("GetClientEntityFromHandle", &EntityList::GetClientEntityFromHandle)
         .addFunction("GetHighestEntityIndex", &EntityList::GetHighestEntityIndex)
        .endClass()
      .endNamespace();

    luabridge::getGlobalNamespace(state)
      .beginNamespace("dota")
        .beginClass<dota::ParticleManager>("ParticleManager")
         .addStaticFunction("GetInstance", &dota::ParticleManager::GetInstance)
         .addFunction("CreateParticle", &dota::ParticleManager::CreateParticle)
         .addFunction("DestroyParticle", &dota::ParticleManager::DestroyParticle)
         .addStaticFunction("PrecacheParticleSystem", &dota::ParticleManager::PrecacheParticleSystem)
         .addFunction("ReleaseParticleIndex", &dota::ParticleManager::ReleaseParticleIndex)
         .addFunction("SetParticleControl", &dota::ParticleManager::SetParticleControl)
         .addFunction("SetParticleControlEnt", &dota::ParticleManager::SetParticleControlEnt)
        .endClass()
      .endNamespace();

    luabridge::getGlobalNamespace(state)
      .beginNamespace("dota")
        .beginClass<IGameEventManager2>("IGameEventManager2")
         .addFunction("LoadEventsFromFile", &IGameEventManager2::LoadEventsFromFile)
         .addFunction("Reset", &IGameEventManager2::Reset)
         .addFunction("AddListener", &IGameEventManager2::AddListener)
         .addFunction("FindListener", &IGameEventManager2::FindListener)
         .addFunction("CreateEvent", &IGameEventManager2::CreateEvent)
         .addFunction("FireEvent", &IGameEventManager2::FireEvent)
         .addFunction("FireEventClientSide", &IGameEventManager2::FireEventClientSide)
         .addFunction("DuplicateEvent", &IGameEventManager2::DuplicateEvent)
         .addFunction("FreeEvent", &IGameEventManager2::FreeEvent)
         .addFunction("SerializeEvent", &IGameEventManager2::SerializeEvent)
         .addFunction("UnserializeEvent", &IGameEventManager2::UnserializeEvent)
        .endClass()
      .endNamespace();

    luabridge::getGlobalNamespace(state)
      .beginNamespace("dota")
        .beginClass<dota::UnitInventory>("CUnitInventory")
         .addFunction("GetItemInSlot", &dota::UnitInventory::GetItemInSlot)
        .endClass()
     .endNamespace();

    luabridge::getGlobalNamespace(state)
      .beginNamespace("dota")
        .beginClass<Vector>("Vector")
         .addConstructor<void (*) (float, float, float)>()
         .addData("x", &Vector::x)
         .addData("y", &Vector::y)
         .addData("z", &Vector::z)
         .addFunction("IsValid", &Vector::IsValid)
         .addFunction("Invalidate", &Vector::Invalidate)
         .addFunction("Zero", &Vector::Zero)
         .addFunction("Length", &Vector::Length)
         .addFunction("DistTo", &Vector::DistTo)
        .endClass()
     .endNamespace();

    luabridge::getGlobalNamespace(state)
      .beginNamespace("dota")
        .beginClass<dota::CNewParticleEffect>("CNewParticleEffect")
         .addFunction("SetControlPoint", &dota::CNewParticleEffect::SetControlPoint)
        .endClass()
     .endNamespace();

    luabridge::getGlobalNamespace(state)
      .beginNamespace("dota")
        .beginClass<CBaseHandle>("CBaseHandle")
         .addFunction("ToInt", &CBaseHandle::ToInt)
        .endClass()
     .endNamespace();

    luabridge::getGlobalNamespace(state)
      .beginNamespace("dota")
        .beginClass<dota::BaseEntity>("BaseEntity")
         .addFunction("GetRefEHandle", &dota::BaseEntity::GetRefEHandle)
         .addFunction("GetClientClass", &dota::BaseEntity::GetClientClass)
         .addFunction("ComputeTranslucencyType", &dota::BaseEntity::ComputeTranslucencyType)
         .addFunction("IsAlive", &dota::BaseEntity::IsAlive)
         .addFunction("GetTeamIndex", &dota::BaseEntity::GetTeamIndex)
         .addFunction("GetHealth", &dota::BaseEntity::GetHealth)
         .addFunction("IsPlayer", &dota::BaseEntity::IsPlayer)
         .addFunction("GetAbsOrigin", &dota::BaseEntity::GetAbsOrigin)
         .addFunction("GetAbsAngles", &dota::BaseEntity::GetAbsAngles)
         .addFunction("GetIndex", &dota::BaseEntity::GetIndex)
        .endClass()
        .deriveClass <dota::BaseNPC, dota::BaseEntity> ("BaseNPC")
          .addFunction ("GetEffectiveInvisibilityLevel", &dota::BaseNPC::GetEffectiveInvisibilityLevel)
          .addFunction ("IsVisibleByEnemyTeam", &dota::BaseNPC::IsVisibleByEnemyTeam)
          .addFunction ("GetHealth", &dota::BaseNPC::GetHealth)
          .addFunction ("GetVisibilityLevel", &dota::BaseNPC::GetVisibilityLevel)
          .addFunction ("SetVisibilityLevel", &dota::BaseNPC::SetVisibilityLevel)
          .addFunction ("GetMana", &dota::BaseNPC::GetMana)
          .addFunction ("GetMaxMana", &dota::BaseNPC::GetMaxMana)
          .addFunction ("GetInventory", &dota::BaseNPC::GetInventory)
          .addFunction ("GetModifierManager", &dota::BaseNPC::GetModifierManager)
          .addFunction ("GetPhysicalArmor", &dota::BaseNPC::GetPhysicalArmor)
          .addFunction ("IsEntityLastHittable", &dota::BaseNPC::IsEntityLastHittable)
          .addFunction ("GetPhysicalArmorReduction", &dota::BaseNPC::GetPhysicalArmorReduction)
          .addFunction ("GetDamageMin", &dota::BaseNPC::GetDamageMin)
          .addFunction ("GetDamageMax", &dota::BaseNPC::GetDamageMax)
          .addFunction ("GetBonusDamage", &dota::BaseNPC::GetBonusDamage)
          .addFunction ("GetAttackRange", &dota::BaseNPC::GetAttackRange)
          .addFunction ("IsEntityInRange", &dota::BaseNPC::IsEntityInRange)
        .endClass ()
        .deriveClass <dota::BaseNPCHero, dota::BaseNPC> ("BaseNPCHero")
          .addFunction ("IsIllusion", &dota::BaseNPCHero::IsIllusion)
        .endClass ()
        .deriveClass <dota::DotaItem, dota::BaseEntity> ("DotaItem")
          .addFunction ("GetName", &dota::DotaItem::GetName)
          .addFunction ("GetItemId", &dota::DotaItem::GetItemId)
          .addFunction ("GetRequiresCharges", &dota::DotaItem::GetRequiresCharges)
          .addFunction ("GetCurrentCharges", &dota::DotaItem::GetCurrentCharges)
          .addFunction ("GetInitialCharges", &dota::DotaItem::GetInitialCharges)
          .addFunction ("GetPurchaser", &dota::DotaItem::GetPurchaser)
          .addFunction ("GetPurchaseTime", &dota::DotaItem::GetPurchaseTime)
        .endClass ()
        .deriveClass <dota::BasePlayer, dota::BaseEntity> ("BasePlayer")
          .addFunction ("GetPlayerId", &dota::BasePlayer::GetPlayerId)
        .endClass ()
        .deriveClass <dota::DotaPlayer, dota::BasePlayer> ("DotaPlayer")
          .addFunction ("GetAssignedHero", &dota::DotaPlayer::GetAssignedHero)
          .addFunction ("PrepareUnitOrders", &dota::DotaPlayer::PrepareUnitOrders)
        .endClass ()
        .deriveClass <LuaDotaPlayer, dota::DotaPlayer> ("LuaDotaPlayer")
          .addFunction ("GetAssignedHero", &LuaDotaPlayer::LuaGetAssignedHero)
        .endClass ()
      .endNamespace();

    luabridge::getGlobalNamespace(state)
      .beginNamespace("dota")
        .beginClass<ClientClass>("ClientClass")
         .addFunction("GetName", &ClientClass::GetName)
        .endClass()
      .endNamespace();
  }
}