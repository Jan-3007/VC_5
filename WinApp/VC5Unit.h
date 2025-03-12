#pragma once



class VC5Unit
{
protected:
	// LED
	uint8_t led_index_;
	ColorTemplate led_color_;

	uint8_t display_index_;
	std::wstring name_;

	// current state
	bool mute_;
	uint volume_ {0};

public:
	// ctor
	VC5Unit(uint8_t led_index, uint8_t display_index, std::wstring name, bool mute)
		: led_index_ {led_index}, display_index_ {display_index}, name_ {name}, mute_ {mute}
	{
		if (mute_)
		{
			set_led_color(mute_on);
		}
		else
		{
			set_led_color(mute_off);
		}
	}

	bool
	is_mute() const
		{
			return mute_;
		}

	void
	set_mute(const bool val)
		{
			mute_ = val;
			if(val)
			{
				// mute active
				set_led_color(mute_on);
			}
			else
			{
				// mute inactive
				set_led_color(mute_off);
			}
		}

	uint
	get_volume() const
		{
			return volume_;
		}
	
	void
	change_volume(const int8_t val)
		{
			int new_vol = volume_ + val;
			if (new_vol >= 0 && new_vol <= 100)
			{
				volume_ = new_vol;
			}
			else if (new_vol > 100)
			{
				volume_ = 100;
			}
			else
			{
				volume_ = 0;
			}
		}

	void
	update_mute()
		{
			if (volume_ == 0)
			{
				set_mute(true);
			}
			else
			{
				set_mute(false);
			}
		}

	uint8_t
	get_led_index() const
		{
			return led_index_;
		}

	void
	set_led_color(const ColorTemplate& color)
		{
			led_color_ = color;
		}

	ColorTemplate
	get_led_color() const
		{
			return led_color_;
		}

	std::wstring
	get_name() const
		{
			return name_;
		}

	void
	set_name(const std::wstring& name)
		{
			name_ = name;
		}

	uint8_t
	get_display_index() const
		{
			return display_index_;
		}


};

