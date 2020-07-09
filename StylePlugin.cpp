// Copyright 2001-2019 Crytek GmbH / Crytek Group. All rights reserved.
#include "StdAfx.h"
#include "StylePlugin.h"

#include <CryCore/Platform/platform_impl.inl>
#include <Commands/ICommandManager.h>

namespace Private_StylePlugin
{
	void SampleCommand()
	{
		CryWarning(VALIDATOR_MODULE_EDITOR, VALIDATOR_WARNING, "Sample Command executed");
	}
}

REGISTER_EDITOR_COMMAND(Private_StylePlugin::SampleCommand, sample, sample_command, CCommandDescription(""));

REGISTER_PLUGIN(CStylePlugin);
