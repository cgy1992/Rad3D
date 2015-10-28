/*
*	Mesh Serializer
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MMeshSource.h"

namespace Rad {

	#define MC_MESH_BUFFER                      0x0100
	//#define MC_MESH_MATERIAL					0x0101
	#define MC_MESH_SHADER						0x0102
	#define MC_BOUND							0x0200
	#define MC_SKELETON                         0x0300
	#define MC_ANIMATION0						0x0001
	#define MC_ANIMATION						0x0400
	#define MC_ANIMATION_EVENT					0x0401

	#define MODEL_FILE_MAGIC                    "RadMesh"
	#define MODEL_FILE_MAGIC_LEN				(12)
	#define MODEL_FILE_VERSION					(('M' << 24) | ('D' << 16) | ('L' << 8) | 0)


	class M_ENTRY MeshSerializer
	{
	public:
		enum eVertexElement {
			VE_POSITION = 1 << 0,
			VE_NORMAL = 1 << 1,
			VE_COLOR = 1 << 2,
			VE_TANGENT = 1 << 3,
			VE_TEXCOORD = 1 << 4,
			VE_LIGHTMAPUV = 1 << 5,
			VE_BONEWEIGHT = 1 << 6,
			VE_BONEINDEX = 1 << 7
		};

		enum {
			K_File_Verion = MODEL_FILE_VERSION + 1,

			K_MeshBuffer_V0 = 0,
			K_MeshBuffer_V1 = 1,

			K_Material_V0 = 0,
			K_Material_V1 = 1,
			K_Material_V2 = 2,

			K_Bound_V0 = 0,
			K_Bound_V1 = 1,

			K_Skeleton_V0 = 0,
			K_Skeleton_V1 = 1,
			K_Skeleton_V2 = 2,
		};

	public:
		static void 
			Load(MeshSource * mesh, DataStreamPtr stream);

		static void 
			ReadMeshBuffer(MeshSource * mesh, MeshBuffer * mb, DataStreamPtr stream);
		static void
			ReadMeshMaterial(MeshSource * mesh, MeshBuffer * mb, DataStreamPtr stream);
		static void
			ReadMeshShader(MeshSource * mesh, MeshBuffer * mb, DataStreamPtr stream);
		static void 
			ReadSkeleton(MeshSource * mesh, DataStreamPtr stream);
		static void 
			ReadBound(MeshSource * mesh, DataStreamPtr stream);

		static int 
			GenVertexDecl(RenderOp * rop, int vertexElems);

	public:
		static bool 
			Save(MeshSource * mesh, const String & filename);
		
		static void 
			WriteMeshBuffer(MeshBuffer * mb, OSerializer * stream);
		static void
			WriteMeshMaterial(MeshBuffer * mb, OSerializer * stream);
		static void
			WriteMeshShader(MeshBuffer * mb, OSerializer * stream);
		static void 
			WriteSkeleton(MeshSource * mesh, OSerializer * stream);
		static void 
			WriteBound(MeshSource * mesh, OSerializer * stream);
	};

}