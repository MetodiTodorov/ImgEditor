#pragma once

///////////////////////////////////////////////////////////////////////////////
class Application;

///////////////////////////////////////////////////////////////////////////////
class CommandProcessor
{
public:
	CommandProcessor(Application* app);

private:
	Application*	app_;
};

///////////////////////////////////////////////////////////////////////////////