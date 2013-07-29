#include "DotaGlobal.h"

unsigned long CGameRules::get_announcer_address_ = 0;
unsigned long CGameRules::get_announcer_killingspree_address_ = 0;
CGameRules* CGameRules::instance_ = nullptr;

CHud* CHud::instance_ = nullptr;
unsigned long CHud::find_element_address_ = 0;

unsigned long CMinimap::create_location_ping_address_ = 0;

unsigned long CModifierManager::get_modifier_constant_address_ = 0;
unsigned long CModifierManager::modifier_default_params_ = 0;

unsigned long ParticleManager::create_particle_address_ = 0;
unsigned long ParticleManager::set_particle_ent_address_ = 0;
unsigned long ParticleManager::release_particle_address_ = 0;
unsigned long ParticleManager::precache_particle_system_adddress_ = 0;
unsigned long ParticleManager::set_particle_control_address_ = 0;
unsigned long ParticleManager::destroy_particle_address_ = 0;
ParticleManager* ParticleManager::instance_ = nullptr;

unsigned long CBaseNpc::is_entity_in_range_address_ = 0;
unsigned long CBaseNpc::get_attack_range_address_ = 0;

unsigned long CAnnouncer::set_announcer_item_address_ = 0;

unsigned long CDotaPlayer::prepare_unit_orders_address = 0;
unsigned long CDotaPlayer::show_ring_effect_address = 0;
unsigned long CDotaPlayer::set_click_behaviour_address = 0;
unsigned long CDotaPlayer::get_local_selected_unit_address = 0;

CCommandBuffer* CCommandBuffer::instance_ = nullptr;

CUtlVector<IGameSystem*>* GameSystemsRetriever::game_systems_ = nullptr;

CDotaGameManager* CDotaGameManager::instance_ = nullptr;