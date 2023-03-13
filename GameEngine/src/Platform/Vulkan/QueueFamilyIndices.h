#pragma once

#include <optional>

namespace GE
{
	struct QueueFamilityIndices
	{
		std::optional<uint32_t> m_GraphicsFamily;
		std::optional<uint32_t> m_PresentFamily;

		bool IsComplete() const
		{
			return m_GraphicsFamily.has_value() && m_PresentFamily.has_value();
		}
	};
}
