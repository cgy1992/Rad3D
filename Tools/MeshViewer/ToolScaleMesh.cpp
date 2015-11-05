#include "ToolScaleMesh.h"

ToolScaleMesh::ToolScaleMesh()
{
	mLayout = new MGUI::Layout(NULL, NULL);
	mLayout->SetOrder(MGUI::eOrder::OVERLAP);
	mLayout->SetAlign(MGUI::eAlign::STRETCH);

	MGUI::Layout * layout = MGUI::Layout::Load("ScaleMesh.layout", mLayout);
	layout->SetAlign(MGUI::eAlign::CENTER);

	MGUI::Button * bnOK = layout->GetChildT<MGUI::Button>("OK");
	MGUI::Button * bnCancel = layout->GetChildT<MGUI::Button>("Cancel");
	mScaleBox = layout->GetChildT<MGUI::EditBox>("Scale");

	bnOK->E_Click += new cListener1<ToolScaleMesh, const MGUI::ClickEvent *>(this, &ToolScaleMesh::OnOK);
	bnCancel->E_Click += new cListener1<ToolScaleMesh, const MGUI::ClickEvent *>(this, &ToolScaleMesh::OnCancel);

	mLayout->SetVisible(false);
}

ToolScaleMesh::~ToolScaleMesh()
{
	delete mLayout;
}

void ToolScaleMesh::Show(MeshSourcePtr mesh)
{
	mLayout->SetVisible(true);

	mMesh = mesh;
}

void ToolScaleMesh::OnOK(const MGUI::ClickEvent * e)
{
	mLayout->SetVisible(false);

	String text;
	text.FromUnicode(mScaleBox->GetCaption().c_str());

	float scale = text.ToFloat();
	for (int i = 0; i < mMesh->GetMeshBufferCount(); ++i)
	{
		MeshBuffer * mb = mMesh->GetMeshBuffer(i);

		RenderOp * pRenderOp = mb->GetRenderOp();
		int offset = pRenderOp->vertexDeclarations[0].GetElementOffset(eVertexSemantic::POSITION);
		int stride = pRenderOp->vertexBuffers[0]->GetStride();
		int count = pRenderOp->vertexBuffers[0]->GetCount();

		unsigned char * vdata = (unsigned char *)pRenderOp->vertexBuffers[0]->Lock(eLockFlag::READ_WRITE);
		for (int j = 0; j < count; ++j)
		{
			Float3 & p = *(Float3 *)(vdata + offset);

			p *= scale;

			vdata += stride;
		}
		pRenderOp->vertexBuffers[0]->Unlock();
	}

	mMesh->GenAabbFromRenderMesh();
	mMesh = NULL;
}

void ToolScaleMesh::OnCancel(const MGUI::ClickEvent * e)
{
	mLayout->SetVisible(false);

	mMesh = NULL;
}