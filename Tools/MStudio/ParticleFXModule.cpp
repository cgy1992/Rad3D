#include "ParticleFXModule.h"

ParticleFXModule::ParticleFXModule()
	: EditorModule("ParticleFX")
{
	mParticleFXPanel = new ParticleFXPanel;
	mPropertyPanel = new ParticleFXPropertyPanel;
	mUserPanel = new ParticleFXUserPanel;

	Hide();
}

ParticleFXModule::~ParticleFXModule()
{
	delete mParticleFXPanel;
	delete mPropertyPanel;
	delete mUserPanel;
}

void ParticleFXModule::Layout()
{
	mParticleFXPanel->Layout();
	mPropertyPanel->Layout();
	mUserPanel->Layout();
}

void ParticleFXModule::Show()
{
	mParticleFXPanel->Show();
	mPropertyPanel->Show();
	mUserPanel->Show();
}

void ParticleFXModule::Hide()
{
	mParticleFXPanel->Hide();
	mPropertyPanel->Hide();
	mUserPanel->Hide();
}