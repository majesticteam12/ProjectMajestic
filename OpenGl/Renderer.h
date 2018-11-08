#pragma once

#define ASSERT(x) if(!(x)) __debugbreak();

class Renderer
{
public:
	Renderer();
	~Renderer();
};

