#include "J1GameplayTags.h"

namespace J1GameplayTags
{
	/** Input **/
	UE_DEFINE_GAMEPLAY_TAG(Input_Action_SetDestinationClick, "Input.Action.SetDestinationClick");
	UE_DEFINE_GAMEPLAY_TAG(Input_Action_BaseAttack, "Input.Action.BaseAttack");

	/** Event **/
	UE_DEFINE_GAMEPLAY_TAG(Event_Montage_Begin, "Event.Montage.Begin");
	UE_DEFINE_GAMEPLAY_TAG(Event_Montage_End, "Event.Montage.End");
	UE_DEFINE_GAMEPLAY_TAG(Event_Montage_Attack, "Event.Montage.Attack");

	/** Aurora **/
	UE_DEFINE_GAMEPLAY_TAG(Aurora, "Aurora");
	UE_DEFINE_GAMEPLAY_TAG(Aurora_Skill, "Aurora.Skill");
	UE_DEFINE_GAMEPLAY_TAG(Aurora_Skill_NormalAttack, "Aurora.Skill.NormalAttack");
	UE_DEFINE_GAMEPLAY_TAG(Aurora_Skill_NormalAttack_1, "Aurora.Skill.NormalAttack.0");
	UE_DEFINE_GAMEPLAY_TAG(Aurora_Skill_NormalAttack_End, "Aurora.Skill.NormalAttack.End");

	/** Sevarog **/
	UE_DEFINE_GAMEPLAY_TAG(Sevarog, "Sevarog");
	UE_DEFINE_GAMEPLAY_TAG(Sevarog_Skill, "Sevarog.Skill");
	UE_DEFINE_GAMEPLAY_TAG(Sevarog_Skill_NormalAttack, "Sevarog.Skill.NormalAttack");
	UE_DEFINE_GAMEPLAY_TAG(Sevarog_Skill_NormalAttack_1, "Sevarog.Skill.NormalAttack.0");
	UE_DEFINE_GAMEPLAY_TAG(Sevarog_Skill_NormalAttack_End, "Sevarog.Skill.NormalAttack.End");
}