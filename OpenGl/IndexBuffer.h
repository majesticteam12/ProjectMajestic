#pragma once

#include <GL\glew.h>

class IndexBuffer
{
private:
	unsigned int m_rendererID;
	unsigned int m_count;
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const ;
	void UnBind() const ;

	unsigned int GetCount() const ;
};

