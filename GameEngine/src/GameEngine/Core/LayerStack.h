#pragma once

#include "Layer.h"

namespace GE
{
	
	class LayerStack
	{
	public:
		using Layers = std::vector<Layer*>;
		using Iter = Layers::iterator;
		using Const_Iter = Layers::const_iterator;
		using Rev_Iter = Layers::reverse_iterator;
		using Const_Rev_Iter = Layers::const_reverse_iterator;

	public:

		LayerStack() = default;
		~LayerStack();

		void Push(Layer* layer);
		void PushOverlay(Layer* overlay);
		void Pop(Layer* layer);
		void PopOverlay(Layer* overlay);

		Iter begin() { return m_Layers.begin(); }
		Iter end() { return m_Layers.end(); }
		Rev_Iter rbegin() { return m_Layers.rbegin(); }
		Rev_Iter rend() { return m_Layers.rend(); }

		Const_Iter begin() const { return m_Layers.begin(); }
		Const_Iter end() const { return m_Layers.end(); }
		Const_Rev_Iter rbegin() const { return m_Layers.rbegin(); }
		Const_Rev_Iter rend() const { return m_Layers.rend(); }

	private:

		Layers m_Layers;
		unsigned int m_LayerInsertIndex = 0;
	};
}