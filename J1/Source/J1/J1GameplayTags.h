#pragma once

#include "NativeGameplayTags.h"

namespace J1GameplayTags
{
	/** Input **/
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Action_SetDestinationClick);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Action_BaseAttack);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Action_Q);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Montage_Begin);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Montage_End);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Montage_Attack);

	/** Aurora **/
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Aurora);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Aurora_Skill);

	/* Aurora : normal attack */
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Aurora_Skill_NormalAttack);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Aurora_Skill_NormalAttack_0);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Aurora_Skill_NormalAttack_End);

	/* Aurora : q skill */
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Aurora_Skill_QSkill);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Aurora_Skill_QSkill_0);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Aurora_Skill_QSkill_End);

	/** GoblinSpear **/
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GoblinSpear);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GoblinSpear_Skill);

	/* GoblinSpear : normal attack */
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GoblinSpear_Skill_NormalAttack);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GoblinSpear_Skill_NormalAttack_0);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GoblinSpear_Skill_NormalAttack_End);

	/** GoblinSlingshot **/
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GoblinSlingshot);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GoblinSlingshot_Skill);

	/* GoblinSlingshot : normal attack */
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GoblinSlingshot_Skill_RangedNormalAttack);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GoblinSlingshot_Skill_RangedNormalAttack_0);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GoblinSlingshot_Skill_RangedNormalAttack_End);

	/** DemonRed **/
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(DemonRed);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(DemonRed_Skill);

	/* DemonRed : normal attack */
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(DemonRed_Skill_NormalAttack);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(DemonRed_Skill_NormalAttack_0);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(DemonRed_Skill_NormalAttack_End);

	/** Sevarog **/
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Sevarog);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Sevarog_Skill);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Sevarog_Skill_NormalAttack);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Sevarog_Skill_NormalAttack_0);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Sevarog_Skill_NormalAttack_End);

	/* Sevarog : poison rain */
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Sevarog_Skill_PoisonRain);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Sevarog_Skill_PoisonRain_0);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Sevarog_Skill_PoisonRain_1);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Sevarog_Skill_PoisonRain_2);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Sevarog_Skill_PoisonRain_3);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Sevarog_Skill_PoisonRain_4);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Sevarog_Skill_PoisonRain_5);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Sevarog_Skill_PoisonRain_6);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Sevarog_Skill_PoisonRain_7);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Sevarog_Skill_PoisonRain_End);
	/* Sevarog : spread cloud */
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Sevarog_Skill_SpreadCloud);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Sevarog_Skill_SpreadCloud_0);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Sevarog_Skill_SpreadCloud_End);

	/* Sevarog : Gimmick */
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Sevarog_Gimmick);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Sevarog_Gimmick_FindSafeZone);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Sevarog_Gimmick_FindSafeZone_End);
}