
#
# MParticleFX
#

include $(CLEAR_VARS)

LOCAL_MODULE := libMParticleFX
LOCAL_SHARED_LIBRARIES := libMCore

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/../../MCore \
	$(LOCAL_PATH)/../../MCore/Platform/android \
	$(LOCAL_PATH)/../../MParticleFX \
	$(LOCAL_PATH)/../../ThirdParty/xml
	
	
LOCAL_SRC_FILES := \
	../../MParticleFX/ParticleFX.cpp \
	../../MParticleFX/PSBillboard.cpp \
	../../MParticleFX/PSDecal.cpp \
	../../MParticleFX/PSEmitterBox.cpp \
	../../MParticleFX/PSEmitterCylinder.cpp \
	../../MParticleFX/PSEmitterEllipsoid.cpp \
	../../MParticleFX/PSEmitterMesh.cpp \
	../../MParticleFX/PSEmitterPoint.cpp \
	../../MParticleFX/PSGroup.cpp \
	../../MParticleFX/PSLight.cpp \
	../../MParticleFX/PSLine.cpp \
	../../MParticleFX/PSMesh.cpp \
	../../MParticleFX/PSModifierKeyController.cpp \
	../../MParticleFX/PSModifierMagneticField.cpp \
	../../MParticleFX/PSModifierColor.cpp \
	../../MParticleFX/PSModifierDeflectorPlane.cpp \
	../../MParticleFX/PSModifierDirectionRandom.cpp \
	../../MParticleFX/PSModifierForce.cpp \
	../../MParticleFX/PSModifierRotation.cpp \
	../../MParticleFX/PSModifierRotation2.cpp \
	../../MParticleFX/PSModifierScale.cpp \
	../../MParticleFX/PSModifierUVAnim.cpp \
	../../MParticleFX/PSModifierUVScroll.cpp \
	../../MParticleFX/PSParticleSystem.cpp \
	../../MParticleFX/PSParticleSystemSet.cpp \
	../../MParticleFX/PSBillboardSet.cpp \
	../../MParticleFX/PSDecalSet.cpp \
	../../MParticleFX/PSEmitterSet.cpp \
	../../MParticleFX/PSGroupSet.cpp \
	../../MParticleFX/PSLightSet.cpp \
	../../MParticleFX/PSLineSet.cpp \
	../../MParticleFX/PSMeshSet.cpp \
	../../MParticleFX/PSRibbonSet.cpp \
	../../MParticleFX/PSSliceSet.cpp \
	../../MParticleFX/PSRibbon.cpp \
	../../MParticleFX/PSShaderMask.cpp \
	../../MParticleFX/PSShaderMesh.cpp \
	../../MParticleFX/PSShaderStandard.cpp \
	../../MParticleFX/PSSlice.cpp \
	../../MParticleFX/PSTypes.cpp
	
	
include $(BUILD_SHARED_LIBRARY)
