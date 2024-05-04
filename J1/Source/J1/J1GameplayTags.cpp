#include "J1GameplayTags.h"

namespace J1GameplayTags
{
	/** Input **/
	UE_DEFINE_GAMEPLAY_TAG(Input_Action_SetDestinationClick, "Input.Action.SetDestinationClick");
	UE_DEFINE_GAMEPLAY_TAG(Input_Action_BaseAttack, "Input.Action.BaseAttack");
	UE_DEFINE_GAMEPLAY_TAG(Input_Action_Q, "Input.Action.Q");

	/** Event **/
	UE_DEFINE_GAMEPLAY_TAG(Event_Montage_Begin, "Event.Montage.Begin");
	UE_DEFINE_GAMEPLAY_TAG(Event_Montage_End, "Event.Montage.End");
	UE_DEFINE_GAMEPLAY_TAG(Event_Montage_Attack, "Event.Montage.Attack");

	/** Aurora **/
	UE_DEFINE_GAMEPLAY_TAG(Aurora, "Aurora");
	UE_DEFINE_GAMEPLAY_TAG(Aurora_Skill, "Aurora.Skill");
	/* Aurora : normal attack */
	UE_DEFINE_GAMEPLAY_TAG(Aurora_Skill_NormalAttack, "Aurora.Skill.NormalAttack");
	UE_DEFINE_GAMEPLAY_TAG(Aurora_Skill_NormalAttack_1, "Aurora.Skill.NormalAttack.0");
	UE_DEFINE_GAMEPLAY_TAG(Aurora_Skill_NormalAttack_End, "Aurora.Skill.NormalAttack.End");
	/* Aurora : q skill */
	UE_DEFINE_GAMEPLAY_TAG(Aurora_Skill_QSkill, "Aurora.Skill.QSkill");
	UE_DEFINE_GAMEPLAY_TAG(Aurora_Skill_QSkill_0, "Aurora.Skill.QSkill.0");
	UE_DEFINE_GAMEPLAY_TAG(Aurora_Skill_QSkill_End, "Aurora.Skill.QSkill.End");

	/** Sevarog **/
	UE_DEFINE_GAMEPLAY_TAG(Sevarog, "Sevarog");
	UE_DEFINE_GAMEPLAY_TAG(Sevarog_Skill, "Sevarog.Skill");
	UE_DEFINE_GAMEPLAY_TAG(Sevarog_Skill_NormalAttack, "Sevarog.Skill.NormalAttack");
	UE_DEFINE_GAMEPLAY_TAG(Sevarog_Skill_NormalAttack_0, "Sevarog.Skill.NormalAttack.0");
	UE_DEFINE_GAMEPLAY_TAG(Sevarog_Skill_NormalAttack_End, "Sevarog.Skill.NormalAttack.End");
	/* Sevarog : poison rain */
	UE_DEFINE_GAMEPLAY_TAG(Sevarog_Skill_PoisonRain, "Sevarog.Skill.PoisonRain");
	UE_DEFINE_GAMEPLAY_TAG(Sevarog_Skill_PoisonRain_0, "Sevarog.Skill.PoisonRain.0");
	UE_DEFINE_GAMEPLAY_TAG(Sevarog_Skill_PoisonRain_1, "Sevarog.Skill.PoisonRain.1");
	UE_DEFINE_GAMEPLAY_TAG(Sevarog_Skill_PoisonRain_2, "Sevarog.Skill.PoisonRain.2");
	UE_DEFINE_GAMEPLAY_TAG(Sevarog_Skill_PoisonRain_3, "Sevarog.Skill.PoisonRain.3");
	UE_DEFINE_GAMEPLAY_TAG(Sevarog_Skill_PoisonRain_4, "Sevarog.Skill.PoisonRain.4");
	UE_DEFINE_GAMEPLAY_TAG(Sevarog_Skill_PoisonRain_5, "Sevarog.Skill.PoisonRain.5");
	UE_DEFINE_GAMEPLAY_TAG(Sevarog_Skill_PoisonRain_6, "Sevarog.Skill.PoisonRain.6");
	UE_DEFINE_GAMEPLAY_TAG(Sevarog_Skill_PoisonRain_7, "Sevarog.Skill.PoisonRain.7");
	UE_DEFINE_GAMEPLAY_TAG(Sevarog_Skill_PoisonRain_End, "Sevarog.Skill.PoisonRain.End");

	/* Sevarog : Gimmick */
	UE_DEFINE_GAMEPLAY_TAG(Sevarog_Gimmick, "Sevarog.Gimmick");
	UE_DEFINE_GAMEPLAY_TAG(Sevarog_Gimmick_FindSafeZone, "Sevarog.Gimmick.FindSafeZone");
	UE_DEFINE_GAMEPLAY_TAG(Sevarog_Gimmick_FindSafeZone_End, "Sevarog.Gimmick.FindSafeZone.End");
}