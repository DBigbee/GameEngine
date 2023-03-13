#pragma once


namespace GE
{
	class Texture
	{
	public:

		virtual ~Texture()  = default;

		virtual int32_t GetWidth() const = 0;

		virtual int32_t GetHeight() const = 0;

		virtual int32_t GetChannels() const = 0;

		virtual std::string GetPath() const = 0;
	};

	class Texture2D : public Texture
	{
	public:

		static Ref<Texture2D> Create(const std::string& path);
	};
}