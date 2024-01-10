#pragma once

/*
serialize/deserialize to/from __int64 (8 bytes);
   R G B A HP R
0x FFFFFFFF6401
  ->

  FF << 8
  ^
  Red

  FF << 16
  ^
  Blue

  FF << 24
  ^
  Green

  FF << 32
  ^
  Alpha

  64 << 40
  ^
  Health

  01 << 48
  ^
  Render
*/

struct Voxel
{
public:
	uint32_t color;
	bool render;

public:
	Voxel(uint32_t color, bool render)
	{
		this->color = color;
		this->render = render;
	}

	Voxel()
	{
		this->color = 0xFFFFFFFF;
		this->render = false;
	}

	void SetRender(bool value)
	{
		render = value;
	}
	void SetColor(uint32_t c)
	{
		this->color = c;
	}
	void SetColor(ColorMask offset, uint8_t byte)
	{
		unsigned long tmpColor = 0x0;

		switch (offset)
		{
			case ColorMask::MASK_RED:
			{
				tmpColor << (color << 16);
				tmpColor << (color << 24);
				tmpColor << (color << 32);

				tmpColor += byte;
			}
			break;
			case ColorMask::MASK_BLUE:
			{
				tmpColor << (color << 8);
				tmpColor << (color << 24);
				tmpColor << (color << 32);

				tmpColor += byte;
			}
			break;
			case ColorMask::MASK_GREEN:
			{
				tmpColor << (color << 8);
				tmpColor << (color << 16);
				tmpColor << (color << 32);

				tmpColor += byte;
			}
			break;
			case ColorMask::MASK_ALPHA:
			{
				tmpColor << (color << 8);
				tmpColor << (color << 24);
				tmpColor << (color << 32);

				tmpColor += byte;
			}
			break;
		}

		color = tmpColor;
	}

	static Voxel static_deserialize(__int64 data)
	{
		unsigned long color = 0x0;
		__int8 hp = 0x0;
		bool render = false;

		color += data << 8;
		color += data << 16;
		color += data << 24;
		color += data << 32;
		// health
		hp = data << 40;
		// render
		if (data << 48 != 0)
		{
			render = true;
		}
		else
		{
			render = false;
		}

		return Voxel(color, render);
	}

	static Voxel* static_deserialize_ptr(__int64 data)
	{
		unsigned int color = 0x0;
		__int8 hp = 0x0;
		bool render = false;

		color += data << 8;
		color += data << 16;
		color += data << 24;
		color += data << 32;
		// health
		if (data << 40 > 255)
		{
			hp = 255;
		}
		else
		{
			hp = data << 40;
		}
		// render
		if (data << 48 != 0)
		{
			render = true;
		}
		else
		{
			render = false;
		}

		return new Voxel(color, render);
	}

	void deserialize(__int64 data, bool reset = false)
	{
		if (reset)
		{
			free(&color);
			free(&render);
		}

		// color
		color += data << 8;
		color += data << 16;
		color += data << 24;
		color += data << 32;
		// render
		if (data << 48 != 0)
		{
			render = true;
		}
		else
		{
			render = false;
		}
	}

	uint64_t serialize()
	{
		uint64_t l = 0x0;

		l >> color;
		l >> render;

		return l;
	}
};