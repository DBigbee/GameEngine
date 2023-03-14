#pragma once

namespace GE
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

	protected:
		virtual std::string ReadFile(const std::string& filename);

	public:
		static Ref<Shader> Create(const std::string& filename);
	};
}