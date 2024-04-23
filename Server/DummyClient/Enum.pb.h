// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Enum.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_Enum_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_Enum_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3021000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3021012 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_Enum_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_Enum_2eproto {
  static const uint32_t offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_Enum_2eproto;
PROTOBUF_NAMESPACE_OPEN
PROTOBUF_NAMESPACE_CLOSE
namespace Protocol {

enum ObjectType : int {
  OBJECT_TYPE_NONE = 0,
  OBJECT_TYPE_CREATURE = 1,
  OBJECT_TYPE_PROJECTILE = 2,
  OBJECT_TYPE_ENV = 3,
  ObjectType_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::min(),
  ObjectType_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::max()
};
bool ObjectType_IsValid(int value);
constexpr ObjectType ObjectType_MIN = OBJECT_TYPE_NONE;
constexpr ObjectType ObjectType_MAX = OBJECT_TYPE_ENV;
constexpr int ObjectType_ARRAYSIZE = ObjectType_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* ObjectType_descriptor();
template<typename T>
inline const std::string& ObjectType_Name(T enum_t_value) {
  static_assert(::std::is_same<T, ObjectType>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function ObjectType_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    ObjectType_descriptor(), enum_t_value);
}
inline bool ObjectType_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, ObjectType* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<ObjectType>(
    ObjectType_descriptor(), name, value);
}
enum CreatureType : int {
  CREATURE_TYPE_NONE = 0,
  CREATURE_TYPE_PLAYER = 1,
  CREATURE_TYPE_MONSTER = 2,
  CREATURE_TYPE_NPC = 3,
  CreatureType_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::min(),
  CreatureType_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::max()
};
bool CreatureType_IsValid(int value);
constexpr CreatureType CreatureType_MIN = CREATURE_TYPE_NONE;
constexpr CreatureType CreatureType_MAX = CREATURE_TYPE_NPC;
constexpr int CreatureType_ARRAYSIZE = CreatureType_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* CreatureType_descriptor();
template<typename T>
inline const std::string& CreatureType_Name(T enum_t_value) {
  static_assert(::std::is_same<T, CreatureType>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function CreatureType_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    CreatureType_descriptor(), enum_t_value);
}
inline bool CreatureType_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, CreatureType* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<CreatureType>(
    CreatureType_descriptor(), name, value);
}
enum PlayerType : int {
  PLAYER_TYPE_NONE = 0,
  PLAYER_TYPE_KNIGHT = 1,
  PLAYER_TYPE_MAGE = 2,
  PLAYER_TYPE_ARCHER = 3,
  PlayerType_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::min(),
  PlayerType_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::max()
};
bool PlayerType_IsValid(int value);
constexpr PlayerType PlayerType_MIN = PLAYER_TYPE_NONE;
constexpr PlayerType PlayerType_MAX = PLAYER_TYPE_ARCHER;
constexpr int PlayerType_ARRAYSIZE = PlayerType_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* PlayerType_descriptor();
template<typename T>
inline const std::string& PlayerType_Name(T enum_t_value) {
  static_assert(::std::is_same<T, PlayerType>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function PlayerType_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    PlayerType_descriptor(), enum_t_value);
}
inline bool PlayerType_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, PlayerType* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<PlayerType>(
    PlayerType_descriptor(), name, value);
}
enum MonsterType : int {
  MONSTER_TYPE_NONE = 0,
  MONSTER_TYPE_GENERAL = 1,
  MONSTER_TYPE_BOSS = 2,
  MonsterType_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::min(),
  MonsterType_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::max()
};
bool MonsterType_IsValid(int value);
constexpr MonsterType MonsterType_MIN = MONSTER_TYPE_NONE;
constexpr MonsterType MonsterType_MAX = MONSTER_TYPE_BOSS;
constexpr int MonsterType_ARRAYSIZE = MonsterType_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* MonsterType_descriptor();
template<typename T>
inline const std::string& MonsterType_Name(T enum_t_value) {
  static_assert(::std::is_same<T, MonsterType>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function MonsterType_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    MonsterType_descriptor(), enum_t_value);
}
inline bool MonsterType_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, MonsterType* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<MonsterType>(
    MonsterType_descriptor(), name, value);
}
enum MoveState : int {
  MOVE_STATE_NONE = 0,
  MOVE_STATE_IDLE = 1,
  MOVE_STATE_RUN = 2,
  MOVE_STATE_DASH = 3,
  MOVE_STATE_SKILL = 4,
  MOVE_STATE_DEAD = 6,
  MoveState_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::min(),
  MoveState_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::max()
};
bool MoveState_IsValid(int value);
constexpr MoveState MoveState_MIN = MOVE_STATE_NONE;
constexpr MoveState MoveState_MAX = MOVE_STATE_DEAD;
constexpr int MoveState_ARRAYSIZE = MoveState_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* MoveState_descriptor();
template<typename T>
inline const std::string& MoveState_Name(T enum_t_value) {
  static_assert(::std::is_same<T, MoveState>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function MoveState_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    MoveState_descriptor(), enum_t_value);
}
inline bool MoveState_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, MoveState* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<MoveState>(
    MoveState_descriptor(), name, value);
}
enum BuffType : int {
  BUFF_TYPE_NONE = 0,
  BUFF_TYPE_HIT = 1,
  BUFF_TYPE_PARALYSIS = 2,
  BUFF_TYPE_PUSH = 3,
  BUFF_TYPE_MEZ = 4,
  BuffType_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::min(),
  BuffType_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::max()
};
bool BuffType_IsValid(int value);
constexpr BuffType BuffType_MIN = BUFF_TYPE_NONE;
constexpr BuffType BuffType_MAX = BUFF_TYPE_MEZ;
constexpr int BuffType_ARRAYSIZE = BuffType_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* BuffType_descriptor();
template<typename T>
inline const std::string& BuffType_Name(T enum_t_value) {
  static_assert(::std::is_same<T, BuffType>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function BuffType_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    BuffType_descriptor(), enum_t_value);
}
inline bool BuffType_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, BuffType* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<BuffType>(
    BuffType_descriptor(), name, value);
}
enum BuffDurationType : int {
  BUFF_DURATION_TYPE_NONE = 0,
  BUFF_DURATION_TYPE_INSTANT = 1,
  BUFF_DURATION_TYPE_FINITE = 2,
  BUFF_DURATION_TYPE_INFINITE = 3,
  BuffDurationType_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::min(),
  BuffDurationType_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::max()
};
bool BuffDurationType_IsValid(int value);
constexpr BuffDurationType BuffDurationType_MIN = BUFF_DURATION_TYPE_NONE;
constexpr BuffDurationType BuffDurationType_MAX = BUFF_DURATION_TYPE_INFINITE;
constexpr int BuffDurationType_ARRAYSIZE = BuffDurationType_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* BuffDurationType_descriptor();
template<typename T>
inline const std::string& BuffDurationType_Name(T enum_t_value) {
  static_assert(::std::is_same<T, BuffDurationType>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function BuffDurationType_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    BuffDurationType_descriptor(), enum_t_value);
}
inline bool BuffDurationType_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, BuffDurationType* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<BuffDurationType>(
    BuffDurationType_descriptor(), name, value);
}
enum SkillType : int {
  SKILL_TYPE_NONE = 0,
  SKILL_TYPE_CLOSE = 1,
  SKILL_TYPE_PROJECTILE = 2,
  SkillType_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::min(),
  SkillType_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::max()
};
bool SkillType_IsValid(int value);
constexpr SkillType SkillType_MIN = SKILL_TYPE_NONE;
constexpr SkillType SkillType_MAX = SKILL_TYPE_PROJECTILE;
constexpr int SkillType_ARRAYSIZE = SkillType_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* SkillType_descriptor();
template<typename T>
inline const std::string& SkillType_Name(T enum_t_value) {
  static_assert(::std::is_same<T, SkillType>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function SkillType_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    SkillType_descriptor(), enum_t_value);
}
inline bool SkillType_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, SkillType* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<SkillType>(
    SkillType_descriptor(), name, value);
}
enum EffectType : int {
  EFFECT_TYPE_NONE = 0,
  EFFECT_TYPE_RECTANGLE = 1,
  EFFECT_TYPE_CIRCLE = 2,
  EFFECT_TYPE_PIZZA = 3,
  EffectType_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::min(),
  EffectType_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::max()
};
bool EffectType_IsValid(int value);
constexpr EffectType EffectType_MIN = EFFECT_TYPE_NONE;
constexpr EffectType EffectType_MAX = EFFECT_TYPE_PIZZA;
constexpr int EffectType_ARRAYSIZE = EffectType_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* EffectType_descriptor();
template<typename T>
inline const std::string& EffectType_Name(T enum_t_value) {
  static_assert(::std::is_same<T, EffectType>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function EffectType_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    EffectType_descriptor(), enum_t_value);
}
inline bool EffectType_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, EffectType* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<EffectType>(
    EffectType_descriptor(), name, value);
}
enum SkillSlot : int {
  SKILL_SLOT_NONE = 0,
  SKILL_SLOT_ATTACK = 1,
  SKILL_SLOT_Q = 2,
  SKILL_SLOT_W = 3,
  SKILL_SLOT_E = 4,
  SKILL_SLOT_R = 5,
  SKILL_SLOT_DASH = 6,
  SkillSlot_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::min(),
  SkillSlot_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::max()
};
bool SkillSlot_IsValid(int value);
constexpr SkillSlot SkillSlot_MIN = SKILL_SLOT_NONE;
constexpr SkillSlot SkillSlot_MAX = SKILL_SLOT_DASH;
constexpr int SkillSlot_ARRAYSIZE = SkillSlot_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* SkillSlot_descriptor();
template<typename T>
inline const std::string& SkillSlot_Name(T enum_t_value) {
  static_assert(::std::is_same<T, SkillSlot>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function SkillSlot_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    SkillSlot_descriptor(), enum_t_value);
}
inline bool SkillSlot_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, SkillSlot* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<SkillSlot>(
    SkillSlot_descriptor(), name, value);
}
enum RoomType : int {
  ROOOM_TYPE_NONE = 0,
  ROOM_TYPE_START_ROOM = 1,
  RoomType_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::min(),
  RoomType_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::max()
};
bool RoomType_IsValid(int value);
constexpr RoomType RoomType_MIN = ROOOM_TYPE_NONE;
constexpr RoomType RoomType_MAX = ROOM_TYPE_START_ROOM;
constexpr int RoomType_ARRAYSIZE = RoomType_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* RoomType_descriptor();
template<typename T>
inline const std::string& RoomType_Name(T enum_t_value) {
  static_assert(::std::is_same<T, RoomType>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function RoomType_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    RoomType_descriptor(), enum_t_value);
}
inline bool RoomType_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, RoomType* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<RoomType>(
    RoomType_descriptor(), name, value);
}
// ===================================================================


// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace Protocol

PROTOBUF_NAMESPACE_OPEN

template <> struct is_proto_enum< ::Protocol::ObjectType> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::Protocol::ObjectType>() {
  return ::Protocol::ObjectType_descriptor();
}
template <> struct is_proto_enum< ::Protocol::CreatureType> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::Protocol::CreatureType>() {
  return ::Protocol::CreatureType_descriptor();
}
template <> struct is_proto_enum< ::Protocol::PlayerType> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::Protocol::PlayerType>() {
  return ::Protocol::PlayerType_descriptor();
}
template <> struct is_proto_enum< ::Protocol::MonsterType> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::Protocol::MonsterType>() {
  return ::Protocol::MonsterType_descriptor();
}
template <> struct is_proto_enum< ::Protocol::MoveState> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::Protocol::MoveState>() {
  return ::Protocol::MoveState_descriptor();
}
template <> struct is_proto_enum< ::Protocol::BuffType> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::Protocol::BuffType>() {
  return ::Protocol::BuffType_descriptor();
}
template <> struct is_proto_enum< ::Protocol::BuffDurationType> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::Protocol::BuffDurationType>() {
  return ::Protocol::BuffDurationType_descriptor();
}
template <> struct is_proto_enum< ::Protocol::SkillType> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::Protocol::SkillType>() {
  return ::Protocol::SkillType_descriptor();
}
template <> struct is_proto_enum< ::Protocol::EffectType> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::Protocol::EffectType>() {
  return ::Protocol::EffectType_descriptor();
}
template <> struct is_proto_enum< ::Protocol::SkillSlot> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::Protocol::SkillSlot>() {
  return ::Protocol::SkillSlot_descriptor();
}
template <> struct is_proto_enum< ::Protocol::RoomType> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::Protocol::RoomType>() {
  return ::Protocol::RoomType_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_Enum_2eproto
