#include "SkeletonDisplayer.h"

SkeletonDisplayer::SkeletonDisplayer()
{
	mSkeleton = NULL;
	mLayout = NULL;

	World::Instance()->E_RenderSolidEnd += new cListener0<SkeletonDisplayer>(this, &SkeletonDisplayer::OnRender);
}

SkeletonDisplayer::~SkeletonDisplayer()
{
	safe_delete (mLayout);
}

void SkeletonDisplayer::Attach(Mesh * mesh)
{
	safe_delete (mLayout);
	mSkeleton = NULL;

	if (mesh != NULL && mesh->GetSkeleton() != NULL)
	{
		mMesh = mesh;
		mSkeleton = mesh->GetSkeleton().c_ptr();

		mLayout = new MGUI::Layout(NULL, NULL);
		mLayout->SetAlign(MGUI::eAlign::STRETCH);
		mLayout->SetPickFlag(MGUI::ePickFlag::NONE);

		mTextBox = new MGUI::TextBox(NULL, mLayout);
		//mTextBox->SetAlign(MGUI::eAlign::RIGHT | MGUI::eAlign::BOTTOM);
	}
}

void SkeletonDisplayer::OnRender()
{
	if (mSkeleton == NULL)
		return ;

	Float3 size = mMesh->GetWorldAabb().GetSize();
	float radius = Max3(size.x, size.y, size.z) / 100;

	float x = MGUI::InputManager::Instance()->_getMousePosition().x;
	float y = MGUI::InputManager::Instance()->_getMousePosition().y;

	const Viewport & vp = World::Instance()->MainRenderContext()->GetViewport();
	x -= vp.x;
	y -= vp.y;
	x /= vp.w;
	y /= vp.h;

	Ray ray = World::Instance()->MainCamera()->GetViewportRay(x, y);
	bool checked = false;
	mTextBox->SetCaption(WS(""));

	RenderSystem::Instance()->SetRenderState(eFillMode::SOLID, eCullMode::NONE);

	for (int i = 0; i < mSkeleton->GetBoneCount(); ++i)
	{
		Bone * bone = mSkeleton->GetBone(i);

		Sphere sph;
		sph.center = bone->GetWorldPosition();
		sph.radius = radius;

		if (!checked && ray.Intersect(NULL, sph))
		{
			RenderHelper::Instance()->DebugDrawSphere(sph, Float4(1, 1, 0), Mat4::Identity);
			
			Float3 projPos = sph.center * World::Instance()->MainCamera()->GetViewProjTM();
			projPos.x = projPos.x * 0.5f + 0.5f;
			projPos.y = 0.5f - projPos.y * 0.5f;
			projPos.x = vp.x + projPos.x * vp.w;
			projPos.y = vp.y + projPos.y * vp.h;

			mTextBox->SetCaption(bone->GetName().c_wstr());
			mTextBox->SetRectPosition(projPos.x + 15, projPos.y - 15);
			
			checked = true;
		}
		else
		{
			RenderHelper::Instance()->DebugDrawSphere(sph, Float4(0, 1, 0), Mat4::Identity);
		}
	}

	Array<Float3> lines;
	for (int i = 0; i < mSkeleton->GetBoneCount(); ++i)
	{
		Bone * bone = mSkeleton->GetBone(i);;
		Bone * parent = bone->GetParentBone();

		if (parent)
		{
			lines.PushBack(parent->GetWorldPosition());
			lines.PushBack(bone->GetWorldPosition());
		}
	}

	if (lines.Size() > 0)
	{
		RenderHelper::Instance()->DebugDraw(&lines[0], ePrimType::POINT_LIST, lines.Size() / 2, Float4(0, 1, 0), Mat4::Identity);
	}
}