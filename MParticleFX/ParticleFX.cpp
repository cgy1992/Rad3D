#include "ParticleFX.h"
#include "PSManager.h"

#include "PSBillboardSet.h"
#include "PSRibbonSet.h"
#include "PSLineSet.h"
#include "PSSliceSet.h"
#include "PSDecalSet.h"
#include "PSMeshSet.h"
#include "PSParticleSystemSet.h"
#include "PSLightSet.h"
#include "PSGroupSet.h"
#include "PSEmitterSet.h"

#include "PSShaderStandard.h"
#include "PSShaderMask.h"
#include "PSShaderMesh.h"

#include "PSEmitterPoint.h"
#include "PSEmitterBox.h"
#include "PSEmitterCylinder.h"
#include "PSEmitterEllipsoid.h"
#include "PSEmitterMesh.h"

#include "PSModifierColor.h"
#include "PSModifierDeflectorPlane.h"
#include "PSModifierDirectionRandom.h"
#include "PSModifierForce.h"
#include "PSModifierRotation.h"
#include "PSModifierRotation2.h"
#include "PSModifierScale.h"
#include "PSModifierUVAnim.h"
#include "PSModifierUVScroll.h"
#include "PSModifierMagneticField.h"

#include "RadC.h"
#include "MAudioSystem.h"
#include "PSManager.h"

namespace Rad {

	//
	bool FX_PlaySound(radc_var * result, const radc_varlist * args)
	{
		if (args->size() == 2 &&
			args->getType(0) == radc_enum::type_string &&
			args->getType(1) == radc_enum::type_real)
		{
			const char * filename = args->getString(0);
			int category = (int)args->getReal(1);

			AudioSystem::Instance()->PlaySound(filename, category);
		}

		return false;
	}

	bool FX_PlayParticle(radc_var * result, const radc_varlist * args)
	{
		d_assert (radc_internalArgs() != NULL);

		ParticleSystem * ps = (ParticleSystem *)radc_internalArgs()->getPtr(0);
		Particle * p = (Particle *)radc_internalArgs()->getPtr(1);

		if (args->size() == 2 &&
			args->getType(0) == radc_enum::type_string &&
			args->getType(1) == radc_enum::type_real)
		{
			const char * filename = args->getString(0);
			float duration = (float)args->getReal(1);

			PS_Manager::Instance()->Play(filename, p->Position, duration);

			return true;
		}

		return false;
	}

	bool FX_Kill(radc_var * result, const radc_varlist * args)
	{
		d_assert (radc_internalArgs() != NULL);

		ParticleSystem * ps = (ParticleSystem *)radc_internalArgs()->getPtr(0);
		Particle * p = (Particle *)radc_internalArgs()->getPtr(1);

		if (args->size() == 0)
		{
			p->Life = 0;

			return true;
		}

		return false;
	}

	bool FX_Stop(radc_var * result, const radc_varlist * args)
	{
		d_assert (radc_internalArgs() != NULL);

		ParticleSystem * ps = (ParticleSystem *)radc_internalArgs()->getPtr(0);
		Particle * p = (Particle *)radc_internalArgs()->getPtr(1);

		if (args->size() == 0)
		{
			p->Speed = 0;

			return true;
		}

		return false;
	}

	bool FX_Set_Enable(radc_var * result, const radc_varlist * args)
	{
		d_assert (radc_internalArgs() != NULL);

		ParticleSystem * ps = (ParticleSystem *)radc_internalArgs()->getPtr(0);
		Particle * p = (Particle *)radc_internalArgs()->getPtr(1);

		if (args->size() == 2 && 
			args->getType(0) == radc_enum::type_real)
		{
			int rindex = args->getInt(0);

			if (rindex >= 0 && rindex < ps->GetSetCount())
			{
				PS_Set * render = ps->GetSet(rindex);
				
				render->SetEnable(true);

				return true;
			}
		}

		return false;
	}

	bool FX_Set_Disable(radc_var * result, const radc_varlist * args)
	{
		d_assert (radc_internalArgs() != NULL);

		ParticleSystem * ps = (ParticleSystem *)radc_internalArgs()->getPtr(0);
		Particle * p = (Particle *)radc_internalArgs()->getPtr(1);

		if (args->size() == 2 && 
			args->getType(0) == radc_enum::type_real)
		{
			int rindex = args->getInt(0);

			if (rindex >= 0 && rindex < ps->GetSetCount())
			{
				PS_Set * render = ps->GetSet(rindex);

				render->SetEnable(false);

				return true;
			}
		}

		return false;
	}

	bool FX_Emitter_Enable(radc_var * result, const radc_varlist * args)
	{
		d_assert (radc_internalArgs() != NULL);

		ParticleSystem * ps = (ParticleSystem *)radc_internalArgs()->getPtr(0);
		Particle * p = (Particle *)radc_internalArgs()->getPtr(1);

		if (args->size() == 2 && 
			args->getType(0) == radc_enum::type_real &&
			args->getType(1) == radc_enum::type_real)
		{
			int rindex = args->getInt(0);
			int eindex = args->getInt(1);

			if (rindex >= 0 && rindex < ps->GetSetCount())
			{
				PS_Set * render = ps->GetSet(rindex);
				if (eindex >= 0 && eindex < render->GetEmitterCount())
				{
					PS_Emitter * emitter = render->GetEmitter(eindex);
					
					emitter->SetEnable(true);
				}

				return true;
			}
		}

		return false;
	}

	bool FX_Emitter_Disable(radc_var * result, const radc_varlist * args)
	{
		d_assert (radc_internalArgs() != NULL);

		ParticleSystem * ps = (ParticleSystem *)radc_internalArgs()->getPtr(0);
		Particle * p = (Particle *)radc_internalArgs()->getPtr(1);

		if (args->size() == 2 && 
			args->getType(0) == radc_enum::type_real &&
			args->getType(1) == radc_enum::type_real)
		{
			int rindex = args->getInt(0);
			int eindex = args->getInt(1);

			if (rindex >= 0 && rindex < ps->GetSetCount())
			{
				PS_Set * set = ps->GetSet(rindex);
				if (eindex >= 0 && eindex < set->GetEmitterCount())
				{
					PS_Emitter * emitter = set->GetEmitter(eindex);

					emitter->SetEnable(false);
				}

				return true;
			}
		}

		return false;
	}

	bool FX_Modifier_Enable(radc_var * result, const radc_varlist * args)
	{
		d_assert (radc_internalArgs() != NULL);

		ParticleSystem * ps = (ParticleSystem *)radc_internalArgs()->getPtr(0);
		Particle * p = (Particle *)radc_internalArgs()->getPtr(1);

		if (args->size() == 2 && 
			args->getType(0) == radc_enum::type_real &&
			args->getType(1) == radc_enum::type_real)
		{
			int rindex = args->getInt(0);
			int findex = args->getInt(1);

			if (rindex >= 0 && rindex < ps->GetSetCount())
			{
				PS_Set * set = ps->GetSet(rindex);
				if (findex >= 0 && findex < set->GetModifierCount())
				{
					PS_Modifier * modifier = set->GetModifier(findex);

					modifier->SetEnable(true);
				}

				return true;
			}
		}

		return false;
	}

	bool FX_Modifier_Disable(radc_var * result, const radc_varlist * args)
	{
		d_assert (radc_internalArgs() != NULL);

		ParticleSystem * ps = (ParticleSystem *)radc_internalArgs()->getPtr(0);
		Particle * p = (Particle *)radc_internalArgs()->getPtr(1);

		if (args->size() == 2 && 
			args->getType(0) == radc_enum::type_real &&
			args->getType(1) == radc_enum::type_real)
		{
			int rindex = args->getInt(0);
			int findex = args->getInt(1);

			if (rindex >= 0 && rindex < ps->GetSetCount())
			{
				PS_Set * set = ps->GetSet(rindex);
				if (findex >= 0 && findex < set->GetModifierCount())
				{
					PS_Modifier * modifier = set->GetModifier(findex);

					modifier->SetEnable(false);
				}

				return true;
			}
		}

		return false;
	}

	bool FX_Emitter_Trace(radc_var * result, const radc_varlist * args)
	{
		d_assert (radc_internalArgs() != NULL);

		ParticleSystem * ps = (ParticleSystem *)radc_internalArgs()->getPtr(0);
		Particle * p = (Particle *)radc_internalArgs()->getPtr(1);

		if (args->size() == 3 &&
			args->getType(0) == radc_enum::type_real &&
			args->getType(1) == radc_enum::type_real &&
			args->getType(2) == radc_enum::type_real)
		{
			int rindex = args->getInt(0);
			int eindex = args->getInt(1);
			int flags = args->getInt(2);

			if (ps && rindex >= 0 && rindex < ps->GetSetCount())
			{
				PS_Set * set = ps->GetSet(rindex);
				if (eindex >= 0 && eindex < set->GetEmitterCount())
				{
					PS_Emitter * emitter = set->GetEmitter(eindex);

					if (!set->IsLocalSpace())
					{
						Mat4 m = ps->GetWorldTM();
						m.Inverse();

						p->Position.TransformA(m);
					}

					emitter->SetPosition(p->Position);

					if (flags)
					{
						emitter->SetRotation(p->Rotation);
					}
				}

				return true;
			}
		}

		return false;
	}

	radc_reg FX_LIB[] = {
		RADC_REG( "PlaySound", FX_PlaySound, "args: filename, category." ),

		RADC_REG( "PlayParticle", FX_PlayParticle, "args: filename, duration." ),

		RADC_REG( "Kill", FX_Kill, "no args." ),
		RADC_REG( "Stop", FX_Stop, "no args." ),

		RADC_REG( "Set_Enable", FX_Set_Enable, "args: renderIndex." ),
		RADC_REG( "Set_Disable", FX_Set_Disable, "args: renderIndex." ),

		RADC_REG( "Emitter_Enable", FX_Emitter_Enable, "args: renderIndex, emitterIndex." ),
		RADC_REG( "Emitter_Disable", FX_Emitter_Disable, "args: renderIndex, emitterIndex." ),
		RADC_REG( "Emitter_Trace", FX_Emitter_Trace, "args: renderIndex, emitterIndex, rotationEnable." ),

		RADC_REG( "Modifier_Enable", FX_Modifier_Enable, "args: renderIndex, modifierIndex." ),
		RADC_REG( "Modifier_Disable", FX_Modifier_Disable, "args: renderIndex, modifierIndex." ),

		RADC_REG( NULL, NULL, NULL )
	};


	ParticleFX::ParticleFX()
	{
		radc_regLib("FX", FX_LIB, NULL);

		Root::REG_OBJECT_T<PS_BillboardSet>();
		Root::REG_OBJECT_T<PS_RibbonSet>();
		Root::REG_OBJECT_T<PS_LineSet>();
		Root::REG_OBJECT_T<PS_SliceSet>();
		Root::REG_OBJECT_T<PS_DecalSet>();
		Root::REG_OBJECT_T<PS_MeshSet>();
		Root::REG_OBJECT_T<PS_ParticleSystemSet>();
		Root::REG_OBJECT_T<PS_LightSet>();
		Root::REG_OBJECT_T<PS_GroupSet>();
		Root::REG_OBJECT_T<PS_EmitterSet>();

		Root::REG_OBJECT_T<PS_ShaderStandard>();
		Root::REG_OBJECT_T<PS_ShaderMask>();
		Root::REG_OBJECT_T<PS_ShaderMesh>();

		Root::REG_OBJECT_T<PS_EmitterPoint>();
		Root::REG_OBJECT_T<PS_EmitterBox>();
		Root::REG_OBJECT_T<PS_EmitterCylinder>();
		Root::REG_OBJECT_T<PS_EmitterEllipsoid>();
		Root::REG_OBJECT_T<PS_EmitterMesh>();

		Root::REG_OBJECT_T<PS_ModifierColor>();
		Root::REG_OBJECT_T<PS_ModifierDeflectorPlane>();
		Root::REG_OBJECT_T<PS_ModifierDirectionRandom>();
		Root::REG_OBJECT_T<PS_ModifierForce>();
		Root::REG_OBJECT_T<PS_ModifierScale>();
		Root::REG_OBJECT_T<PS_ModifierUVAnim>();
		Root::REG_OBJECT_T<PS_ModifierUVAnimKF>();
		Root::REG_OBJECT_T<PS_ModifierRotation>();
		Root::REG_OBJECT_T<PS_ModifierRotation2>();
		Root::REG_OBJECT_T<PS_ModifierUVScroll>();
		Root::REG_OBJECT_T<PS_ModifierUVScroll2>();
		Root::REG_OBJECT_T<PS_ModifierMagneticField>();
	}

	ParticleFX::~ParticleFX()
	{
	}
}
