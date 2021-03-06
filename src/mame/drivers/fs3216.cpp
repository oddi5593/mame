// license:BSD-3-Clause
// copyright-holders:AJR
/****************************************************************************

    Skeleton driver for Fortune 32:16.

****************************************************************************/

#include "emu.h"
//#include "bus/rs232/rs232.h"
#include "cpu/m68000/m68000.h"
#include "cpu/8x300/8x300.h"
#include "cpu/mcs48/mcs48.h"
//#include "machine/com8116.h"
//#include "machine/upd765.h"
#include "machine/z80ctc.h"
#include "machine/z80dart.h"
#include "video/mc6845.h"
#include "screen.h"


class fs3216_state : public driver_device
{
public:
	fs3216_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag)
		, m_maincpu(*this, "maincpu")
	{
	}

	void fs3216(machine_config &config);

private:
	MC6845_UPDATE_ROW(update_row);

	void main_map(address_map &map);
	void wdcpu_prog_map(address_map &map);
	void wdcpu_bank_map(address_map &map);

	required_device<cpu_device> m_maincpu;
};


MC6845_UPDATE_ROW(fs3216_state::update_row)
{
}


void fs3216_state::main_map(address_map &map)
{
	map(0x000000, 0x003fff).rom().region("bios", 0);
	map(0x780000, 0x783fff).rom().region("bios", 0);
	map(0x800000, 0x803fff).rom().region("bios", 0);
}

void fs3216_state::wdcpu_prog_map(address_map &map)
{
	map(0x000, 0x7ff).rom().region("wdcpu", 0);
}

void fs3216_state::wdcpu_bank_map(address_map &map)
{
}


void fs3216_state::fs3216(machine_config &config)
{
	M68000(config, m_maincpu, 44.2368_MHz_XTAL / 8); // 5.5 MHz
	m_maincpu->set_addrmap(AS_PROGRAM, &fs3216_state::main_map);

	Z80CTC(config, "ctc", 44.2368_MHz_XTAL / 8); // Z8430BPS

	Z80DART(config, "dart", 44.2368_MHz_XTAL / 8); // Z8470BPS

	mc6845_device &crtc(MC6845(config, "crtc", 14.58_MHz_XTAL / 10)); // HD46505RP; clock unknown
	crtc.set_char_width(10); // unknown
	crtc.set_show_border_area(false);
	crtc.set_update_row_callback(FUNC(fs3216_state::update_row), this);

	screen_device &screen(SCREEN(config, "screen", SCREEN_TYPE_RASTER));
	screen.set_raw(14.58_MHz_XTAL, 900, 0, 800, 270, 0, 250); // parameters guessed
	screen.set_screen_update("crtc", FUNC(mc6845_device::screen_update));

	n8x300_cpu_device &wdcpu(N8X300(config, "wdcpu", 20_MHz_XTAL / 2)); // N8X305I
	wdcpu.set_addrmap(AS_PROGRAM, &fs3216_state::wdcpu_prog_map);
	wdcpu.set_addrmap(AS_IO, &fs3216_state::wdcpu_bank_map);
}


static INPUT_PORTS_START(fs3216)
INPUT_PORTS_END


// XTALs on Mother Board (1001176-01 Rev. 5): 44.2368 MHz (22A), 16.000 MHz (2J)
// XTALs on Comm A board (1000014-01 2 Port / 1000171-01 4 Port Rev. 7; 10000065-01 Rev. 3): two K1135CM Dual Baud Rate Generators (7D, 8D)
// XTALs on Comm B board (1001651-01 Rev. G): none
// XTALs on Video Controller board (1000443-1 Rev. I): 14.580 MHz (1H)
// XTALs on WD-1001 CLB Disk Controller board (1473-008): 20.000 (Y1), 8.00? [somewhat defaced] (Y2)
ROM_START(fs3216)
	ROM_REGION16_BE(0x4000, "bios", 0)
	ROM_LOAD16_BYTE("17k_1260-02_h.bin", 0x0000, 0x2000, CRC(75ed6de8) SHA1(0360548493b778995ae436da475b6356945e1872))
	ROM_LOAD16_BYTE("15k_1260-01_l.bin", 0x0001, 0x2000, CRC(82695233) SHA1(0d69309f41306298bf6a4ba6928c53f908bb3f2c))

	ROM_REGION16_BE(0x2000, "comm_a", 0)
	ROM_LOAD16_BYTE("1896-01_c90c3cb92588a2b4bb28bcf4bb8e2023.bin", 0x0000, 0x1000, CRC(ac4cdbd2) SHA1(e448a01a9809cccfb526ac1d4e97d9be3af1e5eb))
	ROM_LOAD16_BYTE("1895-01_fb20aa682a17028cdae2687fc47daef1.bin", 0x0001, 0x1000, CRC(82ebffb5) SHA1(3888b7ba07d0b25bfb9e0444215d4fa9ecd66273))

	ROM_REGION16_BE(0x2000, "comm_b", 0)
	ROM_LOAD16_BYTE("1658-04_b99b2d6b67222a571cc9879f98f2136f.bin", 0x0000, 0x1000, CRC(18b43218) SHA1(ded1419185693350ed2d5868819b0db0c2917ff3))
	ROM_LOAD16_BYTE("1658-03_8425b4008a0fa9092158eb4683110e0f.bin", 0x0001, 0x1000, CRC(83bcdf34) SHA1(782cc58b179b0a42dfe9e09f465582b1420c0c4a))

	ROM_REGION16_BE(0x2000, "video", 0)
	ROM_LOAD16_BYTE("1148-02_3dda8d9a72db50bfc8c2eab697b14952.bin", 0x0000, 0x1000, CRC(ce8f42a4) SHA1(4cfb967890de069270a068b4d4f1f5cf6a9a4c7b))
	ROM_LOAD16_BYTE("1148-01_da087b69caa08bf7c8433a2e089143ce.bin", 0x0001, 0x1000, CRC(72f4c435) SHA1(679fab926b45c99ab19baa75d0d7002d4a5d9299))

	ROM_REGION(0x1000, "chargen", 0)
	ROM_LOAD("370-01_baf9a45321f489df3ee41175f29105b6.bin", 0x0000, 0x1000, CRC(4d3f1c2a) SHA1(6b5b03757ea53a39675ef442d090e5608bd659c3))

	ROM_REGION16_BE(0x2000, "wd1001_clb", 0)
	ROM_LOAD16_BYTE("u29_1139-02_d54c89bf6a84505b3b9ae05f06597c8d.bin", 0x0000, 0x1000, CRC(396b709a) SHA1(dc1ddef8a16c0529bf76fcd5933ba52e0409e3f9))
	ROM_LOAD16_BYTE("u28_1139-01_3b99c99f03cb4788cc0142e7a9497cda.bin", 0x0001, 0x1000, CRC(c42b7678) SHA1(bec25327cf0bcc8edcb09605cbb609b5708b89f6))

	ROM_REGION16_LE(0x800, "wdcpu", 0) // 2x N82S181N
	ROM_LOAD16_BYTE("u35_ap2001r4m_a794e8b07817734303ede17f38a91e0b_ms2012.bin", 0x000, 0x400, CRC(833a60c9) SHA1(f6414623fc52d030df8814befda02928e2ac5771))
	ROM_LOAD16_BYTE("u53_ap2000r4l_9b8ee868fd1129000b3168350114b6da.bin", 0x001, 0x400, CRC(91b21c9b) SHA1(87fb5b1d5804f771782ceab74fd8d7c97e189bc7))

	ROM_REGION(0x400, "wd1001_prom", 0) // N82S181N (address decoding?)
	ROM_LOAD("u26_ap2002r4f_b3ae6f8966230689d34c82b3c9d817ac.bin", 0x000, 0x400, CRC(fcd31bff) SHA1(ae34c6eb6659dc992896b388be1badfab0fd7971))
ROM_END

COMP(1982, fs3216, 0, 0, fs3216, fs3216, fs3216_state, empty_init, "Fortune Systems", "Fortune 32:16", MACHINE_IS_SKELETON)
