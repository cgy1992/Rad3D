
include $(CLEAR_VARS)

LOCAL_MODULE     := libOde

LOCAL_CFLAGS += "-DdNODEBUG"
LOCAL_CFLAGS += "-DdIDESINGLE"
LOCAL_CFLAGS += "-DCCD_IDESINGLE"
LOCAL_CFLAGS += "-D_USE_MATH_DEFINES"


LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/../../ThirdParty/ode/include \
	$(LOCAL_PATH)/../../ThirdParty/ode/ode/src \
	$(LOCAL_PATH)/../../ThirdParty/ode/OPCODE \
	$(LOCAL_PATH)/../../ThirdParty/ode/ou/include 
	
LOCAL_SRC_FILES  := \
	../../ThirdParty/ode/ode/src/array.cpp \
	../../ThirdParty/ode/ode/src/box.cpp \
	../../ThirdParty/ode/ode/src/capsule.cpp \
	../../ThirdParty/ode/ode/src/collision_cylinder_box.cpp \
	../../ThirdParty/ode/ode/src/collision_cylinder_plane.cpp \
	../../ThirdParty/ode/ode/src/collision_cylinder_sphere.cpp \
	../../ThirdParty/ode/ode/src/collision_cylinder_trimesh.cpp \
	../../ThirdParty/ode/ode/src/collision_kernel.cpp \
	../../ThirdParty/ode/ode/src/collision_quadtreespace.cpp \
	../../ThirdParty/ode/ode/src/collision_sapspace.cpp \
	../../ThirdParty/ode/ode/src/collision_space.cpp \
	../../ThirdParty/ode/ode/src/collision_transform.cpp \
	../../ThirdParty/ode/ode/src/collision_trimesh_box.cpp \
	../../ThirdParty/ode/ode/src/collision_trimesh_ccylinder.cpp \
	../../ThirdParty/ode/ode/src/collision_trimesh_disabled.cpp \
	../../ThirdParty/ode/ode/src/collision_trimesh_distance.cpp \
	../../ThirdParty/ode/ode/src/collision_trimesh_gimpact.cpp \
	../../ThirdParty/ode/ode/src/collision_trimesh_opcode.cpp \
	../../ThirdParty/ode/ode/src/collision_trimesh_plane.cpp \
	../../ThirdParty/ode/ode/src/collision_trimesh_ray.cpp \
	../../ThirdParty/ode/ode/src/collision_trimesh_sphere.cpp \
	../../ThirdParty/ode/ode/src/collision_trimesh_trimesh.cpp \
	../../ThirdParty/ode/ode/src/collision_trimesh_trimesh_new.cpp \
	../../ThirdParty/ode/ode/src/collision_util.cpp \
	../../ThirdParty/ode/ode/src/convex.cpp \
	../../ThirdParty/ode/ode/src/cylinder.cpp \
	../../ThirdParty/ode/ode/src/error.cpp \
	../../ThirdParty/ode/ode/src/export-dif.cpp \
	../../ThirdParty/ode/ode/src/fastdot.c \
	../../ThirdParty/ode/ode/src/fastldlt.c \
	../../ThirdParty/ode/ode/src/fastlsolve.c \
	../../ThirdParty/ode/ode/src/fastltsolve.c \
	../../ThirdParty/ode/ode/src/heightfield.cpp \
	../../ThirdParty/ode/ode/src/lcp.cpp \
	../../ThirdParty/ode/ode/src/mass.cpp \
	../../ThirdParty/ode/ode/src/mat.cpp \
	../../ThirdParty/ode/ode/src/matrix.cpp \
	../../ThirdParty/ode/ode/src/memory.cpp \
	../../ThirdParty/ode/ode/src/misc.cpp \
	../../ThirdParty/ode/ode/src/nextafterf.c \
	../../ThirdParty/ode/ode/src/objects.cpp \
	../../ThirdParty/ode/ode/src/obstack.cpp \
	../../ThirdParty/ode/ode/src/ode.cpp \
	../../ThirdParty/ode/ode/src/odeinit.cpp \
	../../ThirdParty/ode/ode/src/odemath.cpp \
	../../ThirdParty/ode/ode/src/odeou.cpp \
	../../ThirdParty/ode/ode/src/odetls.cpp \
	../../ThirdParty/ode/ode/src/plane.cpp \
	../../ThirdParty/ode/ode/src/quickstep.cpp \
	../../ThirdParty/ode/ode/src/ray.cpp \
	../../ThirdParty/ode/ode/src/rotation.cpp \
	../../ThirdParty/ode/ode/src/sphere.cpp \
	../../ThirdParty/ode/ode/src/step.cpp \
	../../ThirdParty/ode/ode/src/threading_base.cpp \
	../../ThirdParty/ode/ode/src/threading_impl.cpp \
	../../ThirdParty/ode/ode/src/threading_pool_posix.cpp \
	../../ThirdParty/ode/ode/src/threading_pool_win.cpp \
	../../ThirdParty/ode/ode/src/timer.cpp \
	../../ThirdParty/ode/ode/src/util.cpp \
	\
	../../ThirdParty/ode/ode/src/joints/amotor.cpp \
	../../ThirdParty/ode/ode/src/joints/ball.cpp \
	../../ThirdParty/ode/ode/src/joints/contact.cpp \
	../../ThirdParty/ode/ode/src/joints/dball.cpp \
	../../ThirdParty/ode/ode/src/joints/dhinge.cpp \
	../../ThirdParty/ode/ode/src/joints/fixed.cpp \
	../../ThirdParty/ode/ode/src/joints/hinge.cpp \
	../../ThirdParty/ode/ode/src/joints/hinge2.cpp \
	../../ThirdParty/ode/ode/src/joints/joint.cpp \
	../../ThirdParty/ode/ode/src/joints/lmotor.cpp \
	../../ThirdParty/ode/ode/src/joints/null.cpp \
	../../ThirdParty/ode/ode/src/joints/piston.cpp \
	../../ThirdParty/ode/ode/src/joints/plane2d.cpp \
	../../ThirdParty/ode/ode/src/joints/pr.cpp \
	../../ThirdParty/ode/ode/src/joints/pu.cpp \
	../../ThirdParty/ode/ode/src/joints/slider.cpp \
	../../ThirdParty/ode/ode/src/joints/transmission.cpp \
	../../ThirdParty/ode/ode/src/joints/universal.cpp \
	\
	../../ThirdParty/ode/OPCODE/Ice/IceAABB.cpp \
	../../ThirdParty/ode/OPCODE/Ice/IceContainer.cpp \
	../../ThirdParty/ode/OPCODE/Ice/IceHPoint.cpp \
	../../ThirdParty/ode/OPCODE/Ice/IceIndexedTriangle.cpp \
	../../ThirdParty/ode/OPCODE/Ice/IceMatrix3x3.cpp \
	../../ThirdParty/ode/OPCODE/Ice/IceMatrix4x4.cpp \
	../../ThirdParty/ode/OPCODE/Ice/IceOBB.cpp \
	../../ThirdParty/ode/OPCODE/Ice/IcePlane.cpp \
	../../ThirdParty/ode/OPCODE/Ice/IcePoint.cpp \
	../../ThirdParty/ode/OPCODE/Ice/IceRandom.cpp \
	../../ThirdParty/ode/OPCODE/Ice/IceRay.cpp \
	../../ThirdParty/ode/OPCODE/Ice/IceRevisitedRadix.cpp \
	../../ThirdParty/ode/OPCODE/Ice/IceSegment.cpp \
	../../ThirdParty/ode/OPCODE/Ice/IceTriangle.cpp \
	../../ThirdParty/ode/OPCODE/Ice/IceUtils.cpp \
	\
	../../ThirdParty/ode/OPCODE/OPC_AABBCollider.cpp \
	../../ThirdParty/ode/OPCODE/OPC_AABBTree.cpp \
	../../ThirdParty/ode/OPCODE/OPC_BaseModel.cpp \
	../../ThirdParty/ode/OPCODE/OPC_Collider.cpp \
	../../ThirdParty/ode/OPCODE/OPC_Common.cpp \
	../../ThirdParty/ode/OPCODE/OPC_HybridModel.cpp \
	../../ThirdParty/ode/OPCODE/OPC_LSSCollider.cpp \
	../../ThirdParty/ode/OPCODE/OPC_MeshInterface.cpp \
	../../ThirdParty/ode/OPCODE/OPC_Model.cpp \
	../../ThirdParty/ode/OPCODE/OPC_OBBCollider.cpp \
	../../ThirdParty/ode/OPCODE/OPC_OptimizedTree.cpp \
	../../ThirdParty/ode/OPCODE/OPC_Picking.cpp \
	../../ThirdParty/ode/OPCODE/OPC_PlanesCollider.cpp \
	../../ThirdParty/ode/OPCODE/OPC_RayCollider.cpp \
	../../ThirdParty/ode/OPCODE/OPC_SphereCollider.cpp \
	../../ThirdParty/ode/OPCODE/OPC_TreeBuilders.cpp \
	../../ThirdParty/ode/OPCODE/OPC_TreeCollider.cpp \
	../../ThirdParty/ode/OPCODE/OPC_VolumeCollider.cpp \
	../../ThirdParty/ode/OPCODE/Opcode.cpp \
	\
	../../ThirdParty/ode/ou/src/atomic.cpp \
	../../ThirdParty/ode/ou/src/customization.cpp \
	../../ThirdParty/ode/ou/src/malloc.cpp \
	../../ThirdParty/ode/ou/src/threadlocalstorage.cpp \
	../../ThirdParty/ode/ou/test/outest.cpp


include $(BUILD_STATIC_LIBRARY)
