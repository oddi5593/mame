// license:BSD-3-Clause
// copyright-holders:Zsolt Vasvari
#ifndef MAME_INCLUDES_SUPRLOCO_H
#define MAME_INCLUDES_SUPRLOCO_H

#pragma once

#include "emupal.h"

class suprloco_state : public driver_device
{
public:
	suprloco_state(const machine_config &mconfig, device_type type, const char *tag) :
		driver_device(mconfig, type, tag),
		m_maincpu(*this, "maincpu"),
		m_audiocpu(*this, "audiocpu"),
		m_gfxdecode(*this, "gfxdecode"),
		m_spriteram(*this, "spriteram"),
		m_videoram(*this, "videoram"),
		m_scrollram(*this, "scrollram"),
		m_decrypted_opcodes(*this, "decrypted_opcodes")
	{ }

	void suprloco(machine_config &config);

	void init_suprloco();

private:
	required_device<cpu_device> m_maincpu;
	required_device<cpu_device> m_audiocpu;
	required_device<gfxdecode_device> m_gfxdecode;

	required_shared_ptr<uint8_t> m_spriteram;
	required_shared_ptr<uint8_t> m_videoram;
	required_shared_ptr<uint8_t> m_scrollram;
	required_shared_ptr<uint8_t> m_decrypted_opcodes;

	tilemap_t *m_bg_tilemap;
	int m_control;

	DECLARE_WRITE8_MEMBER(videoram_w);
	DECLARE_WRITE8_MEMBER(scrollram_w);
	DECLARE_WRITE8_MEMBER(control_w);

	TILE_GET_INFO_MEMBER(get_tile_info);

	virtual void video_start() override;
	DECLARE_PALETTE_INIT(suprloco);

	uint32_t screen_update(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	inline void draw_pixel(bitmap_ind16 &bitmap,const rectangle &cliprect,int x,int y,int color,int flip);
	void draw_sprite(bitmap_ind16 &bitmap,const rectangle &cliprect,int spr_number);
	void draw_sprites(bitmap_ind16 &bitmap, const rectangle &cliprect);
	void decrypted_opcodes_map(address_map &map);
	void main_map(address_map &map);
	void sound_map(address_map &map);
};

#endif // MAME_INCLUDES_SUPRLOCO_H
