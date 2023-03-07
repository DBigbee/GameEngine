#pragma once

class Texture2D
{
public:

	Texture2D(const std::string& filePath);

	virtual ~Texture2D();

	int32_t GetWidth() const { return m_Width; }

	int32_t GetHeight() const { return m_Height; }

	int32_t GetChannels() const { return m_Channels; }

private:

	void Invalidate(uint8_t* data);

	uint32_t m_Width = 0;

	uint32_t m_Height = 0;

	uint32_t m_Channels = 0;

	std::string m_FilePath;
};