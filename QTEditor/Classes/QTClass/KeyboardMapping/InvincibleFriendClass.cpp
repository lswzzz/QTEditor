#include "InvincibleFriendClass.h"
#include "Global.h"
#include "ControlView.h"
#include "BaseProperties.h"
#include "LayerManagerWidget.h"

InvincibleFriendClass::InvincibleFriendClass()
{
}


InvincibleFriendClass::~InvincibleFriendClass()
{
}

void InvincibleFriendClass::flipXEvent()
{
	auto controllerView = dynamic_cast<ControlView*>(g_ControlView);
	auto baseProperties = controllerView->getBaseProperties();
	baseProperties->switchFlipXState();
}

void InvincibleFriendClass::flipYEvent()
{
	auto controllerView = dynamic_cast<ControlView*>(g_ControlView);
	auto baseProperties = controllerView->getBaseProperties();
	baseProperties->switchFlipYState();
}

void InvincibleFriendClass::collapseCurrentLayer()
{
	auto layerManager = dynamic_cast<LayerManagerWidget*>(g_LayerManager);
	layerManager->collapseCurrentLayer();
}

void InvincibleFriendClass::expandCurrentLayer()
{
	auto layerManager = dynamic_cast<LayerManagerWidget*>(g_LayerManager);
	layerManager->expandCurrentLayer();
}