#pragma once

class GraphicsContext
{
public:

	GraphicsContext(class WinWindow* window) {};

	virtual ~GraphicsContext() {};

	virtual void DrawFrame() {};

	virtual void WaitIdle() {};

	static std::shared_ptr<GraphicsContext> Create(class WinWindow* window);
};